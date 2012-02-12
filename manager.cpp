/*
Copyright 2008,2011 Will Stephenson <wstephenson@kde.org>
Copyright 2010 Lamarque Souza <lamarque@kde.org>

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

ModemManager::ModemManagerPrivate::ModemManagerPrivate() : watcher(ModemManager::DBUS_SERVICE, QDBusConnection::systemBus(), QDBusServiceWatcher::WatchForUnregistration, this), iface(ModemManager::DBUS_SERVICE, ModemManager::DBUS_DAEMON_PATH, QDBusConnection::systemBus())
{
    qDBusRegisterMetaType<QList<QDBusObjectPath> >();
    registerModemManagerTypes();

    connect(&iface, SIGNAL(DeviceAdded(QDBusObjectPath)),
                  this, SLOT(deviceAdded(QDBusObjectPath)));
    connect(&iface, SIGNAL(DeviceRemoved(QDBusObjectPath)),
                  this, SLOT(deviceRemoved(QDBusObjectPath)));

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

ModemManager::ModemInterface * ModemManager::ModemManagerPrivate::findModemInterface(const QString &udi, const ModemManager::ModemInterface::GsmInterfaceType ifaceType)
{
    ModemManager::ModemInterface * modem = 0;
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
            } else {
                return 0;
            }
        }
    }
    return modem;
}

ModemManager::ModemInterface * ModemManager::ModemManagerPrivate::createModemInterface(const QString &udi, const ModemManager::ModemInterface::GsmInterfaceType ifaceType)
{
    mmDebug();
    OrgFreedesktopModemManagerModemInterface modemIface(ModemManager::DBUS_SERVICE, udi, QDBusConnection::systemBus());
    uint modemType = modemIface.type();
    ModemManager::ModemInterface * createdInterface = 0;
    switch ( modemType ) {
        case ModemManager::ModemInterface::GsmType:
            switch (ifaceType) {
                case ModemManager::ModemInterface::GsmCard:
                    createdInterface = new ModemManager::ModemGsmCardInterface(udi, this);
                break;
                case ModemManager::ModemInterface::GsmContacts:
                    createdInterface = new ModemManager::ModemGsmContactsInterface(udi, this);
                break;
                case ModemManager::ModemInterface::GsmNetwork:
                    createdInterface = new ModemManager::ModemGsmNetworkInterface(udi, this);
                break;
                case ModemManager::ModemInterface::GsmSms:
                    createdInterface = new ModemManager::ModemGsmSmsInterface(udi, this);
                break;
                case ModemManager::ModemInterface::GsmHso:
                    createdInterface = new ModemManager::ModemGsmHsoInterface(udi, this);
                break;
                case ModemManager::ModemInterface::GsmUssd:
                    createdInterface = new ModemManager::ModemGsmUssdInterface(udi, this);
                break;
                case ModemManager::ModemInterface::NotGsm: // to prevent compilation warning
                break;
            }
            break;
        case ModemManager::ModemInterface::CdmaType:
            createdInterface = new ModemManager::ModemCdmaInterface(udi, this);
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
    ModemInterfaceIfaceMap map = modemMap.take(objpath.path());
    emit modemRemoved(objpath.path());
    foreach (ModemManager::ModemInterface * modem, map) {
        modem->deleteLater();
    }
    map.clear();
}

void ModemManager::ModemManagerPrivate::daemonRegistered()
{
    init();
    emit serviceAppeared();
}

void ModemManager::ModemManagerPrivate::daemonUnregistered()
{
    QMap<QString, ModemInterfaceIfaceMap>::iterator i;
    for (i = modemMap.begin(); i != modemMap.end(); ++i) {
        emit modemRemoved(i.key());
        foreach (ModemManager::ModemInterface * modem, i.value()) {
            modem->deleteLater();
        }
	i.value().clear();
    }
    modemMap.clear();
    emit serviceDisappeared();
}

ModemManager::ModemInterfaceList ModemManager::modemInterfaces()
{
    ModemInterfaceList list;
    QMap<QString, ModemInterfaceIfaceMap>::const_iterator i;
    for (i = globalModemManager->modemMap.constBegin(); i != globalModemManager->modemMap.constEnd(); ++i) {
        ModemInterface * modemIface = globalModemManager->findModemInterface(i.key(), ModemInterface::GsmNetwork);

        if (modemIface) {
            list.append(modemIface);
        }
    }

    return list;
}

ModemManager::ModemInterface * ModemManager::findModemInterface(const QString &udi, const ModemManager::ModemInterface::GsmInterfaceType ifaceType)
{
    return globalModemManager->findModemInterface(udi, ifaceType);
}

ModemManager::Notifier * ModemManager::notifier()
{
    return globalModemManager;
}

