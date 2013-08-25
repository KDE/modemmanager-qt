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

#ifndef MODEMMANAGERQT_MANAGER_P_H
#define MODEMMANAGERQT_MANAGER_P_H

#include <QObject>
#include <QDBusServiceWatcher>

#include "dbus/mm-modeminterface.h"
#include "dbus/mm-manager-clientinterface.h"
#include "dbus/mm-modem-gsm-networkinterface.h"

namespace ModemManager
{
typedef QMap<ModemInterface::GsmInterfaceType, ModemInterface::Ptr> ModemInterfaceIfaceMap;

class ModemInterface;
class ModemManagerPrivate : public Notifier
{
Q_OBJECT

public:
    ModemManagerPrivate();
    ~ModemManagerPrivate();
    QDBusServiceWatcher watcher;
    OrgFreedesktopModemManagerInterface iface;
    QMap<QString, ModemInterfaceIfaceMap> modemMap;
    ModemManager::ModemInterface::Ptr findModemInterface(const QString &udi, const ModemManager::ModemInterface::GsmInterfaceType ifaceType);
    ModemManager::ModemInterface::Ptr createModemInterface(const QString &udi, const ModemManager::ModemInterface::GsmInterfaceType ifaceType);
protected Q_SLOTS:
    void init();
    void deviceAdded(const QDBusObjectPath & device);
    void deviceRemoved(const QDBusObjectPath & device);
    void daemonRegistered();
    void daemonUnregistered();
};
} // namespace ModemManager


#endif

