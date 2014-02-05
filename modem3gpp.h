/*
    Copyright 2008,2011 Will Stephenson <wstephenson@kde.org>
    Copyright 2010 Lamarque Souza <lamarque@kde.org>
    Copyright 2013 Lukas Tinkl <ltinkl@redhat.com>
    Copyright 2013 Jan Grulich <jgrulich@redhat.com>

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

#ifndef MODEMMANAGERQT_MODEM3GPP_H
#define MODEMMANAGERQT_MODEM3GPP_H

#include "ModemManagerQt-export.h"

#include <QObject>
#include <QSharedPointer>

#include "generic-types.h"
#include "interface.h"

class Modem3gppPrivate;

namespace ModemManager
{
/**
 * @brief The Modem3gpp class
 *
 * This class provides access to specific actions that may be performed in modems with 3GPP capabilities.
 */
class MODEMMANAGERQT_EXPORT Modem3gpp : public Interface
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(Modem3gpp)
    Q_FLAGS(MMModem3gppFacility)

public:
    typedef QSharedPointer<Modem3gpp> Ptr;
    typedef QList<Ptr> List;

    Q_DECLARE_FLAGS(FacilityLocks, MMModem3gppFacility)

    explicit Modem3gpp(const QString &path, QObject *parent = 0);
    ~Modem3gpp();

    /**
     * @return the IMEI of the device
     */
    QString imei() const;

    /**
     * @return mobile registration status as defined in 3GPP TS 27.007 section 10.1.19
     */
    MMModem3gppRegistrationState registrationState() const;

    /**
     * @return code of the operator to which the mobile is currently registered.
     *
     * Returned in the format "MCCMNC", where MCC is the three-digit ITU E.212
     * Mobile Country Code and MNC is the two- or three-digit GSM Mobile Network
     * Code. e.g. e"31026" or "310260".
     *
     * If the MCC and MNC are not known or the mobile is not registered to a
     * mobile network, this property will be an empty string.
     */
    QString operatorCode() const;

    /**
     * @return name of the operator to which the mobile is currently registered.
     *
     * If the operator name is not known or the mobile is not registered to a
     * mobile network, this property will be an empty string.
     */
    QString operatorName() const;

    /**
     * @return QFlags of MMModem3gppFacility values for which PIN locking is enabled
     */
    FacilityLocks enabledFacilityLocks() const;

    /**
      * Register the device to network.
      *
      * @param networkId The operator ID (ie, "MCCMNC", like "310260") to register. An empty string can be used to register to the home network.
    */
    void registerToNetwork(const QString &networkId = QString());

    /**
     * Scan for available networks.
     *
     * @return a QList<QVariantMap> with the results, where each map may contain these values:
     *
     * "status": A MMModem3gppNetworkAvailability value representing network
     * availability status, given as an unsigned integer (signature "u"). This
     * key will always be present.
     *
     * "operator-long": Long-format name of operator, given as a string value
     * (signature "s"). If the name is unknown, this field should not be present.
     *
     * "operator-short": Short-format name of operator, given as a string value
     * (signature "s"). If the name is unknown, this field should not be present.
     *
     * "operator-code": Mobile code of the operator, given as a string value
     * (signature "s"). Returned in the format "MCCMNC", where MCC is the
     * three-digit ITU E.212 Mobile Country Code and MNC is the two- or
     * three-digit GSM Mobile Network Code. e.g. "31026" or "310260".
     *
     * "access-technology": A MMModemAccessTechnology value representing the
     * generic access technology used by this mobile network, given as an
     * unsigned integer (signature "u").
     */
    QDBusPendingReply<QVariantMapList> scan();

Q_SIGNALS:
    void registrationStateChanged(MMModem3gppRegistrationState registrationState);
    void enabledFacilityLocksChanged(FacilityLocks locks);

private Q_SLOTS:
    void onPropertiesChanged(const QString &interface, const QVariantMap &properties, const QStringList &invalidatedProps);
};

Q_DECLARE_OPERATORS_FOR_FLAGS(Modem3gpp::FacilityLocks)

} // namespace ModemManager

#endif
