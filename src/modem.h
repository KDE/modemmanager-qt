/*
    SPDX-FileCopyrightText: 2008, 2011 Will Stephenson <wstephenson@kde.org>
    SPDX-FileCopyrightText: 2010 Lamarque Souza <lamarque@kde.org>
    SPDX-FileCopyrightText: 2013 Daniel Nicoletti <dantti12@gmail.com>
    SPDX-FileCopyrightText: 2013 Lukas Tinkl <ltinkl@redhat.com>
    SPDX-FileCopyrightText: 2013-2015 Jan Grulich <jgrulich@redhat.com>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef MODEMMANAGERQT_MODEM_H
#define MODEMMANAGERQT_MODEM_H

#include <ModemManager/ModemManager.h>

#include <modemmanagerqt_export.h>

#include <QDBusObjectPath>
#include <QObject>
#include <QSharedPointer>

#include "bearer.h"
#include "generictypes.h"
#include "interface.h"

namespace ModemManager
{
class ModemPrivate;

/*!
 * \class ModemManager::BearerProperties
 *
 * \inheaderfile ModemManagerQt/Modem
 *
 * \inmodule ModemManagerQt
 *
 * \brief This class represents bearer properties used for creating of new bearers.
 */
class MODEMMANAGERQT_EXPORT BearerProperties
{
public:
    /*!
     * Constructs an empty BearerProperties object
     */
    BearerProperties();

    /*!
     * Destroys this BearerProperties object.
     */
    ~BearerProperties();

    /*!
     * Constructs a BearerProperties object that is a copy of the object \a other.
     */
    BearerProperties(const BearerProperties &other);

    /*!
     * Returns Access Point Name
     */
    QString apn() const;

    /*!
     * Sets Access Point Name
     */
    void setApn(const QString &apn);

    /*!
     * Returns addressing type
     */
    MMBearerIpFamily ipType() const;

    /*!
     * Sets addressing type
     */
    void setIpType(MMBearerIpFamily ipType);

    /*!
     * Returns used authentication method
     */
    MMBearerAllowedAuth allowedAuthentication() const;

    /*!
     * Sets the authentication method to use
     */
    void setAllowedAuthentication(MMBearerAllowedAuth allowedAuth);

    /*!
     * Returns user name
     */
    QString user() const;

    /*!
     * Sets user name
     */
    void setUser(const QString &user);

    /*!
     * Returns password
     */
    QString password() const;

    /*!
     * Sets password
     */
    void setPassword(const QString &password);

    /*!
     * Returns whether connection is allowed during roaming
     */
    bool allowRoaming() const;

    /*!
     * Sets whether connection is allowed during roaming
     */
    void setAllowRoaming(bool allow);

    /*!
     * Returns protocol of the Rm interface
     */
    MMModemCdmaRmProtocol rmProtocol() const;

    /*!
     * Sets protocol of the Rm interface
     */
    void setRmProtocol(MMModemCdmaRmProtocol rmProtocol);

    /*!
     * Returns telephone number to dial
     */
    QString number() const;

    /*!
     * Sets telephone number to dial
     */
    void setNumber(const QString &number);

    /*!
     * Makes a copy of the IpConfig object \a other.
     */
    BearerProperties &operator=(const BearerProperties &other);

private:
    class Private;
    Private *const d;
};

/*!
 * \class ModemManager::Modem
 *
 * \inheaderfile ModemManagerQt/Modem
 *
 * \inmodule ModemManagerQt
 *
 * \brief The Modem class.
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

    /*!
     * \typedef ModemManager:Modem:Ptr
     */
    typedef QSharedPointer<Modem> Ptr;
    /*!
     * \typedef ModemManager:Modem:Ptr
     */
    typedef QList<Ptr> List;

    /*!
     */
    explicit Modem(const QString &path, QObject *parent = nullptr);
    ~Modem() override;

    /*!
     */
    QString uni() const;
    /*!
     * Returns \a true if the modem is fully functional, \a false when in low power mode or disabled
     * \sa setEnabled()
     */
    bool isEnabled() const;
    /*!
     */
    bool isValid() const;

    /*!
     * Enable or disable the modem.
     *
     * When enabled, the modem's radio is powered on and data sessions, voice calls, location services, and Short Message Service may be available.
     *
     * When disabled, the modem enters low-power state and no network-related operations are available.
     */
    QDBusPendingReply<void> setEnabled(bool enable);

    /*!
     * Create a new packet data bearer using the given characteristics.
     *
     * This request may fail if the modem does not support additional bearers, if too many bearers are already defined, or if properties are invalid.
     *
     */
    QDBusPendingReply<QDBusObjectPath> createBearer(const ModemManager::BearerProperties &bearerProperties);

    /*!
     * Delete an existing packet data bearer.
     *
     * If the bearer is currently active and providing packet data server, it will be disconnected and that packet data service will terminate.
     *
     * \a bearer path to the bearer to delete
     */
    QDBusPendingReply<void> deleteBearer(const QString &bearer);

    /*!
     * Returns the configured packet data bearers (EPS Bearers, PDP Contexts, or CDMA2000 Packet Data Sessions).
     */
    ModemManager::Bearer::List listBearers() const;

    /*!
     * Returns the configured packet data bearer on given path
     */
    ModemManager::Bearer::Ptr findBearer(const QString &bearer) const;

    /*!
     * Clear non-persistent configuration and state, and return the device to a newly-powered-on state.
     *
     * This command may power-cycle the device.
     */
    QDBusPendingReply<void> reset();

    /*!
     * Clear the modem's configuration (including persistent configuration and state), and return the device to a factory-default state.
     *
     * If not required by the modem, \a code may be ignored.
     *
     * This command may or may not power-cycle the device.
     *
     * \a code Carrier-supplied code required to reset the modem.
     */
    QDBusPendingReply<void> factoryReset(const QString &code);

    /*!
     * Set the power \a state of the modem. This action can only be run when the modem is in MM_MODEM_STATE_DISABLED state.
     */
    QDBusPendingReply<void> setPowerState(MMModemPowerState state);

    /*!
     * Set the capabilities of the device. A restart of the modem may be required.
     *
     * \a caps QFlags of MMModemCapability values, to specify the capabilities to use.
     */
    QDBusPendingReply<void> setCurrentCapabilities(Capabilities caps);

    /*!
     * Set the access technologies (e.g. 2G/3G/4G preference) the device is currently allowed to use when connecting to a network.
     *
     * The given combination should be supported by the modem, as specified in supportedModes()
     */
    QDBusPendingReply<void> setCurrentModes(const CurrentModesType &mode);

    /*!
     * Set the radio frequency and technology bands the device is currently allowed to use when connecting to a network.
     *
     * \a bands List of MMModemBand values, to specify the bands to be used.
     */
    QDBusPendingReply<void> setCurrentBands(const QList<MMModemBand> &bands);

    /*!
     * Selects which SIM slot to be considered as primary, on devices that expose
     * multiple slots in the simSlots() property.
     *
     * When the switch happens the modem may require a full device reprobe, so the modem
     * object in DBus will get removed, and recreated once the selected SIM slot is in use.
     *
     * There is no limitation on which SIM slot to select, so the user may also set as
     * primary a slot that doesn't currently have any valid SIM card inserted.
     *
     * \a slot SIM slot number to set as primary.
     *
     * \since 6.24.0
     */
    QDBusPendingReply<void> setPrimarySimSlot(uint slot);

    /*!
     * Send an arbitrary AT command to a modem and get the response.
     *
     * Note that using this interface call is only allowed when running
     * ModemManager in debug mode or if the project was built using the
     * with-at-command-via-dbus configure option.
     *
     * \a cmd The command string, e.g. "AT+GCAP" or "+GCAP" (leading AT is inserted if necessary).
     * \a timeout The number of seconds to wait for a response.
     */
    QDBusPendingReply<QString> command(const QString &cmd, uint timeout);

    /*!
     * Get information for available cells in different access technologies,
     * either serving or neighboring along with radio frequency information.
     *
     * Returns an array of dictionaries, where each dictionary reports information for
     * one single cell. Each dictionary has mandatory keys "cell-type" (a MMCellType value)
     * and "serving" (a boolean indicating whether this is a serving or neighboring cell).
     * Additional keys depend on the cell type (CDMA, GSM, UMTS, TD-SCDMA, LTE, 5GNR).
     *
     * \since 6.24.0
     */
    QDBusPendingReply<QVariantMapList> getCellInfo();

    /*!
     * Returns The path of the SIM object available in this device, if any.
     */
    QString simPath() const;

    /*!
     * Returns the list of SIM slots available in the system, including the SIM object
     * paths if the cards are present. If a given SIM slot at a given index doesn't have
     * a SIM card available, an empty object path will be given.
     *
     * The length of this list will be equal to the amount of available SIM slots in the
     * system, and the index in the list is the slot index.
     *
     * \since 6.24.0
     */
    UIntList simSlots() const;

    /*!
     * Returns the index of the primary active SIM slot in the simSlots() list,
     * given in the [1,N] range.
     *
     * If multiple SIM slots aren't supported, this property will report value 0.
     *
     * In a Multi SIM Single Standby setup, this index identifies the only SIM
     * that is currently active. In a Multi SIM Multi Standby setup, this index
     * identifies the active SIM that is considered primary, i.e. the one that
     * will be used when a data connection is setup.
     *
     * \since 6.24.0
     */
    uint primarySimSlot() const;

    /*!
     * Returns List of MMModemCapability values, specifying the combinations of generic family of access technologies the modem supports.
     *
     * If the modem doesn't allow changing the current capabilities, a single entry with MM_MODEM_CAPABILITY_ANY will be given.
     */
    QList<MMModemCapability> supportedCapabilities() const;

    /*!
     * Returns QFlags of MMModemCapability values, specifying the generic family of
     * access technologies the modem currently supports without a firmware
     * reload or reinitialization.
     */
    Capabilities currentCapabilities() const;

    /*!
     * Returns The maximum number of defined packet data bearers the modem supports.
     *
     * This is not the number of active/connected bearers the modem supports,
     * but simply the number of bearers that may be defined at any given time.
     * For example, POTS and CDMA2000-only devices support only one bearer,
     * while GSM/UMTS devices typically support three or more, and any
     * LTE-capable device (whether LTE-only, GSM/UMTS-capable, and/or
     * CDMA2000-capable) also typically support three or more.
     */
    uint maxBearers() const;

    /*!
     * Returns The maximum number of active packet data bearers the modem supports.
     *
     * POTS and CDMA2000-only devices support one active bearer, while GSM/UMTS
     * and LTE-capable devices (including LTE/CDMA devices) typically support at
     * least two active bearers.
     */
    uint maxActiveBearers() const;

    /*!
     * Returns the maximum number of active MM_BEARER_TYPE_DEFAULT bearers that may be
     * explicitly enabled by the user with multiplexing support on one single network interface.
     *
     * If the modem doesn't support multiplexing of data sessions, a value of 0 will be reported.
     *
     * \since 6.24.0
     */
    uint maxActiveMultiplexedBearers() const;

    /*!
     * Returns The equipment manufacturer, as reported by the modem.
     */
    QString manufacturer() const;

    /*!
     * Returns The equipment model, as reported by the modem.
     */
    QString model() const;

    /*!
     * Returns The revision identification of the software, as reported by the modem.
     */
    QString revision() const;

    /*!
     * Returns the revision identification of the hardware, as reported by the modem.
     *
     * \since 6.24.0
     */
    QString hardwareRevision() const;

    /*!
     * Returns the description of the carrier-specific configuration (MCFG) in use by the modem.
     *
     * \since 6.24.0
     */
    QString carrierConfiguration() const;

    /*!
     * Returns the revision identification of the carrier-specific configuration (MCFG) in use by the modem.
     *
     * \since 6.24.0
     */
    QString carrierConfigurationRevision() const;

    /*!
     * Returns A best-effort device identifier based on various device
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

    /*!
     * Returns The physical modem device reference (ie, USB, PCI, PCMCIA device), which may be dependent upon the operating system.
     *
     * In Linux for example, this points to a sysfs path of the usb_device object.
     */
    QString device() const;

    /*!
     * Returns the physical modem device path (ie, USB, PCI, PCMCIA device), which
     * may be dependent upon the operating system.
     *
     * In Linux for example, this points to a sysfs path of the usb_device object.
     *
     * \since 6.24.0
     */
    QString physdev() const;

    /*!
     * Returns The Operating System device drivers handling communication with the modem hardware.
     */
    QStringList drivers() const;

    /*!
     * Returns The name of the plugin handling this modem.
     */
    QString plugin() const;

    /*!
     * Returns The name of the primary port using to control the modem.
     */
    QString primaryPort() const;

    /*!
     * Returns The list of ports in the modem, given as an array of string and unsigned integer pairs.
     * The string is the port name or path, and the integer is the port type given as a MMModemPortType value.
     *
     * \since 1.1.94
     */
    PortList ports() const;

    /*!
     * Returns The identity of the device.
     *
     * This will be the IMEI number for GSM devices and the hex-format ESN/MEID for CDMA devices.
     */
    QString equipmentIdentifier() const;

    /*!
     * Returns Current lock state of the device, given as a MMModemLock value.
     */
    MMModemLock unlockRequired() const;

    /*!
     * Returns A dictionary in which the keys are MMModemLock flags, and the
     * values are integers giving the number of PIN tries remaining before the
     * code becomes blocked (requiring a PUK) or permanently blocked. Dictionary
     * entries exist only for the codes for which the modem is able to report
     * retry counts.
     */
    UnlockRetriesMap unlockRetries() const;

    /*!
     * Returns Overall state of the modem, given as a MMModemState value.
     *
     * If the device's state cannot be determined, MM_MODEM_STATE_UNKNOWN will be reported.
     */
    MMModemState state() const;

    /*!
     * Returns Error specifying why the modem is in MM_MODEM_STATE_FAILED state, given as a MMModemStateFailedReason value.
     */
    MMModemStateFailedReason stateFailedReason() const;

    /*!
     * Returns QFlags of MMModemAccessTechnology values, specifying the current
     * network access technologies used by the device to communicate with the
     * network.
     *
     * If the device's access technology cannot be determined, MM_MODEM_ACCESS_TECHNOLOGY_UNKNOWN will be reported.
     */
    AccessTechnologies accessTechnologies() const;

    /*!
     * Returns Signal quality in percent (0 - 100) of the dominant access
     * technology the device is using to communicate with the network. Always 0
     * for POTS devices.
     *
     * The additional boolean value indicates if the quality value given was recently taken.
     */
    SignalQualityPair signalQuality() const;

    /*!
     * Returns List of numbers (e.g. MSISDN in 3GPP) being currently handled by this modem.
     */
    QStringList ownNumbers() const;

    /*!
     * Returns A MMModemPowerState value specifying the current power state of the modem.
     */
    MMModemPowerState powerState() const;

    /*!
     * Returns This property exposes the supported mode combinations, given as an list of unsigned integer pairs, where:
     * The first integer is a bitmask of MMModemMode values, specifying the allowed modes.
     * The second integer is a single MMModemMode, which specifies the preferred access technology, among the ones defined in the allowed modes.
     */
    SupportedModesType supportedModes() const;

    /*!
     * Returns A pair of MMModemMode values, where the first one is a bitmask
     * specifying the access technologies (eg 2G/3G/4G) the device is currently
     * allowed to use when connecting to a network, and the second one is the
     * preferred mode of those specified as allowed.
     *
     * The pair must be one of those specified in supportedModes()
     */
    CurrentModesType currentModes() const;

    /*!
     * Returns List of MMModemBand values, specifying the radio frequency and technology bands supported by the device.
     *
     * For POTS devices, only the MM_MODEM_BAND_ANY mode will be returned.
     */
    QList<MMModemBand> supportedBands() const;

    /*!
     * Returns List of MMModemBand values, specifying the radio frequency and
     * technology bands the device is currently using when connecting to a
     * network.
     *
     * It must be a subset of supportedBands()
     */
    QList<MMModemBand> currentBands() const;

    /*!
     * Returns QFlags of MMBearerIpFamily values, specifying the IP families supported by the device.
     */
    IpBearerFamilies supportedIpFamilies() const;

    /*!
     * Sets the timeout in milliseconds for all async method DBus calls.
     * -1 means the default DBus timeout (usually 25 seconds).
     */
    void setTimeout(int timeout);

    /*!
     * Returns the current value of the DBus timeout in milliseconds.
     * -1 means the default DBus timeout (usually 25 seconds).
     */
    int timeout() const;

Q_SIGNALS:
    /*!
     */
    void bearerAdded(const QString &bearer);
    /*!
     */
    void bearerRemoved(const QString &bearer);
    /*!
     */
    void bearersChanged();

    /*!
     */
    void simPathChanged(const QString &oldPath, const QString &newPath);
    /*!
     * \since 6.24.0
     */
    void simSlotsChanged(const ModemManager::UIntList &simSlots);
    /*!
     * \since 6.24.0
     */
    void primarySimSlotChanged(uint slot);
    /*!
     */
    void supportedCapabilitiesChanged(const QList<MMModemCapability> &supportedCapabilities);
    /*!
     */
    void currentCapabilitiesChanged(const QFlags<MMModemCapability> &currentCapabilities);
    /*!
     */
    void maxBearersChanged(uint maxBearers);
    /*!
     */
    void maxActiveBearersChanged(uint maxActiveBearers);
    /*!
     * \since 6.24.0
     */
    void maxActiveMultiplexedBearersChanged(uint maxActiveMultiplexedBearers);
    /*!
     */
    void manufacturerChanged(const QString &manufacturer);
    /*!
     */
    void modelChanged(const QString &model);
    /*!
     */
    void revisionChanged(const QString &revision);
    /*!
     * \since 6.24.0
     */
    void hardwareRevisionChanged(const QString &hardwareRevision);
    /*!
     * \since 6.24.0
     */
    void carrierConfigurationChanged(const QString &carrierConfiguration);
    /*!
     * \since 6.24.0
     */
    void carrierConfigurationRevisionChanged(const QString &carrierConfigurationRevision);
    /*!
     */
    void deviceIdentifierChanged(const QString &deviceIdentifier);
    /*!
     */
    void deviceChanged(const QString &device);
    /*!
     * Emitted when the physical modem device path changes.
     * \since 6.24.0
     */
    void physdevChanged(const QString &physdev);
    /*!
     */
    void driversChanged(const QStringList &drivers);
    /*!
     */
    void pluginChanged(const QString &plugin);
    /*!
     */
    void primaryPortChanged(const QString &primaryPort);
    /*!
     */
    void portsChanged(const ModemManager::PortList &ports);
    /*!
     */
    void equipmentIdentifierChanged(const QString &equipmentIdentifier);
    /*!
     */
    void unlockRequiredChanged(MMModemLock unlockRequired);
    /*!
     */
    void unlockRetriesChanged(const ModemManager::UnlockRetriesMap &unlockRetries);
    /*!
     */
    void stateFailedReasonChanged(MMModemStateFailedReason stateFailedReason);
    /*!
     */
    void accessTechnologiesChanged(QFlags<MMModemAccessTechnology> accessTechnologies);
    /*!
     */
    void signalQualityChanged(ModemManager::SignalQualityPair signalQuality);
    /*!
     */
    void ownNumbersChanged(const QStringList &ownNumbers);
    /*!
     */
    void powerStateChanged(MMModemPowerState powerState);
    /*!
     */
    void supportedModesChanged(ModemManager::SupportedModesType supportedModes);
    /*!
     */
    void currentModesChanged(ModemManager::CurrentModesType currentModes);
    /*!
     */
    void supportedBandsChanged(const QList<MMModemBand> &supportedBands);
    /*!
     */
    void currentBandsChanged(const QList<MMModemBand> &supportedBands);
    /*!
     */
    void supportedIpFamiliesChanged(QFlags<MMBearerIpFamily> supportedIpFamilies);
    /*!
     */
    void stateChanged(MMModemState oldState, MMModemState newState, MMModemStateChangeReason reason);
};

Q_DECLARE_OPERATORS_FOR_FLAGS(Modem::Capabilities)
Q_DECLARE_OPERATORS_FOR_FLAGS(Modem::AccessTechnologies)
Q_DECLARE_OPERATORS_FOR_FLAGS(Modem::ModemModes)
Q_DECLARE_OPERATORS_FOR_FLAGS(Modem::IpBearerFamilies)

} // namespace ModemManager

#endif
