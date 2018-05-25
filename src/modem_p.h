/*
    Copyright 2008 Will Stephenson <wstephenson@kde.org>
    Copyright 2010 Lamarque Souza <lamarque@kde.org>
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

#ifndef MODEMMANAGERQT_MODEM_P_H
#define MODEMMANAGERQT_MODEM_P_H

#include "dbus/modeminterface.h"

#include "interface_p.h"
#include "bearer.h"
#include "modem.h"

namespace ModemManager
{

class ModemPrivate: public InterfacePrivate
{
    Q_OBJECT
public:
    explicit ModemPrivate(const QString &path, Modem *q);
    OrgFreedesktopModemManager1ModemInterface modemIface;

    QMap<QString, Bearer::Ptr> bearers;
    QString simPath;
    QList<MMModemCapability> supportedCapabilities;
    QFlags<MMModemCapability> currentCapabilities;
    uint maxBearers;
    uint maxActiveBearers;
    QString manufacturer;
    QString model;
    QString revision;
    QString deviceIdentifier;
    QString device;
    QStringList drivers;
    QString plugin;
    QString primaryPort;
    PortList ports;
    QString equipmentIdentifier;
    MMModemLock unlockRequired;
    UnlockRetriesMap unlockRetries;
    MMModemState state;
    MMModemStateFailedReason stateFailedReason;
    ModemManager::Modem::AccessTechnologies accessTechnologies;
    SignalQualityPair signalQuality;
    QStringList ownNumbers;
    MMModemPowerState powerState;
    SupportedModesType supportedModes;
    CurrentModesType currentModes;
    QList<MMModemBand> supportedBands;
    QList<MMModemBand> currentBands;
    ModemManager::Modem::IpBearerFamilies supportedIpFamilies;

    ModemManager::Bearer::Ptr findRegisteredBearer(const QString &path);

    Q_DECLARE_PUBLIC(Modem)
    Modem *q_ptr;
private Q_SLOTS:
    void initializeBearers();
    void onPropertiesChanged(const QString &interface, const QVariantMap &properties, const QStringList &invalidatedProps) override;
    void onStateChanged(int oldState, int newState, uint reason);
};

} // namespace ModemManager

#endif
