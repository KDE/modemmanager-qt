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

#ifndef MODEMMANAGERQT_MODEM_SIMPLE_H
#define MODEMMANAGERQT_MODEM_SIMPLE_H

#include <ModemManager/ModemManager.h>

#include <modemmanagerqt_export.h>

#include "generictypes.h"
#include "interface.h"

namespace ModemManager
{

class ModemSimplePrivate;

/**
 * @brief The ModemSimple class
 *
 * The Simple interface allows controlling and querying the status of Modems.
 */
class MODEMMANAGERQT_EXPORT ModemSimple : public Interface
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(ModemSimple)

public:
    typedef QSharedPointer<ModemSimple> Ptr;
    typedef QList<Ptr> List;

    struct ConnectPropertiesStruct {
        QString pin; ///< SIM-PIN unlock code.
        QString operatorId; ///< ETSI MCC-MNC of a network to force registration with.
        QString apn; ///< For GSM/UMTS and LTE devices the APN to use.
        MMBearerIpFamily ipType; ///< For GSM/UMTS and LTE devices the IP addressing type to use.
        MMBearerAllowedAuth allowedAuth; ///< The authentication method to use, optional in 3GPP.
        QString user; ///< User name (if any) required by the network.
        QString password; ///< Password (if any) required by the network.
        QString number; ///< For POTS devices the number to dial.
        bool allowRoaming; ///< FALSE to allow only connections to home networks.
        MMModemCdmaRmProtocol rmProtocol; ///< For CDMA devices, the protocol of the Rm interface.
    };

    struct ModemStatusStruct {
        MMModemState state; ///< value specifying the overall state of the modem.
        uint signalQuality; ///< Signal quality value, given only when registered.
        MMModemBand currentBands; ///< List of MMModemBand values, given only when registered.
        MMModemAccessTechnology accessTechnology; ///< A MMModemAccessTechnology value, given only when registered.
        MMModem3gppRegistrationState m3gppRegistrationState; ///< A MMModem3gppRegistrationState value specifying the state of the registration, given only when registered in a 3GPP network.
        QString m3gppOperatorCode; ///< Operator MCC-MNC, given only when registered in a 3GPP network.
        QString m3gppOperatorName; ///< Operator name, given only when registered in a 3GPP network.
        MMModemCdmaRegistrationState cdma1xRegistrationState; ///< A MMModemCdmaRegistrationState value specifying the state of the registration, given only when registered in a CDMA1x network.
        MMModemCdmaRegistrationState evdoRegistrationState; ///< A MMModemCdmaRegistrationState value specifying the state of the registration, given only when registered in a EV-DO network.
        uint cdmaSid; ///< The System Identifier of the serving network, if registered in a CDMA1x network and if known.
        uint cdmaNid; ///< The Network Identifier of the serving network, if registered in a CDMA1x network and if known.
    };

    explicit ModemSimple(const QString &path, QObject *parent = 0 );
    ~ModemSimple();

    QString uni() const;

    /**
     * Do everything needed to connect the modem using the given properties.
     *
     * This method will attempt to find a matching packet data bearer and activate it if necessary,
     * returning the bearer's IP details. If no matching bearer is found, a new bearer will be created
     * and activated, but this operation may fail if no resources are available to complete this connection
     * attempt (ie, if a conflicting bearer is already active).
     *
     * This call may make a large number of changes to modem configuration based on properties passed in. For
     * example, given a PIN-locked, disabled GSM/UMTS modem, this call may unlock the SIM PIN, alter the access
     * technology preference, wait for network registration (or force registration to a specific provider), create
     * a new packet data bearer using the given "apn", and connect that bearer.
     *
     * @return On successful connect, returns the object path of the connected packet data bearer used for the connection attempt.
     */
    QDBusPendingReply<QDBusObjectPath> connectModem(ConnectPropertiesStruct properties);

    /**
     * Get the general modem status.
     */
    QDBusPendingReply<QVariantMap> getStatus();

    /**
     * Disconnect an active packet data connection.
     */
    QDBusPendingReply<void> disconnectModem(const QString &bearer);

    /**
     * Convenient method calling disconnectModem with "/" to make ModemManager disconnect all modems
     */
    QDBusPendingReply<void> disconnectAllModems();

    /**
     * Transforms passed QVariantMap to ModemStatusStruct
     */
    ModemStatusStruct mapToModemStatusStruct(const QVariantMap &map);
};

} // namespace ModemManager

#endif
