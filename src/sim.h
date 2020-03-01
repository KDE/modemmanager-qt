/*
    SPDX-FileCopyrightText: 2008, 2011 Will Stephenson <wstephenson@kde.org>
    SPDX-FileCopyrightText: 2010-2011 Lamarque Souza <lamarque@kde.org>
    SPDX-FileCopyrightText: 2013 Lukas Tinkl <ltinkl@redhat.com>
    SPDX-FileCopyrightText: 2013-2015 Jan Grulich <jgrulich@redhat.com>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef MODEMMANAGERQT_SIM_H
#define MODEMMANAGERQT_SIM_H

#include <modemmanagerqt_export.h>

#include <QObject>
#include <QSharedPointer>
#include <QDBusPendingReply>

namespace ModemManager
{

class SimPrivate;

/**
 * @brief The Sim class
 *
 * The SIM class handles communication with SIM, USIM, and RUIM (CDMA SIM) cards.
 */
class MODEMMANAGERQT_EXPORT Sim : public QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(Sim)

public:
    typedef QSharedPointer<Sim> Ptr;
    typedef QList<Ptr> List;

    explicit Sim(const QString &path, QObject *parent = nullptr);
    ~Sim();

    /**
     * @return An obfuscated SIM identifier based on the IMSI or the ICCID.
     *
     * This may be available before the PIN has been entered depending on the device itself.
     */
    QString simIdentifier() const;

    /**
     * @return The IMSI of the SIM card, if any.
     */
    QString imsi() const;

    /**
     * @return The ID of the network operator, as given by the SIM card, if known.
     */
    QString operatorIdentifier() const;

    /**
     * @return The name of the network operator, as given by the SIM card, if known.
     */
    QString operatorName() const;

    /**
     * Send the PIN to unlock the SIM card.
     * @param pin A string containing the PIN code.
     */
    QDBusPendingReply<> sendPin(const QString &pin);

    /**
     * Send the PUK and a new PIN to unlock the SIM card.
     * @param puk A string containing the PUK code.
     * @param pin A string containing the PIN code.
     */
    QDBusPendingReply<> sendPuk(const QString &puk, const QString &pin);

    /**
     * Enable or disable the PIN checking.
     * @param pin A string containing the PIN code.
     * @param enabled TRUE to enable PIN checking, FALSE otherwise.
     */
    QDBusPendingReply<> enablePin(const QString &pin, bool enabled);

    /**
     * Change the PIN code.
     * @param oldPin A string containing the current PIN code.
     * @param newPin A string containing the new PIN code.
     */
    QDBusPendingReply<> changePin(const QString &oldPin, const QString &newPin);

    QString uni() const;

    /**
     * Sets the timeout in milliseconds for all async method DBus calls.
     * -1 means the default DBus timeout (usually 25 seconds).
     */
    void setTimeout(int timeout);

    /**
     * Returns the current value of the DBus timeout in milliseconds.
     * -1 means the default DBus timeout (usually 25 seconds).
     */
    int timeout() const;

Q_SIGNALS:
    void simIdentifierChanged(const QString &identifier);
    void imsiChanged(const QString &imsi);
    void operatorIdentifierChanged(const QString &identifier);
    void operatorNameChanged(const QString &name);

private:
    SimPrivate *const d_ptr;
};
} // namespace ModemManager

#endif
