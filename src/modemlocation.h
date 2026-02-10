/*
    SPDX-FileCopyrightText: 2008, 2011 Will Stephenson <wstephenson@kde.org>
    SPDX-FileCopyrightText: 2010 Lamarque Souza <lamarque@kde.org>
    SPDX-FileCopyrightText: 2013 Lukas Tinkl <ltinkl@redhat.com>
    SPDX-FileCopyrightText: 2013 Jan Grulich <jgrulich@redhat.com>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef MODEMMANAGERQT_MODEMLOCATION_H
#define MODEMMANAGERQT_MODEMLOCATION_H

#include <modemmanagerqt_export.h>

#include <QDBusPendingReply>
#include <QObject>
#include <QSharedPointer>
#include <QStringList>

#include "generictypes.h"
#include "interface.h"

namespace ModemManager
{
class ModemLocationPrivate;

/*!
 * \class ModemManager::ModemLocation
 *
 * \inheaderfile ModemManagerQt/ModemLocation
 *
 * \inmodule ModemManagerQt
 *
 * \brief The ModemLocation class.
 *
 * The Location class allows devices to provide location information to
 * client applications. Not all devices can provide this information, or even if
 * they do, they may not be able to provide it while a data session is active.
 */
class MODEMMANAGERQT_EXPORT ModemLocation : public Interface
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(ModemLocation)
    Q_FLAGS(MMModemLocationSource)
    Q_FLAGS(MMModemLocationAssistanceDataType)

public:
    /*!
     * \typedef ModemManager::ModemLocation::Ptr
     */
    typedef QSharedPointer<ModemLocation> Ptr;
    /*!
     * \typedef ModemManager::ModemLocation::List
     */
    typedef QList<Ptr> List;

    Q_DECLARE_FLAGS(LocationSources, MMModemLocationSource)
    Q_DECLARE_FLAGS(AssistanceDataTypes, MMModemLocationAssistanceDataType)

    /*!
     */
    explicit ModemLocation(const QString &path, QObject *parent = nullptr);
    ~ModemLocation() override;

    /*!
     * Configure the location sources to use when gathering location
     * information. Also enable or disable location information gathering. This
     * method may require the client to authenticate itself.
     *
     * When signals are emitted, any client application (including malicious
     * ones!) can listen for location updates unless D-Bus permissions restrict
     * these signals from certain users. If further security is desired, the
     * \a signLocation argument can be set to FALSE to disable location updates
     * via the locationChanged() signal and require applications to call authenticated APIs
     * (like GetLocation() ) to get location information.
     */
    QDBusPendingReply<void> setup(ModemManager::ModemLocation::LocationSources sources, bool signalLocation);

    /*!
     * Returns current location information, if any. If the modem supports
     * multiple location types it may return more than one. See the "Location"
     * property for more information on the dictionary returned at location.
     *
     * This method may require the client to authenticate itself.
     */
    QDBusPendingReply<LocationInformationMap> getLocation();

    /*!
     * Set the SUPL server to use for A-GPS.
     *
     * \since 6.24.0
     */
    QDBusPendingReply<void> setSuplServer(const QString &server);

    /*!
     * Inject assistance data into the modem.
     *
     * \since 6.24.0
     */
    QDBusPendingReply<void> injectAssistanceData(const QByteArray &data);

    /*!
     * Set the refresh rate for the GPS engine.
     *
     * \since 6.24.0
     */
    QDBusPendingReply<void> setGpsRefreshRate(uint rate);

    /*!
     * Returns QFlags of MMModemLocationSource values, specifying the supported location sources.
     */
    LocationSources capabilities() const;

    /*!
     * Returns QFlags specifying which of the supported MMModemLocationSource location sources is currently enabled in the device.
     */
    LocationSources enabledCapabilities() const;

    /*!
     * Returns whether the device has any location capabilities
     */
    bool isEnabled() const;

    /*!
     * Returns supported assistance data types.
     *
     * \since 6.24.0
     */
    AssistanceDataTypes supportedAssistanceData() const;

    /*!
     * Returns the configured SUPL server.
     *
     * \since 6.24.0
     */
    QString suplServer() const;

    /*!
     * Returns the list of assistance data servers.
     *
     * \since 6.24.0
     */
    QStringList assistanceDataServers() const;

    /*!
     * Returns the GPS refresh rate in seconds.
     *
     * \since 6.24.0
     */
    uint gpsRefreshRate() const;

    /*!
     * Returns TRUE if location updates will be emitted via the locationChanged() signal, FALSE if location updates will not be emitted.
     *
     * See the setup() method for more information.
     */
    bool signalsLocation() const;

    /*!
     * Returns Dictionary of available location information when location information gathering is enabled. If the modem supports multiple
     *         location types it may return more than one here.
     * Note that if the device was told not to emit updated location information when location information gathering was initially enabled,
     * this property may not return any location information for security reasons.
     */
    LocationInformationMap location() const;

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
    void capabilitiesChanged(QFlags<MMModemLocationSource> capabilities);
    /*!
     */
    void enabledCapabilitiesChanged(QFlags<MMModemLocationSource> capabilities);
    /*!
     * \since 6.24.0
     */
    void supportedAssistanceDataChanged(QFlags<MMModemLocationAssistanceDataType> supported);
    /*!
     * \since 6.24.0
     */
    void suplServerChanged(const QString &server);
    /*!
     * \since 6.24.0
     */
    void assistanceDataServersChanged(const QStringList &servers);
    /*!
     * \since 6.24.0
     */
    void gpsRefreshRateChanged(uint rate);
    /*!
     */
    void signalsLocationChanged(bool signalsLocation);
    /*!
     * Emitted when the location has changed
     */
    void locationChanged(const ModemManager::LocationInformationMap &location);
};

Q_DECLARE_OPERATORS_FOR_FLAGS(ModemLocation::LocationSources)
Q_DECLARE_OPERATORS_FOR_FLAGS(ModemLocation::AssistanceDataTypes)

} // namespace ModemManager

#endif
