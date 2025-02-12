/*
    SPDX-FileCopyrightText: 2014 Lukas Tinkl <ltinkl@redhat.com>
    SPDX-FileCopyrightText: 2015 Jan Grulich <jgrulich@redhat.com>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef MODEMMANAGERQT_MODEMSIGNAL_H
#define MODEMMANAGERQT_MODEMSIGNAL_H

#include "interface.h"
#include <modemmanagerqt_export.h>

#include <QDBusPendingReply>
#include <QObject>
#include <QSharedPointer>

namespace ModemManager
{
class ModemSignalPrivate;

/*!
 * \class ModemManager::ModemSignal
 *
 * \inheaderfile ModemManagerQt/ModemSignal
 *
 * \inmodule ModemManagerQt
 *
 * \brief The ModemSignal class.
 *
 * This class provides access to extended signal quality information.
 *
 * \since 1.1.94
 */
class MODEMMANAGERQT_EXPORT ModemSignal : public Interface
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(ModemSignal)

public:
    /*!
     * \typedef ModemManager::ModemSignal::Ptr
     */
    typedef QSharedPointer<ModemSignal> Ptr;
    /*!
     * \typedef ModemManager::ModemSignal::List
     */
    typedef QList<Ptr> List;

    /*!
     */
    explicit ModemSignal(const QString &path, QObject *parent = nullptr);
    ~ModemSignal() override;

    /*!
     * Returns refresh rate for the extended signal quality information updates, in seconds.
     * A value of 0 disables the retrieval of the values.
     */
    uint rate() const;

    /*!
     * Returns Dictionary of available signal information for the CDMA1x access technology.
     *
     * This dictionary is composed of a string key, with an associated data which contains type-specific information.
     *
     * \a rssi The CDMA1x RSSI (Received Signal Strength Indication), in dBm, given as a floating point value (signature "d").
     *
     * \a ecio The CDMA1x Ec/Io, in dBm, given as a floating point value (signature "d").
     */
    QVariantMap cdma() const;

    /*!
     * Returns Dictionary of available signal information for the CDMA EV-DO access technology.
     *
     * This dictionary is composed of a string key, with an associated data which contains type-specific information.
     *
     * \a rssi The CDMA EV-DO RSSI (Received Signal Strength Indication), in dBm, given as a floating point value (signature "d").
     *
     * \a ecio The CDMA EV-DO Ec/Io, in dBm, given as a floating point value (signature "d").
     *
     * \a sinr CDMA EV-DO SINR level, in dB, given as a floating point value (signature "d").
     *
     * \a io The CDMA EV-DO Io, in dBm, given as a floating point value (signature "d").
     */
    QVariantMap evdo() const;

    /*!
     * Returns Dictionary of available signal information for the GSM/GPRS access technology.
     *
     * This dictionary is composed of a string key, with an associated data which contains type-specific information.
     *
     * \a rssi The GSM RSSI (Received Signal Strength Indication), in dBm, given as a floating point value (signature "d").
     */
    QVariantMap gsm() const;

    /*!
     * Returns Dictionary of available signal information for the UMTS (WCDMA) access technology.
     *
     * This dictionary is composed of a string key, with an associated data which contains type-specific information.
     *
     * \a rssi The LTE RSSI (Received Signal Strength Indication), in dBm, given as a floating point value (signature "d").
     *
     * \a rsrq The LTE RSRQ (Reference Signal Received Quality), in dB, given as a floating point value (signature "d").
     *
     * \a rsrp The LTE RSRP (Reference Signal Received Power), in dBm, given as a floating point value (signature "d").
     *
     * \a snr The LTE S/R ratio, in dB, given as a floating point value (signature "d").
     */
    QVariantMap lte() const;

    /*!
     * Returns Dictionary of available signal information for the UMTS (WCDMA) access technology.
     *
     * This dictionary is composed of a string key, with an associated data which contains type-specific information.
     *
     * \a rssi The UMTS RSSI (Received Signal Strength Indication), in dBm, given as a floating point value (signature "d").
     *
     * \a ecio The UMTS Ec/Io, in dBm, given as a floating point value (signature "d").
     */
    QVariantMap umts() const;

    /*!
     * Setup extended signal quality information retrieval.
     *
     * \a rate refresh rate to set, in seconds. 0 to disable retrieval.
     */
    QDBusPendingReply<void> setup(uint rate);

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
    void rateChanged(uint rate);
    /*!
     */
    void cdmaChanged(const QVariantMap &cdma);
    /*!
     */
    void evdoChanged(const QVariantMap &evdo);
    /*!
     */
    void gsmChanged(const QVariantMap &gsm);
    /*!
     */
    void umtsChanged(const QVariantMap &umts);
    /*!
     */
    void lteChanged(const QString &lte);
};

} // namespace ModemManager

#endif
