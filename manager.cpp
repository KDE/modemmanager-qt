/*
Copyright 2008,2011 Will Stephenson <wstephenson@kde.org>
Copyright 2010 Lamarque Souza <lamarque@kde.org>

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 2 of
the License or (at your option) version 3 or any later version
accepted by the membership of KDE e.V. (or its successor approved
by the membership of KDE e.V.), which shall act as a proxy
defined in Section 14 of version 3 of the license.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "manager.h"
#include "modemcdmainterface.h"
#include "modemgsmcardinterface.h"
#include "modemgsmcontactsinterface.h"
#include "modemgsmnetworkinterface.h"
#include "modemgsmsmsinterface.h"
#include "modemgsmhsointerface.h"
#include "modemgsmussdinterface.h"

#include "manager_p.h"
#include "macros.h"

#include "mmdebug.h"

const QString ModemManager::DBUS_SERVICE = QString::fromLatin1("org.freedesktop.ModemManager");
const QString ModemManager::DBUS_DAEMON_PATH = QString::fromLatin1("/org/freedesktop/ModemManager");

MM_GLOBAL_STATIC(ModemManager::ModemManagerPrivate, globalModemManager)

ModemManager::ModemManagerPrivate::ModemManagerPrivate() : watcher(ModemManager::DBUS_SERVICE, QDBusConnection::systemBus(), QDBusServiceWatcher::WatchForUnregistration, this), iface(ModemManager::DBUS_SERVICE, ModemManager::DBUS_DAEMON_PATH, QDBusConnection::systemBus())
{
    qDBusRegisterMetaType<QList<QDBusObjectPath> >();
    registerModemManagerTypes();

    connect( &iface, SIGNAL(DeviceAdded(const QDBusObjectPath &)),
                  this, SLOT(deviceAdded(const QDBusObjectPath &)));
    connect( &iface, SIGNAL(DeviceRemoved(const QDBusObjectPath &)),
                  this, SLOT(deviceRemoved(const QDBusObjectPath &)));

    connect(&watcher, SIGNAL(serviceUnregistered(const QString&)), SLOT(daemonUnregistered()));
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
            modemMap.insert(op.path(), 0);
            mmDebug() << "  " << op.path();
        }
    } else {
        mmDebug() << "Error getting device list: " << deviceList.error().name() << ": " << deviceList.error().message();
    }
}

ModemManager::ModemInterface * ModemManager::ModemManagerPrivate::findModemInterface(const QString &udi, const ModemManager::ModemInterface::GsmInterfaceType ifaceType)
{
    ModemManager::ModemInterface * modem = 0;
    if (modemMap.contains(udi) && modemMap.value(udi) != 0) {
        modem = modemMap.value(udi);
    } else {
        modem = createModemInterface(udi, ifaceType);
        modemMap.insert(udi, modem);
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
                    createdInterface = new ModemManager::ModemGsmCardInterface(udi, this); // these are deleted by the frontend manager
                break;
                case ModemManager::ModemInterface::GsmContacts:
                    createdInterface = new ModemManager::ModemGsmContactsInterface(udi, this); // these are deleted by the frontend manager
                break;
                case ModemManager::ModemInterface::GsmNetwork:
                    createdInterface = new ModemManager::ModemGsmNetworkInterface(udi, this); // these are deleted by the frontend manager
                break;
                case ModemManager::ModemInterface::GsmSms:
                    createdInterface = new ModemManager::ModemGsmSmsInterface(udi, this); // these are deleted by the frontend manager
                break;
                case ModemManager::ModemInterface::GsmHso:
                    createdInterface = new ModemManager::ModemGsmHsoInterface(udi, this); // these are deleted by the frontend manager
                break;
                case ModemManager::ModemInterface::GsmUssd:
                    createdInterface = new ModemManager::ModemGsmUssdInterface(udi, this); // these are deleted by the frontend manager
                break;
                case ModemManager::ModemInterface::NotGsm: // to prevent compilation warning
                break;
            }
            break;
        case ModemManager::ModemInterface::CdmaType:
            createdInterface = new ModemManager::ModemCdmaInterface(udi, this); // these are deleted by the frontend manager
            break;
        default:
            mmDebug() << "libmm-qt: Can't create object of type " << modemType << "for" << udi;
            break;
    }

    return createdInterface;
}

void ModemManager::ModemManagerPrivate::deviceAdded(const QDBusObjectPath & objpath)
{
    mmDebug();
    modemInterfaces.append(objpath.path());
    emit modemAdded(objpath.path());
}

void ModemManager::ModemManagerPrivate::deviceRemoved(const QDBusObjectPath & objpath)
{
    mmDebug();
    modemInterfaces.removeAll(objpath.path());
    emit modemRemoved(objpath.path());
}

void ModemManager::ModemManagerPrivate::daemonRegistered()
{
    init();
    emit serviceAppeared();
}

void ModemManager::ModemManagerPrivate::daemonUnregistered()
{
    foreach(const QString &path, modemInterfaces) {
        modemRemoved(path);
    }
    modemInterfaces.clear();
    emit serviceDisappeared();
}
ModemManager::ModemInterfaceList ModemManager::modemInterfaces()
{
    ModemInterfaceList list;
    foreach (const QString &udi, globalModemManager->modemInterfaces) {
        ModemInterface * modemIface = globalModemManager->findModemInterface(udi, ModemInterface::GsmNetwork);

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

