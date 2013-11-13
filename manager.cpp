/*
Copyright 2008,2011 Will Stephenson <wstephenson@kde.org>
Copyright 2010 Lamarque Souza <lamarque@kde.org>
Copyright 2013 Daniel Nicoletti <dantti12@gmail.com>
Copyright 2013 Lukas Tinkl <ltinkl@redhat.com>

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) version 3, or any
later version accepted by the membership of KDE e.V. (or its
successor approved by the membership of KDE e.V.), which shall
act as a proxy defined in Section 6 of version 3 of the license.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "manager.h"
#include "manager_p.h"
#include "macros.h"
#include "mmdebug.h"
#include "generic-types.h"
#include "modem.h"

MM_GLOBAL_STATIC(ModemManager::ModemManagerPrivate, globalModemManager)

ModemManager::ModemManagerPrivate::ModemManagerPrivate()
    : watcher(MM_DBUS_SERVICE, QDBusConnection::systemBus(), QDBusServiceWatcher::WatchForRegistration | QDBusServiceWatcher::WatchForUnregistration, this)
    , iface(MM_DBUS_SERVICE, MM_DBUS_PATH, QDBusConnection::systemBus(), this)
    , manager(MM_DBUS_SERVICE, MM_DBUS_PATH, QDBusConnection::systemBus(), this)
{
    qDBusRegisterMetaType<QList<QDBusObjectPath> >();
    qDBusRegisterMetaType<NMVariantMapMap>();
    qDBusRegisterMetaType<DBUSManagerStruct>();
    qDBusRegisterMetaType<UIntList>();
    registerModemManagerTypes();

    bool serviceFound = manager.isValid();
    if (!serviceFound) {
        // find out whether it will be activated automatically
        QDBusMessage message = QDBusMessage::createMethodCall("org.freedesktop.DBus",
                                                              "/org/freedesktop/DBus",
                                                              "org.freedesktop.DBus",
                                                              "ListActivatableNames");

        QDBusReply<QStringList> reply = QDBusConnection::systemBus().call(message);
        if (reply.isValid() && reply.value().contains(MM_DBUS_SERVICE)) {
            QDBusConnection::systemBus().interface()->startService(MM_DBUS_SERVICE);
            serviceFound = true;
        }
    }

    if (serviceFound) {
        connect(&manager, SIGNAL(InterfacesAdded(QDBusObjectPath,NMVariantMapMap)),
                this, SLOT(onInterfacesAdded(QDBusObjectPath,NMVariantMapMap)));
        connect(&manager, SIGNAL(InterfacesRemoved(QDBusObjectPath,QStringList)),
                this, SLOT(onInterfacesRemoved(QDBusObjectPath,QStringList)));
    }

    connect(&watcher, SIGNAL(serviceRegistered(QString)), SLOT(daemonRegistered()));
    connect(&watcher, SIGNAL(serviceUnregistered(QString)), SLOT(daemonUnregistered()));

    init();
}

ModemManager::ModemManagerPrivate::~ModemManagerPrivate()
{
}

void ModemManager::ModemManagerPrivate::init()
{
    modemList.clear();

    QDBusPendingReply<DBUSManagerStruct> reply = manager.GetManagedObjects();
    reply.waitForFinished();
    if (!reply.isError()) {  // enum devices
        Q_FOREACH(const QDBusObjectPath &path, reply.value().keys()) {
            const QString uni = path.path();
            mmDebug() << "Adding device" << uni;

            if (uni == MM_DBUS_PATH || !uni.startsWith(MM_DBUS_MODEM_PREFIX)) // TODO checkme
                continue;

            modemList.insert(uni, ModemDevice::Ptr());
            emit modemAdded(uni);
        }
    } else { // show error
        qWarning() << "Failed enumerating MM objects:" << reply.error().name() << "\n" << reply.error().message();
    }
}

ModemManager::ModemDevice::Ptr ModemManager::ModemManagerPrivate::findModemDevice(const QString &uni)
{
    ModemDevice::Ptr modem;
    if (modemList.contains(uni)) {
        if (modemList.value(uni)) {
            modem = modemList.value(uni);
        } else {
            modem = ModemDevice::Ptr(new ModemDevice(uni), &QObject::deleteLater);
            modemList[uni] = modem;
        }
    }
    return modem;
}

ModemManager::ModemDevice::List ModemManager::ModemManagerPrivate::modemDevices()
{
    ModemDevice::List list;

    QMap<QString, ModemDevice::Ptr>::const_iterator i;
    for (i = modemList.constBegin(); i != modemList.constEnd(); ++i) {
        ModemDevice::Ptr modem = findModemDevice(i.key());
        if (!modem.isNull()) {
            list.append(modem);
        } else {
            qWarning() << "warning: null modem Interface for" << i.key();
        }
    }

    return list;
}

void ModemManager::ModemManagerPrivate::scanDevices()
{
    iface.ScanDevices();
}

void ModemManager::ModemManagerPrivate::daemonRegistered()
{
    init();
    emit serviceAppeared();
}

void ModemManager::ModemManagerPrivate::daemonUnregistered()
{
    emit serviceDisappeared();
    modemList.clear();
}

void ModemManager::ModemManagerPrivate::onInterfacesAdded(const QDBusObjectPath &object_path, const NMVariantMapMap &interfaces_and_properties)
{
    //TODO control added bearers and sim cards

    const QString uni = object_path.path();

    /* Ignore non-modems */
    if (!uni.startsWith(MM_DBUS_MODEM_PREFIX)) {
        return;
    }

    mmDebug() << uni << "has new interfaces:" << interfaces_and_properties.keys();

    // new device, we don't know it yet
    if (!modemList.contains(uni)) {
        modemList.insert(uni, ModemDevice::Ptr());
        emit modemAdded(uni);
    }
    // re-emit in case of modem type change (GSM <-> CDMA)
    else if (modemList.contains(uni) && (interfaces_and_properties.keys().contains(MM_DBUS_INTERFACE_MODEM_MODEM3GPP) ||
                                         interfaces_and_properties.keys().contains(MM_DBUS_INTERFACE_MODEM_MODEMCDMA))) {
        emit modemAdded(uni);
    }
}

void ModemManager::ModemManagerPrivate::onInterfacesRemoved(const QDBusObjectPath &object_path, const QStringList &interfaces)
{
    //TODO control removed bearers and sim cards

    const QString uni = object_path.path();

    /* Ignore non-modems */
    if (!uni.startsWith(MM_DBUS_MODEM_PREFIX)) {
        return;
    }

    mmDebug() << uni << "lost interfaces:" << interfaces;

    ModemDevice modem(uni);

    if (!uni.isEmpty() && (interfaces.isEmpty() || modem.interfaces().isEmpty())) {
        emit modemRemoved(uni);
        modemList.remove(uni);
    }
}

ModemManager::ModemDevice::Ptr ModemManager::findModemDevice(const QString &uni)
{
    return globalModemManager->findModemDevice(uni);
}

ModemManager::ModemDevice::List ModemManager::modemDevices()
{
    return globalModemManager->modemDevices();
}

ModemManager::Notifier * ModemManager::notifier()
{
    return globalModemManager;
}

void ModemManager::scanDevices()
{
    globalModemManager->scanDevices();
}
