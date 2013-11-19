/*
    Copyright 2008,2011 Will Stephenson <wstephenson@kde.org>
    Copyright 2010 Lamarque Souza <lamarque@kde.org>
    Copyright 2013 Daniel Nicoletti <dantti12@gmail.com>
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

#ifndef MODEMMANAGERQT_MODEM_H
#define MODEMMANAGERQT_MODEM_H

#include <ModemManager/ModemManager.h>

#include "ModemManagerQt-export.h"

#include <QObject>
#include <QVariant>
#include <QSharedPointer>
#include <QDBusObjectPath>

#include "generic-types.h"
#include "interface.h"

class ModemPrivate;

namespace ModemManager
{
/**
 * @brief The Modem class
 *
 * The Modem interface controls the status and actions in a given modem object.
 */
class MODEMMANAGERQT_EXPORT Modem : public Interface
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(Modem)

    Q_FLAGS(MMModemCapability)
    Q_FLAGS(MMModemAccessTechnology)
    Q_FLAGS(MMModemMode)
    Q_FLAGS(MMBearerIpFamily)

public:
    Q_DECLARE_FLAGS(Capabilities, MMModemCapability)
    Q_DECLARE_FLAGS(AccessTechnologies, MMModemAccessTechnology)
    Q_DECLARE_FLAGS(ModemModes, MMModemMode)
    Q_DECLARE_FLAGS(IpBearerFamilies, MMBearerIpFamily)

    typedef QSharedPointer<Modem> Ptr;
    typedef QList<Ptr> List;

    struct BearerStruct {
        QString apn; ///< Access Point Name, given as a string value. Required in 3GPP.
        MMBearerIpFamily ipType; ///< Addressing type, given as a MMBearerIpFamily value. Optional in 3GPP and CDMA.
        MMBearerAllowedAuth allowedAuth; ///< The authentication method to use, given as a MMBearerAllowedAuth value. Optional in 3GPP.
        QString user; ///< User name (if any) required by the network, given as a string value. Optional in 3GPP.
        QString password; ///< Password (if any) required by the network, given as a string value. Optional in 3GPP.
        bool allowRoaming; ///< Flag to tell whether connection is allowed during roaming, given as a boolean value. Optional in 3GPP.
        MMModemCdmaRmProtocol rmProtocol; ///< Protocol of the Rm interface, given as a MMModemCdmaRmProtocol value. Optional in CDMA.
        QString number; ///< Telephone number to dial, given as a string value. Required in POTS.
    };

    explicit Modem(const QString &path, QObject *parent = 0 );
    ~Modem();

    QString uni() const;
    /**
     * @return @p true if the modem is fully functional, @p false when in low power mode or disabled
     * @see setEnabled()
     */
    bool isEnabled() const;
    bool isValid() const;

    /**
     * Enable or disable the modem.
     *
     * When enabled, the modem's radio is powered on and data sessions, voice calls, location services, and Short Message Service may be available.
     *
     * When disabled, the modem enters low-power state and no network-related operations are available.
     */
    void setEnabled(bool enable);

    /**
     * Create a new packet data bearer using the given characteristics.
     *
     * This request may fail if the modem does not support additional bearers, if too many bearers are already defined, or if properties are invalid.
     *
     * @return the path to the new bearer
     */
    QString createBearer(const BearerStruct &bearer);

    /**
     * Delete an existing packet data bearer.
     *
     * If the bearer is currently active and providing packet data server, it will be disconnected and that packet data service will terminate.
     * @param bearer path to the bearer to delete
     */
    void deleteBearer(const QString &bearer);

    /**
     * @return the configured packet data bearers (EPS Bearers, PDP Contexts, or CDMA2000 Packet Data Sessions).
     */
    QStringList listBearers();

    /**
     * Clear non-persistent configuration and state, and return the device to a newly-powered-on state.
     *
     * This command may power-cycle the device.
     */
    void reset();

    /**
     * Clear the modem's configuration (including persistent configuration and state), and return the device to a factory-default state.
     *
     * If not required by the modem, @p code may be ignored.
     *
     * This command may or may not power-cycle the device.
     * @param code Carrier-supplied code required to reset the modem.
     */
    void factoryReset(const QString &code);

    /**
     * Set the power @p state of the modem. This action can only be run when the modem is in MM_MODEM_STATE_DISABLED state.
     */
    void setPowerState(MMModemPowerState state);

    /**
     * Set the capabilities of the device. A restart of the modem may be required.
     * @param caps QFlags of MMModemCapability values, to specify the capabilities to use.
     */
    void setCurrentCapabilities(Capabilities caps);

    /**
     * Set the access technologies (e.g. 2G/3G/4G preference) the device is currently allowed to use when connecting to a network.
     *
     * The given combination should be supported by the modem, as specified in supportedModes()
     * @param mode
     */
    void setCurrentModes(const CurrentModesType &mode);

    /**
     * Set the radio frequency and technology bands the device is currently allowed to use when connecting to a network.
     * @param bands List of MMModemBand values, to specify the bands to be used.
     */
    void setCurrentBands(const QList<MMModemBand> &bands);

    QString command(const QString &cmd, uint timeout);

    /**
     * @return The path of the SIM object available in this device, if any.
     */
    QString simPath() const;

    /**
     * @return List of MMModemCapability values, specifying the combinations of generic family of access technologies the modem supports.
     *
     * If the modem doesn't allow changing the current capabilities, a single entry with MM_MODEM_CAPABILITY_ANY will be given.
     */
    QList<MMModemCapability> supportedCapabilities() const;

    /**
     * @return QFlags of MMModemCapability values, specifying the generic family of
     * access technologies the modem currently supports without a firmware
     * reload or reinitialization.
     */
    Capabilities currentCapabilities() const;

    /**
     * @return The maximum number of defined packet data bearers the modem supports.
     *
     * This is not the number of active/connected bearers the modem supports,
     * but simply the number of bearers that may be defined at any given time.
     * For example, POTS and CDMA2000-only devices support only one bearer,
     * while GSM/UMTS devices typically support three or more, and any
     * LTE-capable device (whether LTE-only, GSM/UMTS-capable, and/or
     * CDMA2000-capable) also typically support three or more.
     */
    uint maxBearers() const;

    /**
     * @return The maximum number of active packet data bearers the modem supports.
     *
     * POTS and CDMA2000-only devices support one active bearer, while GSM/UMTS
     * and LTE-capable devices (including LTE/CDMA devices) typically support at
     * least two active bearers.
     */
    uint maxActiveBearers() const;

    /**
     * @return The equipment manufacturer, as reported by the modem.
     */
    QString manufacturer() const;

    /**
     * @return The equipment model, as reported by the modem.
     */
    QString model() const;

    /**
     * @return The revision identification of the software, as reported by the modem.
     */
    QString revision() const;

    /**
     * @return A best-effort device identifier based on various device
     * information like model name, firmware revision, USB/PCI/PCMCIA IDs, and
     * other properties.
     *
     * This ID is not guaranteed to be unique and may be shared between
     * identical devices with the same firmware, but is intended to be "unique
     * enough" for use as a casual device identifier for various user experience
     * operations.
     *
     * This is not the device's IMEI or ESN since those may not be available
     * before unlocking the device via a PIN.
     */
    QString deviceIdentifier() const;

    /**
     * @return The physical modem device reference (ie, USB, PCI, PCMCIA device), which may be dependent upon the operating system.
     *
     * In Linux for example, this points to a sysfs path of the usb_device object.
     */
    QString device() const;

    /**
     * @return The Operating System device drivers handling communication with the modem hardware.
     */
    QStringList drivers() const;

    /**
     * @return The name of the plugin handling this modem.
     */
    QString plugin() const;

    /**
     * @return The name of the primary port using to control the modem.
     */
    QString primaryPort() const;

    /**
     * @return The identity of the device.
     *
     * This will be the IMEI number for GSM devices and the hex-format ESN/MEID for CDMA devices.
     */
    QString equipmentIdentifier() const;

    /**
     * @return Current lock state of the device, given as a MMModemLock value.
     */
    MMModemLock unlockRequired() const;

    /**
     * @return A dictionary in which the keys are MMModemLock flags, and the
     * values are integers giving the number of PIN tries remaining before the
     * code becomes blocked (requiring a PUK) or permanently blocked. Dictionary
     * entries exist only for the codes for which the modem is able to report
     * retry counts.
     */
    UnlockRetriesMap unlockRetries() const;

    /**
     * @return Overall state of the modem, given as a MMModemState value.
     *
     * If the device's state cannot be determined, MM_MODEM_STATE_UNKNOWN will be reported.
     */
    MMModemState state() const;

    /**
     * @return Error specifying why the modem is in MM_MODEM_STATE_FAILED state, given as a MMModemStateFailedReason value.
     */
    MMModemStateFailedReason stateFailedReason() const;

    /**
     * @return QFlags of MMModemAccessTechnology values, specifying the current
     * network access technologies used by the device to communicate with the
     * network.
     *
     * If the device's access technology cannot be determined, MM_MODEM_ACCESS_TECHNOLOGY_UNKNOWN will be reported.
     */
    AccessTechnologies accessTechnologies() const;

    /**
     * @return Signal quality in percent (0 - 100) of the dominant access
     * technology the device is using to communicate with the network. Always 0
     * for POTS devices.
     *
     * The additional boolean value indicates if the quality value given was recently taken.
     */
    SignalQualityPair signalQuality() const;

    /**
     * @return List of numbers (e.g. MSISDN in 3GPP) being currently handled by this modem.
     */
    QStringList ownNumbers() const;

    /**
     * @return A MMModemPowerState value specifying the current power state of the modem.
     */
    MMModemPowerState powerState() const;

    /**
     * @return This property exposes the supported mode combinations, given as an list of unsigned integer pairs, where:
     * The first integer is a bitmask of MMModemMode values, specifying the allowed modes.
     * The second integer is a single MMModemMode, which specifies the preferred access technology, among the ones defined in the allowed modes.
     */
    SupportedModesType supportedModes() const;

    /**
     * @return A pair of MMModemMode values, where the first one is a bitmask
     * specifying the access technologies (eg 2G/3G/4G) the device is currently
     * allowed to use when connecting to a network, and the second one is the
     * preferred mode of those specified as allowed.
     *
     * The pair must be one of those specified in supportedModes()
     */
    CurrentModesType currentModes() const;

    /**
     * @return List of MMModemBand values, specifying the radio frequency and technology bands supported by the device.
     *
     * For POTS devices, only the MM_MODEM_BAND_ANY mode will be returned.
     */
    QList<MMModemBand> supportedBands() const;

    /**
     * @return List of MMModemBand values, specifying the radio frequency and
     * technology bands the device is currently using when connecting to a
     * network.
     *
     * It must be a subset of supportedBands()
     */
    QList<MMModemBand> currentBands() const;

    /**
     * @return QFlags of MMBearerIpFamily values, specifying the IP families supported by the device.
     */
    IpBearerFamilies supportedIpFamilies() const;


    // From org.freedesktop.ModemManager.Modem.Simple

    //  Dictionary of properties needed to get the modem connected.
    //  Each implementation is free to add its own specific key-value pairs. The predefined
    //  common ones are:
    //
    //  "pin": SIM-PIN unlock code, given as a string value (signature "s").
    //  "operator-id": ETSI MCC-MNC of a network to force registration with, given as a string value (signature "s").
    //  "apn": For GSM/UMTS and LTE devices the APN to use, given as a string value (signature "s").
    //  "ip-type": For GSM/UMTS and LTE devices the IP addressing type to use, given as a MMBearerIpFamily value (signature "u").
    //  "allowed-auth": The authentication method to use, given as a MMBearerAllowedAuth value (signature "u"). Optional in 3GPP.
    //  "user": User name (if any) required by the network, given as a string value (signature "s"). Optional in 3GPP.
    //  "password": Password (if any) required by the network, given as a string value (signature "s"). Optional in 3GPP.
    //  "number": For POTS devices the number to dial,, given as a string value (signature "s").
    //  "allow-roaming": FALSE to allow only connections to home networks, given as a boolean value (signature "b").
    //  "rm-protocol": For CDMA devices, the protocol of the Rm interface, given as a MMModemCdmaRmProtocol value (signature "u").
    QDBusObjectPath connectModem(const QVariantMap &properties);

    //  Dictionary of properties.
    //  Each implementation is free to add it's own specific key-value pairs. The predefined
    //  common ones are:
    //
    //  "state": A MMModemState value specifying the overall state of the modem, given as an unsigned integer value (signature "u").
    //  "signal-quality": Signal quality value, given only when registered, as an unsigned integer value (signature "u").
    //  "current-bands": List of MMModemBand values, given only when registered, as a list of unsigned integer values (signature "au").
    //  "access-technology": A MMModemAccessTechnology value, given only when registered, as an unsigned integer value (signature "u").
    //  "m3gpp-registration-state":A MMModem3gppRegistrationState value specifying the state of the registration, given only when registered in a 3GPP network, as an unsigned integer value (signature "u").
    //  "m3gpp-operator-code":Operator MCC-MNC, given only when registered in a 3GPP network, as a string value (signature "s").
    //  "m3gpp-operator-name":Operator name, given only when registered in a 3GPP network, as a string value (signature "s").
    //  "cdma-cdma1x-registration-state":A MMModemCdmaRegistrationState value specifying the state of the registration, given only when registered in a CDMA1x network, as an unsigned integer value (signature "u").
    //  "cdma-evdo-registration-state":A MMModemCdmaRegistrationState value specifying the state of the registration, given only when registered in a EV-DO network, as an unsigned integer value (signature "u").
    //  "cdma-sid":The System Identifier of the serving network, if registered in a CDMA1x network and if known. Given as an unsigned integer value (signature "u").
    //  "cdma-nid":The Network Identifier of the serving network, if registered in a CDMA1x network and if known. Given as an unsigned integer value (signature "u").
    QVariantMap status();

    void disconnectModem(const QString &bearer);
    void disconnectAllModems();

Q_SIGNALS:
    void deviceChanged(const QString &device);
    void driversChanged(const QStringList &drivers);
    void enabledChanged(bool enabled);
    void unlockRequiredChanged(MMModemLock lock);
    /**
     * The modem's state (see state()) changed.
     */
    void stateChanged(MMModemState oldState, MMModemState newState, MMModemStateChangeReason reason);
    void signalQualityChanged(uint percentStrength);
    void accessTechnologyChanged(ModemManager::Modem::AccessTechnologies tech);
    void currentModesChanged();
    void simPathChanged(const QString &oldPath, const QString &newPath);
    /**
     * Emitted when the modem's power state changes
     * @param state the new state
     * @see powerState()
     */
    void powerStateChanged(MMModemPowerState state);

private Q_SLOTS:
    void onPropertiesChanged(const QString &ifaceName, const QVariantMap &changedProps, const QStringList &invalidatedProps);
    void onStateChanged(int oldState, int newState, uint reason);

private:
    void init();
};

Q_DECLARE_OPERATORS_FOR_FLAGS(Modem::Capabilities)
Q_DECLARE_OPERATORS_FOR_FLAGS(Modem::AccessTechnologies)
Q_DECLARE_OPERATORS_FOR_FLAGS(Modem::ModemModes)
Q_DECLARE_OPERATORS_FOR_FLAGS(Modem::IpBearerFamilies)

} // namespace ModemManager

#endif
