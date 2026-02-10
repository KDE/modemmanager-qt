/*
    SPDX-FileCopyrightText: 2008, 2011 Will Stephenson <wstephenson@kde.org>
    SPDX-FileCopyrightText: 2010 Lamarque Souza <lamarque@kde.org>
    SPDX-FileCopyrightText: 2013 Lukas Tinkl <ltinkl@redhat.com>
    SPDX-FileCopyrightText: 2013-2015 Jan Grulich <jgrulich@redhat.com>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef MODEMMANAGERQT_MODEM3GPP_H
#define MODEMMANAGERQT_MODEM3GPP_H

#include <modemmanagerqt_export.h>

#include <QByteArray>
#include <QDBusPendingReply>
#include <QObject>
#include <QSharedPointer>

#include "generictypes.h"
#include "interface.h"

namespace ModemManager
{
class Modem3gppPrivate;

/*!
 * \class ModemManager::Modem3gpp
 *
 * \inheaderfile ModemManagerQt/Modem3gpp
 *
 * \inmodule ModemManagerQt
 *
 * \brief The Modem3gpp class.
 *
 * This class provides access to specific actions that may be performed in modems with 3GPP capabilities.
 */
class MODEMMANAGERQT_EXPORT Modem3gpp : public Interface
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(Modem3gpp)
    Q_FLAGS(MMModem3gppFacility)

public:
    /*!
     * \typedef ModemManager::Modem3gpp::Ptr
     */
    typedef QSharedPointer<Modem3gpp> Ptr;
    /*!
     * \typedef ModemManager::Modem3gpp::List
     */
    typedef QList<Ptr> List;

    Q_DECLARE_FLAGS(FacilityLocks, MMModem3gppFacility)

    /*!
     */
    explicit Modem3gpp(const QString &path, QObject *parent = nullptr);
    ~Modem3gpp() override;

    /*!
     * Returns the IMEI of the device
     */
    QString imei() const;

    /*!
     * Returns mobile registration status as defined in 3GPP TS 27.007 section 10.1.19
     */
    MMModem3gppRegistrationState registrationState() const;

    /*!
     * Returns code of the operator to which the mobile is currently registered.
     *
     * Returned in the format "MCCMNC", where MCC is the three-digit ITU E.212
     * Mobile Country Code and MNC is the two- or three-digit GSM Mobile Network
     * Code. e.g. e"31026" or "310260".
     *
     * If the MCC and MNC are not known or the mobile is not registered to a
     * mobile network, this property will be an empty string.
     */
    QString operatorCode() const;

    /*!
     * Returns name of the operator to which the mobile is currently registered.
     *
     * If the operator name is not known or the mobile is not registered to a
     * mobile network, this property will be an empty string.
     */
    QString operatorName() const;

    /*!
     * Returns country code of the operator to which the mobile is currently registered.
     *
     * Returned in the format "ISO 3166-1 alpha-2" according to the MMC mapping from Wikipedia.
     * Country Code is the two-letter country codes defined in ISO 3166-1, part of the ISO 3166 standard,
     * e.g. "RU" or "FI".
     *
     * If the MCC is not known or the mobile is not registered to a
     * mobile network, this property will be an empty string.
     */
    QString countryCode() const;

    /*!
     * Returns QFlags of MMModem3gppFacility values for which PIN locking is enabled
     */
    FacilityLocks enabledFacilityLocks() const;

#if MM_CHECK_VERSION(1, 2, 0)
    /*!
     * Returns Value representing the subscription status of the account and whether there is any data remaining.
     */
    MMModem3gppSubscriptionState subscriptionState() const;
#endif
    /*!
     * Register the device to network.
     *
     * \a networkId The operator ID (ie, "MCCMNC", like "310260") to register. An empty string can be used to register to the home network.
     */
    void registerToNetwork(const QString &networkId = QString());

    /*!
     * Sends control key to modem to disable the selected facility lock.
     *
     * \a facility A MMModem3gppFacility value representing the type of the facility lock to disable.
     * \a controlKey Alphanumeric key required to unlock the facility.
     *
     * \since 6.24.0
     */
    QDBusPendingReply<void> disableFacilityLock(MMModem3gppFacility facility, const QString &controlKey);

    /*!
     * Sends the list of carrier network information to the modem to configure carrier lock.
     *
     * \a data The list of carrier network information to be sent to the modem.
     *
     * \since 6.24.0
     */
    QDBusPendingReply<void> setCarrierLock(const QByteArray &data);

    /*!
     * Explicitly attach or detach packet service on the current registered network.
     *
     * \a state A MMModem3gppPacketServiceState value.
     *
     * \since 6.24.0
     */
    QDBusPendingReply<void> setPacketServiceState(MMModem3gppPacketServiceState state);

    /*!
     * Sets the UE mode of operation for EPS.
     *
     * \a mode A MMModem3gppEpsUeModeOperation value.
     *
     * \since 6.24.0
     */
    QDBusPendingReply<void> setEpsUeModeOperation(MMModem3gppEpsUeModeOperation mode);

    /*!
     * Updates the default settings to be used in the initial default EPS bearer
     * when registering to the LTE network.
     *
     * The allowed properties are the 3GPP-specific ones specified in the bearer properties:
     * "apn", "ip-type", "allowed-auth", "user", and "password".
     *
     * \a settings List of properties to use when requesting the LTE attach procedure.
     *
     * \since 6.24.0
     */
    QDBusPendingReply<void> setInitialEpsBearerSettings(const QVariantMap &settings);

    /*!
     * Updates the 5G specific registration settings configured in the device.
     *
     * The allowed properties are: "mico-mode" (a MMModem3gppMicoMode value representing the
     * Mobile Initiated Connection mode requested by the host) and "drx-cycle" (a
     * MMModem3gppDrxCycle value representing the DRX settings requested by the host).
     *
     * \a properties List of 5G specific registration settings.
     *
     * \since 6.24.0
     */
    QDBusPendingReply<void> setNr5gRegistrationSettings(const QVariantMap &properties);

    /*!
     * Scan for available networks.
     *
     * Returns a QList<QVariantMap> with the results, where each map may contain these values:
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

    /*!
     * Returns the latest network rejection information received from the network
     * during registration failure. This is cleared whenever the modem successfully registers.
     *
     * Network errors are defined in 3GPP TS 24.008 sections 10.5.3.6 and 10.5.5.14
     * (detailed in annex G) and in 3GPP TS 24.301 section 9.9.3.9.
     *
     * The returned map always contains "error" (a MMNetworkError value), and may optionally
     * include "operator-id", "operator-name", and "access-technology" (a MMModemAccessTechnology value).
     *
     * \since 6.24.0
     */
    QVariantMap networkRejection() const;

    /*!
     * Returns a MMModem3gppEpsUeModeOperation value representing the UE mode of
     * operation for EPS.
     *
     * \since 6.24.0
     */
    MMModem3gppEpsUeModeOperation epsUeModeOperation() const;

    /*!
     * Returns the raw Protocol Configuration Options (PCO) received from the network.
     *
     * Each PCO element is a sequence of: the session ID (unsigned integer), a flag
     * indicating whether the data contains the complete PCO structure (boolean), and
     * the raw PCO data (byte array).
     *
     * \since 6.24.0
     */
    ModemManager::PcoInfoList pco() const;

    /*!
     * Returns the object path for the initial default EPS bearer.
     *
     * \since 6.24.0
     */
    QString initialEpsBearer() const;

    /*!
     * Returns the properties requested by the device for the initial EPS bearer during
     * LTE network attach procedure.
     *
     * The network may decide to use different settings during the actual device attach
     * procedure, e.g. if the device is roaming or no explicit settings were requested,
     * so the values shown in the InitialEpsBearer bearer object may be totally different.
     *
     * These settings should be updated using setInitialEpsBearerSettings().
     *
     * \since 6.24.0
     */
    QVariantMap initialEpsBearerSettings() const;

    /*!
     * Returns a MMModem3gppPacketServiceState value specifying the packet domain
     * service state.
     *
     * \since 6.24.0
     */
    MMModem3gppPacketServiceState packetServiceState() const;

    /*!
     * Returns the 5G specific registration settings.
     *
     * The returned map may contain: "mico-mode" (a MMModem3gppMicoMode value representing
     * the Mobile Initiated Connection mode requested by the host) and "drx-cycle" (a
     * MMModem3gppDrxCycle value representing the DRX settings requested by the host).
     *
     * These settings should be updated using setNr5gRegistrationSettings().
     *
     * \since 6.24.0
     */
    QVariantMap nr5gRegistrationSettings() const;

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
    void imeiChanged(const QString &imei);
    /*!
     */
    void registrationStateChanged(MMModem3gppRegistrationState registrationState);
    /*!
     */
    void operatorCodeChanged(const QString &operatorCode);
    /*!
     */
    void operatorNameChanged(const QString &operatorName);
    /*!
     */
    void countryCodeChanged(const QString &countryCode);
    /*!
     */
    void enabledFacilityLocksChanged(QFlags<MMModem3gppFacility> locks);
    /*!
     */
    void subscriptionStateChanged(MMModem3gppSubscriptionState subscriptionState);
    /*!
     * \since 6.24.0
     */
    void networkRejectionChanged(const QVariantMap &networkRejection);
    /*!
     * \since 6.24.0
     */
    void epsUeModeOperationChanged(MMModem3gppEpsUeModeOperation mode);
    /*!
     * \since 6.24.0
     */
    void pcoChanged(const ModemManager::PcoInfoList &pco);
    /*!
     * \since 6.24.0
     */
    void initialEpsBearerChanged(const QString &path);
    /*!
     * \since 6.24.0
     */
    void initialEpsBearerSettingsChanged(const QVariantMap &settings);
    /*!
     * \since 6.24.0
     */
    void packetServiceStateChanged(MMModem3gppPacketServiceState state);
    /*!
     * \since 6.24.0
     */
    void nr5gRegistrationSettingsChanged(const QVariantMap &settings);
};

Q_DECLARE_OPERATORS_FOR_FLAGS(Modem3gpp::FacilityLocks)

} // namespace ModemManager

#endif
