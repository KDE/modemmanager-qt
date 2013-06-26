/*
Copyright 2008,2011 Will Stephenson <wstephenson@kde.org>
Copyright 2010 Lamarque Souza <lamarque@kde.org>
Copyright 2013 Daniel Nicoletti <dantti12@gmail.com>

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
#include "modemgsmcontactsinterface.h"
#include "modemgsmnetworkinterface.h"
#include "modemgsmsmsinterface.h"
#include "modemgsmhsointerface.h"
#include "modemgsmussdinterface.h"
#include "macros.h"
#include "mmdebug.h"

const QString ModemManager::DBUS_SERVICE = QString::fromLatin1("org.freedesktop.ModemManager");
const QString ModemManager::DBUS_DAEMON_PATH = QString::fromLatin1("/org/freedesktop/ModemManager");

MM_GLOBAL_STATIC(ModemManager::ModemManagerPrivate, globalModemManager)

ModemManager::ModemManagerPrivate::ModemManagerPrivate() :
    watcher(ModemManager::DBUS_SERVICE, QDBusConnection::systemBus(), QDBusServiceWatcher::WatchForRegistration | QDBusServiceWatcher::WatchForUnregistration, this),
    iface(ModemManager::DBUS_SERVICE, ModemManager::DBUS_DAEMON_PATH, QDBusConnection::systemBus())
{
    qDBusRegisterMetaType<QList<QDBusObjectPath> >();
    registerModemManagerTypes();

    connect(&iface, SIGNAL(DeviceAdded(QDBusObjectPath)),
            this, SLOT(deviceAdded(QDBusObjectPath)));
    connect(&iface, SIGNAL(DeviceRemoved(QDBusObjectPath)),
            this, SLOT(deviceRemoved(QDBusObjectPath)));

    connect(&watcher, SIGNAL(serviceRegistered(QString)), SLOT(daemonRegistered()));
    connect(&watcher, SIGNAL(serviceUnregistered(QString)), SLOT(daemonUnregistered()));
    init();
}

ModemManager::ModemManagerPrivate::~ModemManagerPrivate()
{
}

void ModemManager::ModemManagerPrivate::init()
{
    QDBusReply< QList<QDBusObjectPath> > deviceList = iface.EnumerateDevices();
    if (deviceList.isValid())
    {
        QList <QDBusObjectPath> devices = deviceList.value();
        foreach (const QDBusObjectPath &op, devices)
        {
            modemMap.insert(op.path(), ModemInterfaceIfaceMap());
            mmDebug() << "  " << op.path();
        }
    } else {
        mmDebug() << "Error getting device list: " << deviceList.error().name() << ": " << deviceList.error().message();
    }
}

ModemManager::ModemInterface::Ptr ModemManager::ModemManagerPrivate::findModemInterface(const QString &udi, const ModemManager::ModemInterface::GsmInterfaceType ifaceType)
{
    ModemManager::ModemInterface::Ptr modem;
    QMap<QString, ModemInterfaceIfaceMap>::ConstIterator mapIt = modemMap.constFind(udi);
    if (mapIt != modemMap.constEnd()) {
        ModemInterfaceIfaceMap map = modemMap.value(udi);
        if (map.contains(ifaceType)) {
            modem = map.value(ifaceType);
        } else {
            modem = createModemInterface(udi, ifaceType);
            if (modem) {
                map.insert(ifaceType, modem);
                modemMap.insert(udi, map);
            }
        }
    }
    return modem;
}

ModemManager::ModemInterface::Ptr ModemManager::ModemManagerPrivate::createModemInterface(const QString &udi, const ModemManager::ModemInterface::GsmInterfaceType ifaceType)
{
    mmDebug();
    OrgFreedesktopModemManagerModemInterface modemIface(ModemManager::DBUS_SERVICE, udi, QDBusConnection::systemBus());
    uint modemType = modemIface.type();
    ModemInterface::Ptr createdInterface;
    switch ( modemType ) {
    case ModemInterface::GsmType:
        switch (ifaceType) {
        case ModemInterface::GsmCard:
            createdInterface = ModemInterface::Ptr(new ModemGsmCardInterface(udi, this));
            break;
        case ModemInterface::GsmContacts:
            createdInterface = ModemInterface::Ptr(new ModemGsmContactsInterface(udi, this));
            break;
        case ModemInterface::GsmNetwork:
            createdInterface = ModemInterface::Ptr(new ModemGsmNetworkInterface(udi, this));
            break;
        case ModemInterface::GsmSms:
            createdInterface = ModemInterface::Ptr(new ModemGsmSmsInterface(udi, this));
            break;
        case ModemInterface::GsmHso:
            createdInterface = ModemInterface::Ptr(new ModemGsmHsoInterface(udi, this));
            break;
        case ModemInterface::GsmUssd:
            createdInterface = ModemInterface::Ptr(new ModemGsmUssdInterface(udi, this));
            break;
        case ModemInterface::NotGsm: // to prevent compilation warning
            break;
        }
        break;
    case ModemInterface::CdmaType:
        createdInterface = ModemInterface::Ptr(new ModemCdmaInterface(udi, this));
        break;
    default:
        mmDebug() << "libQtModemManager: Can't create object of type " << modemType << "for" << udi;
        break;
    }

    return createdInterface;
}

void ModemManager::ModemManagerPrivate::deviceAdded(const QDBusObjectPath & objpath)
{
    mmDebug();
    modemMap.insert(objpath.path(), ModemInterfaceIfaceMap());
    emit modemAdded(objpath.path());
}

void ModemManager::ModemManagerPrivate::deviceRemoved(const QDBusObjectPath & objpath)
{
    mmDebug();
    emit modemRemoved(objpath.path());
    modemMap.remove(objpath.path());
}

void ModemManager::ModemManagerPrivate::daemonRegistered()
{
    init();
    emit serviceAppeared();
}

void ModemManager::ModemManagerPrivate::daemonUnregistered()
{
    emit serviceDisappeared();
    modemMap.clear();
}

ModemManager::ModemInterface::List ModemManager::modemInterfaces()
{
    ModemInterface::List list;
    QMap<QString, ModemInterfaceIfaceMap>::const_iterator i = globalModemManager->modemMap.constBegin();
    while (i != globalModemManager->modemMap.constEnd()) {
        ModemInterface::Ptr modemIface = globalModemManager->findModemInterface(i.key(), ModemInterface::GsmNetwork);
        if (modemIface) {
            list << modemIface;
        }
        ++i;
    }

    return list;
}

ModemManager::ModemInterface::Ptr ModemManager::findModemInterface(const QString &udi, const ModemManager::ModemInterface::GsmInterfaceType ifaceType)
{
    return globalModemManager->findModemInterface(udi, ifaceType);
}

ModemManager::Notifier * ModemManager::notifier()
{
    return globalModemManager;
}

