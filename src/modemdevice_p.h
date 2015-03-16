/*
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

#ifndef MODEMMANAGERQT_MODEMDEVICE_P_H
#define MODEMMANAGERQT_MODEMDEVICE_P_H

#include <QObject>
#include <QStringList>
#include <QMap>

#include "modemdevice.h"
#include "interface.h"
#include "bearer.h"
#include "sim.h"

namespace ModemManager
{

class ModemDevicePrivate : public QObject
{
    Q_OBJECT
public:
    ModemDevicePrivate(const QString &path, ModemManager::ModemDevice *q);
    virtual ~ModemDevicePrivate();
    void init();
    void initInterfaces();
    QString introspect() const;
    QString uni;
    QMap<ModemManager::ModemDevice::InterfaceType, ModemManager::Interface::Ptr> interfaceList;
    //QMap<QString, ModemManager::Bearer::Ptr> bearerList;
    ModemManager::Sim::Ptr simCard;
    ModemManager::Interface::List interfaces() ;
    ModemManager::Interface::Ptr interface(ModemManager::ModemDevice::InterfaceType type);
    ModemManager::Interface::Ptr createInterface(ModemManager::ModemDevice::InterfaceType type);
    ModemManager::Bearer::Ptr findBearer(const QString &uni);
    ModemManager::Bearer::List bearers();
    ModemManager::Sim::Ptr sim();

    Q_DECLARE_PUBLIC(ModemManager::ModemDevice)
    ModemManager::ModemDevice *q_ptr;

private Q_SLOTS:
    void onInterfacesAdded(const QDBusObjectPath &object_path, const ModemManager::MMVariantMapMap &interfaces_and_properties);
    void onInterfacesRemoved(const QDBusObjectPath &object_path, const QStringList &interfaces);
    void onSimPathChanged(const QString &oldPath, const QString &newPath);
};

} // namespace ModemManager

#endif
