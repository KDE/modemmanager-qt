/*
    Copyright 2008,2011 Will Stephenson <wstephenson@kde.org>
    Copyright 2010 Lamarque Souza <lamarque@kde.org>
    Copyright 2013 Daniel Nicoletti <dantti12@gmail.com>
    Copyright 2013 Lukas Tinkl <ltinkl@redhat.com>
    Copyright 2013-2015 Jan Grulich <jgrulich@redhat.com>

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

#ifdef MMQT_STATIC
#include "dbus/fakedbus.h"
#else
#include "dbus/dbus.h"
#endif
#include "generictypes.h"
#include "generictypes_p.h"
#include "macros_p.h"
#include "mmdebug_p.h"
#include "modem.h"

Q_GLOBAL_STATIC(ModemManager::ModemManagerPrivate, globalModemManager)

ModemManager::ModemManagerPrivate::ModemManagerPrivate()
#ifdef MMQT_STATIC
    : watcher(QLatin1String(MMQT_DBUS_SERVICE), QDBusConnection::sessionBus(), QDBusServiceWatcher::WatchForRegistration | QDBusServiceWatcher::WatchForUnregistration, this)
    , iface(QLatin1String(MMQT_DBUS_SERVICE), QLatin1String(MMQT_DBUS_PATH), QDBusConnection::sessionBus(), this)
    , manager(QLatin1String(MMQT_DBUS_SERVICE), QLatin1String(MMQT_DBUS_PATH), QDBusConnection::sessionBus(), this)
#else
    : watcher(QLatin1String(MMQT_DBUS_SERVICE), QDBusConnection::systemBus(), QDBusServiceWatcher::WatchForRegistration | QDBusServiceWatcher::WatchForUnregistration, this)
    , iface(QLatin1String(MMQT_DBUS_SERVICE), QLatin1String(MMQT_DBUS_PATH), QDBusConnection::systemBus(), this)
    , manager(QLatin1String(MMQT_DBUS_SERVICE), QLatin1String(MMQT_DBUS_PATH), QDBusConnection::systemBus(), this)
#endif
{
    qDBusRegisterMetaType<QList<QDBusObjectPath> >();
    registerModemManagerTypes();

    bool serviceFound = manager.isValid();
    if (!serviceFound) {
        // find out whether it will be activated automatically
        QDBusMessage message = QDBusMessage::createMethodCall(QStringLiteral("org.freedesktop.DBus"),
                                                              QStringLiteral("/org/freedesktop/DBus"),
                                                              QStringLiteral("org.freedesktop.DBus"),
                                                              QStringLiteral("ListActivatableNames"));
#ifdef MMQT_STATIC
        QDBusReply<QStringList> reply = QDBusConnection::sessionBus().call(message);
        if (reply.isValid() && reply.value().contains(QLatin1String(MMQT_DBUS_SERVICE))) {
            QDBusConnection::sessionBus().interface()->startService(QLatin1String(MMQT_DBUS_SERVICE));
            serviceFound = true;
        }
    }
#else
        QDBusReply<QStringList> reply = QDBusConnection::systemBus().call(message);
        if (reply.isValid() && reply.value().contains(QLatin1String(MMQT_DBUS_SERVICE))) {
            QDBusConnection::systemBus().interface()->startService(QLatin1String(MMQT_DBUS_SERVICE));
            serviceFound = true;
        }
    }
#endif

    if (serviceFound) {
        connect(&manager, &OrgFreedesktopDBusObjectManagerInterface::InterfacesAdded, this, &ModemManagerPrivate::onInterfacesAdded);
        connect(&manager, &OrgFreedesktopDBusObjectManagerInterface::InterfacesRemoved, this, &ModemManagerPrivate::onInterfacesRemoved);
    }

    connect(&watcher, &QDBusServiceWatcher::serviceRegistered, this, &ModemManagerPrivate::daemonRegistered);
    connect(&watcher, &QDBusServiceWatcher::serviceUnregistered, this, &ModemManagerPrivate::daemonUnregistered);

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
        Q_FOREACH (const QDBusObjectPath &path, reply.value().keys()) {
            const QString uni = path.path();
            qCDebug(MMQT) << "Adding device" << uni;

            if (uni == QLatin1String(MMQT_DBUS_PATH) || !uni.startsWith(QLatin1String(MMQT_DBUS_MODEM_PREFIX)))
                continue;

            modemList.insert(uni, ModemDevice::Ptr());
            Q_EMIT modemAdded(uni);
        }
    } else { // show error
        qCWarning(MMQT) << "Failed enumerating MM objects:" << reply.error().name() << "\n" << reply.error().message();
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
            qCWarning(MMQT) << "warning: null modem Interface for" << i.key();
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
    Q_EMIT serviceAppeared();
}

void ModemManager::ModemManagerPrivate::daemonUnregistered()
{
    Q_EMIT serviceDisappeared();
    modemList.clear();
}

void ModemManager::ModemManagerPrivate::onInterfacesAdded(const QDBusObjectPath &object_path, const MMVariantMapMap &interfaces_and_properties)
{
    //TODO control added bearers and sim cards

    const QString uni = object_path.path();

    /* Ignore non-modems */
    if (!uni.startsWith(QLatin1String(MMQT_DBUS_MODEM_PREFIX))) {
        return;
    }

    qCDebug(MMQT) << uni << "has new interfaces:" << interfaces_and_properties.keys();

    // new device, we don't know it yet
    if (!modemList.contains(uni)) {
        modemList.insert(uni, ModemDevice::Ptr());
        Q_EMIT modemAdded(uni);
    }
    // re-Q_EMIT in case of modem type change (GSM <-> CDMA)
    else if (modemList.contains(uni) && (interfaces_and_properties.keys().contains(QLatin1String(MMQT_DBUS_INTERFACE_MODEM_MODEM3GPP)) ||
                                         interfaces_and_properties.keys().contains(QLatin1String(MMQT_DBUS_INTERFACE_MODEM_MODEMCDMA)))) {
        Q_EMIT modemAdded(uni);
    }
}

void ModemManager::ModemManagerPrivate::onInterfacesRemoved(const QDBusObjectPath &object_path, const QStringList &interfaces)
{
    //TODO control removed bearers and sim cards

    const QString uni = object_path.path();

    /* Ignore non-modems */
    if (!uni.startsWith(QLatin1String(MMQT_DBUS_MODEM_PREFIX))) {
        return;
    }

    qCDebug(MMQT) << uni << "lost interfaces:" << interfaces;

    ModemDevice::Ptr modem = findModemDevice(uni);

    // Remove modem when there is no interface or Modem interfaces has been removed
    if (!uni.isEmpty() && (interfaces.isEmpty() || (modem && modem->interfaces().isEmpty()) || interfaces.contains(QLatin1String(MMQT_DBUS_INTERFACE_MODEM)))) {
        Q_EMIT modemRemoved(uni);
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
