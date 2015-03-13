/*
    Copyright 2013, 2014 Lukas Tinkl <ltinkl@redhat.com>
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

#ifndef MODEMMANAGERQT_MODEMDEVICE_H
#define MODEMMANAGERQT_MODEMDEVICE_H

#include <ModemManager/ModemManager.h>

#include <modemmanagerqt_export.h>

#include <QObject>
#include <QVariant>
#include <QSharedPointer>
#include <QDBusObjectPath>

#include "generictypes.h"
#include "bearer.h"
#include "interface.h"
#include "modem.h"
#include "modemmessaging.h"
#include "sim.h"

namespace ModemManager
{

class ModemDevicePrivate;

class MODEMMANAGERQT_EXPORT ModemDevice : public QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(ModemDevice)

    Q_PROPERTY(QString uni READ uni)
public:
    typedef QSharedPointer<ModemDevice> Ptr;
    typedef QList<Ptr> List;

    enum InterfaceType {
        ModemInterface,
        GsmInterface,
        GsmUssdInterface,
        CdmaInterface,
        MessagingInterface,
        LocationInterface,
        TimeInterface,
        FirmwareInterface,
        OmaInterface,
        SignalInterface
    };

    explicit ModemDevice(const QString &path, QObject *parent = 0);
    explicit ModemDevice(ModemDevicePrivate &dd, QObject *parent = 0);
    virtual ~ModemDevice();

    QString uni() const;

    bool hasInterface(InterfaceType type) const;
    ModemManager::Interface::List interfaces() const;
    ModemManager::Interface::Ptr interface(InterfaceType type) const;

    ModemManager::ModemMessaging::Ptr messagingInterface();
    ModemManager::Modem::Ptr modemInterface();

    ModemManager::Bearer::Ptr findBearer(const QString &uni);
    ModemManager::Bearer::List bearers() const;
    ModemManager::Sim::Ptr sim() const;

    bool isGsmModem() const;
    bool isCdmaModem() const;

Q_SIGNALS:
    void simAdded(const QString &udi);
    void simRemoved(const QString &udi);

    void bearerAdded(const QString &udi); // TODO emit these signals in the next version
    void bearerRemoved(const QString &udi);

private:
    ModemDevicePrivate *const d_ptr;
};
} // namespace ModemManager

#endif
