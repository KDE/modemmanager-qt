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
#include "modemcdmainterface.h"
#include "modemgsmcardinterface.h"
#include "modemgsmnetworkinterface.h"
#include "modemgsmsmsinterface.h"
#include "modemgsmussdinterface.h"
#include "modemmessaginginterface.h"
#include "modemtimeinterface.h"
#include "modemlocationinterface.h"
#include "macros.h"
#include "mmdebug.h"
#include "../dbus/generic-types.h"
#include "../dbus/dbus_manager.h"

MM_GLOBAL_STATIC(ModemManager::ModemManagerPrivate, globalModemManager)

ModemManager::ModemManagerPrivate::ModemManagerPrivate() :
    watcher(MM_DBUS_SERVICE, QDBusConnection::systemBus(), QDBusServiceWatcher::WatchForRegistration | QDBusServiceWatcher::WatchForUnregistration, this),
    iface(MM_DBUS_SERVICE, MM_DBUS_PATH, QDBusConnection::systemBus(), this),
    manager(MM_DBUS_SERVICE, MM_DBUS_PATH, QDBusConnection::systemBus(), this)
{
    qDBusRegisterMetaType<QList<QDBusObjectPath> >();
    qDBusRegisterMetaType<QVariantMapMap>();
    qDBusRegisterMetaType<DBUSManagerStruct>();
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
        connect(&manager, SIGNAL(InterfacesAdded(QDBusObjectPath,QVariantMapMap)),
                this, SLOT(onInterfacesAdded(QDBusObjectPath,QVariantMapMap)));
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
    devices.clear();

    QDBusPendingReply<DBUSManagerStruct> reply = manager.GetManagedObjects();
    reply.waitForFinished();
    if (!reply.isError()) {  // enum devices
        Q_FOREACH(const QDBusObjectPath &path, reply.value().keys()) {
            const QString udi = path.path();
            mmDebug() << "Adding device" << udi;

            if (udi == MM_DBUS_PATH || !udi.startsWith(MM_DBUS_MODEM_PREFIX)) // TODO checkme
                continue;

            devices.append(udi);
        }
    } else { // show error
        qWarning() << "Failed enumerating MM objects:" << reply.error().name() << "\n" << reply.error().message();
    }
}

ModemManager::ModemInterface::Ptr ModemManager::ModemManagerPrivate::findModemInterface(const QString &udi, ModemInterface::InterfaceType ifaceType)
{
    mmDebug();

    ModemInterface modem(udi, this);
    ModemInterface::Ptr createdInterface;

    if (!modem.isValid())
        return createdInterface;

    switch (ifaceType) {
    case ModemInterface::Gsm:
        if (modem.hasInterface(MM_DBUS_INTERFACE_MODEM_MODEM3GPP))
            createdInterface = ModemInterface::Ptr(new Modem3gppInterface(udi, this));
        break;
    case ModemInterface::Messaging:
        if (modem.hasInterface(MM_DBUS_INTERFACE_MODEM_MESSAGING))
            createdInterface = ModemInterface::Ptr(new ModemMessagingInterface(udi, this));
        break;
    case ModemInterface::GsmUssd:
        if (modem.hasInterface(MM_DBUS_INTERFACE_MODEM_MODEM3GPP_USSD))
            createdInterface = ModemInterface::Ptr(new Modem3gppUssdInterface(udi, this));
        break;
    case ModemInterface::Cdma:
        if (modem.hasInterface(MM_DBUS_INTERFACE_MODEM_MODEMCDMA))
            createdInterface = ModemInterface::Ptr(new ModemCdmaInterface(udi, this));
        break;
    case ModemInterface::Location:
        if (modem.hasInterface(MM_DBUS_INTERFACE_MODEM_LOCATION))
            createdInterface = ModemInterface::Ptr(new ModemLocationInterface(udi, this));
        break;
    case ModemInterface::Time:
        if (modem.hasInterface(MM_DBUS_INTERFACE_MODEM_TIME))
            createdInterface = ModemInterface::Ptr(new ModemTimeInterface(udi, this));
        break;
    default:
        mmDebug() << "libQtModemManager: Can't create object of type " << ifaceType << "for" << udi;
        break;
    }

    return createdInterface;
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
    devices.clear();
}

void ModemManager::ModemManagerPrivate::onInterfacesAdded(const QDBusObjectPath &object_path, const QVariantMapMap &interfaces_and_properties)
{
    const QString udi = object_path.path();

    /* Ignore non-modems */
    if (!udi.startsWith(MM_DBUS_MODEM_PREFIX)) {
        return;
    }

    mmDebug() << udi << "has new interfaces:" << interfaces_and_properties.keys();

    // new device, we don't know it yet
    if (!devices.contains(udi)) {
        devices.append(udi);
        emit modemAdded(udi);
    }
    // re-emit in case of modem type change (GSM <-> CDMA)
    else if (devices.contains(udi) && (interfaces_and_properties.keys().contains(MM_DBUS_INTERFACE_MODEM_MODEM3GPP) ||
                                       interfaces_and_properties.keys().contains(MM_DBUS_INTERFACE_MODEM_MODEMCDMA))) {
        emit modemAdded(udi);
    }
}

void ModemManager::ModemManagerPrivate::onInterfacesRemoved(const QDBusObjectPath &object_path, const QStringList &interfaces)
{
    const QString udi = object_path.path();

    /* Ignore non-modems */
    if (!udi.startsWith(MM_DBUS_MODEM_PREFIX)) {
        return;
    }

    mmDebug() << udi << "lost interfaces:" << interfaces;

    ModemInterface modem(udi);

    if (!udi.isEmpty() && (interfaces.isEmpty() || modem.interfaces().isEmpty())) {
        emit modemRemoved(udi);
        devices.removeAll(udi);
    }
}

ModemManager::ModemInterface::List ModemManager::modemInterfaces()
{
    ModemInterface::List list;

    foreach (const QString & device, globalModemManager->devices) {
        ModemInterface modem(device);
        ModemInterface::Ptr modemIface;
        if (modem.isGsmModem())
            modemIface = globalModemManager->findModemInterface(device, ModemInterface::Gsm);
        else if (modem.isCdmaModem())
            modemIface = globalModemManager->findModemInterface(device, ModemInterface::Cdma);

        if (modemIface) {
            list.append(modemIface);
        }
    }

    return list;
}

ModemManager::ModemInterface::Ptr ModemManager::findModemInterface(const QString &udi, ModemInterface::InterfaceType ifaceType)
{
    return globalModemManager->findModemInterface(udi, ifaceType);
}

ModemManager::Notifier * ModemManager::notifier()
{
    return globalModemManager;
}

void ModemManager::scanDevices()
{
    globalModemManager->scanDevices();
}
