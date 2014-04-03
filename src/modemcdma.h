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

#ifndef MODEMMANAGERQT_MODEMCDMA_H
#define MODEMMANAGERQT_MODEMCDMA_H

#include "ModemManagerQt-export.h"

#include <QObject>
#include <QSharedPointer>
#include <QVariant>

#include "generictypes.h"
#include "interface.h"

class ModemCdmaPrivate;

namespace ModemManager {
/**
 * @brief The ModemCdma class
 *
 * This class provides access to specific actions that may be performed in modems with CDMA capabilities.
 */
class MODEMMANAGERQT_EXPORT ModemCdma : public Interface
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(ModemCdma)
public:
    typedef QSharedPointer<ModemCdma> Ptr;
    typedef QList<Ptr> List;

    explicit ModemCdma(const QString &path, QObject *parent = 0);
    ~ModemCdma();

    /**
     * Provisions the modem for use with a given carrier using the modem's
     * Over-The-Air (OTA) activation functionality, if any.
     *
     * Some modems will reboot after this call is made.
     *
     * @param carrierCode name of carrier, or carrier-specific code
     */
    void activate(const QString &carrierCode);

    /**
     * Sets the modem provisioning data directly, without contacting the carrier over the air.
     *
     * Some modems will reboot after this call is made.
     *
     * @param properties QVariantMap consisting of:
     *
     * "spc": The Service Programming Code, given as a string of exactly 6 digit characters. Mandatory parameter.
     * "sid": The System Identification Number, given as a 16-bit unsigned integer (signature "q"). Mandatory parameter.
     * "mdn": The Mobile Directory Number, given as a string of maximum 15 characters. Mandatory parameter.
     * "min": The Mobile Identification Number, given as a string of maximum 15 characters. Mandatory parameter.
     * "mn-ha-key": The MN-HA key, given as a string of maximum 16 characters.
     * "mn-aaa-key": The MN-AAA key, given as a string of maximum 16 characters.
     * "prl": The Preferred Roaming List, given as an array of maximum 16384 bytes.
     */
    void activateManual(const QVariantMap &properties);

    /**
     * @return a MMModemCdmaActivationState value specifying the state of the activation in the 3GPP2 network.
     */
    MMModemCdmaActivationState activationState() const;

    /**
     * @return the modem's Mobile Equipment Identifier.
     */
    QString meid() const;

    /**
     * @return the modem's Electronic Serial Number (superceded by MEID but still used by older devices).
     */
    QString esn() const;

    /**
     * @return the System Identifier of the serving CDMA 1x network, if known, and if the modem is registered with a CDMA 1x network.
     * @see http://ifast.org or the mobile broadband provider database for mappings of SIDs to network providers.
     */
    uint sid() const;

    /**
     * @return the Network Identifier of the serving CDMA 1x network, if known, and if the modem is registered with a CDMA 1x network.
     */
    uint nid() const;

    /**
     * @return a MMModemCdmaRegistrationState value specifying the CDMA 1x registration state.
     */
    MMModemCdmaRegistrationState cdma1xRegistrationState() const;

    /**
     * @return a MMModemCdmaRegistrationState value specifying the EVDO registration state.
     */
    MMModemCdmaRegistrationState evdoRegistrationState() const;

private Q_SLOTS:
    void onActivationStateChanged(uint activation_state, uint activation_error, const QVariantMap &status_changes);

Q_SIGNALS:
    /**
     * This signal is emitted when the activation info this network changes
     *
     * @param state current activation state, given as a MMModemCdmaActivationState.
     * @param error carrier-specific error code, given as a MMCdmaActivationError.
     * @param status_changes properties that have changed as a result of this activation state chage, including "mdn" and "min".
     *                       The map may be empty if the changed properties are unknown.
     */
    void activationStateChanged(MMModemCdmaActivationState state, MMCdmaActivationError error, const QVariantMap &status_changes);
};
} // namespace ModemManager

#endif
