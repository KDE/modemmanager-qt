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

#include "objectmanager.h"
#include "dbus/fakedbus.h"

ObjectManager::ObjectManager(QObject* parent)
    : QDBusAbstractAdaptor(parent)
{
    qDBusRegisterMetaType<ModemManager::MMVariantMapMap>();
    qDBusRegisterMetaType<ModemManager::DBUSManagerStruct>();
}

ObjectManager::~ObjectManager()
{
}

void ObjectManager::addInterfaces(const QDBusObjectPath& object_path, const ModemManager::MMVariantMapMap& interfaces_and_properties)
{
    if (m_managedObjects.contains(object_path)) {
        ModemManager::MMVariantMapMap map = m_managedObjects.value(object_path);
        map.unite(interfaces_and_properties);
        m_managedObjects.insert(object_path, map);
    } else {
        m_managedObjects.insert(object_path, interfaces_and_properties);
    }

    Q_EMIT InterfacesAdded(object_path, interfaces_and_properties);
}

void ObjectManager::removeInterfaces(const QDBusObjectPath& object_path, const QStringList& interfaces)
{
    if (interfaces.contains(QLatin1String(MMQT_DBUS_INTERFACE_MODEM))) {
        m_managedObjects.remove(object_path);
    } else {
        ModemManager::MMVariantMapMap map = m_managedObjects.value(object_path);
        Q_FOREACH (const QString &key, interfaces) {
            map.remove(key);
        }
        m_managedObjects.insert(object_path, map);
    }

    Q_EMIT InterfacesRemoved(object_path, interfaces);
}

ModemManager::DBUSManagerStruct ObjectManager::GetManagedObjects()
{
    return m_managedObjects;
}
