/*
    Copyright 2015 Jan Grulich <jgrulich@redhat.com>

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

#ifndef MODEMMANAGERQT_FAKE_MODEM_OBJECT_MANAGER_H
#define MODEMMANAGERQT_FAKE_MODEM_OBJECT_MANAGER_H

#include "generictypes.h"
#include "generictypes_p.h"

#include <QObject>

#include <QDBusObjectPath>

class ObjectManager : public QDBusAbstractAdaptor
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "org.freedesktop.DBus.ObjectManager")
public:
    explicit ObjectManager(QObject * parent = 0);
    virtual ~ObjectManager();

    void addInterfaces(const QDBusObjectPath &object_path, const ModemManager::MMVariantMapMap &interfaces_and_properties);
    void removeInterfaces(const QDBusObjectPath &object_path, const QStringList &interfaces);
public Q_SLOTS:
    Q_SCRIPTABLE ModemManager::DBUSManagerStruct GetManagedObjects();

Q_SIGNALS:
    Q_SCRIPTABLE void InterfacesAdded(const QDBusObjectPath &object_path, const ModemManager::MMVariantMapMap &interfaces_and_properties);
    Q_SCRIPTABLE void InterfacesRemoved(const QDBusObjectPath &object_path, const QStringList &interfaces);

private:
    QMap<QDBusObjectPath, ModemManager::MMVariantMapMap> m_managedObjects;
};

#endif
