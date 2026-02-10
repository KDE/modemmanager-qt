/*
    SPDX-FileCopyrightText: 2018 Aleksander Morgado <aleksander@aleksander.es>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef MODEMMANAGERQT_CALL_H
#define MODEMMANAGERQT_CALL_H

#include <modemmanagerqt_export.h>

#include <QDBusPendingReply>
#include <QObject>
#include <QSharedPointer>

#include "generictypes.h"

namespace ModemManager
{
class CallPrivate;

/*!
 * \class ModemManager::Call
 *
 * \inheaderfile ModemManagerQt/Call
 *
 * \inmodule ModemManagerQt
 *
 * \brief Provides an interface to manipulate and control a call
 *
 * \note MMCallState, MMCallStateReason and MMCallDirection enums are defined in <ModemManager/ModemManager-enums.h>
 * See http://www.freedesktop.org/software/ModemManager/api/latest/ModemManager-Flags-and-Enumerations.html .
 */
class MODEMMANAGERQT_EXPORT Call : public QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(Call)

public:
    /*!
     * \typedef ModemManager::Call::Ptr
     */
    typedef QSharedPointer<Call> Ptr;
    /*!
     * \typedef ModemManager::Call::List
     */
    typedef QList<Ptr> List;

    /*!
     */
    explicit Call(const QString &path, QObject *parent = nullptr);
    ~Call() override;

    /*!
     */
    QString uni() const;

    /*!
     * If the outgoing call has not yet been started, start it.
     *
     * Applicable only if state is \c MM_CALL_STATE_UNKNOWN and direction is
     * \c MM_CALL_DIRECTION_OUTGOING.
     */
    QDBusPendingReply<> start();

    /*!
     * Accept incoming call (answer).
     *
     * Applicable only if state is \c MM_CALL_STATE_RINGING_IN and direction is
     * \c MM_CALL_DIRECTION_INCOMING.
     */
    QDBusPendingReply<> accept();

    /*!
     * Deflect an incoming or waiting call to a new \a number. The call will be
     * considered terminated once the deflection is performed.
     *
     * Applicable only if state is \c MM_CALL_STATE_RINGING_IN or
     * \c MM_CALL_STATE_WAITING and direction is \c MM_CALL_DIRECTION_INCOMING.
     *
     * \since 6.24.0
     */
    QDBusPendingReply<> deflect(const QString &number);

    /*!
     * Hangup the active call.
     *
     * Applicable only if state is \c MM_CALL_STATE_UNKNOWN.
     */
    QDBusPendingReply<> hangup();

    /*!
     * Join the currently held call into a single multiparty call with another
     * already active call.
     *
     * The calls will be flagged with the Multiparty property while they are
     * part of the multiparty call.
     *
     * Applicable only if state is \c MM_CALL_STATE_HELD.
     *
     * \since 6.24.0
     */
    QDBusPendingReply<> joinMultiparty();

    /*!
     * If this call is part of an ongoing multiparty call, detach it from the
     * multiparty call, put the multiparty call on hold, and activate this one
     * alone. This operation makes this call private again between both ends of
     * the call.
     *
     * Applicable only if state is \c MM_CALL_STATE_ACTIVE or
     * \c MM_CALL_STATE_HELD and the call is a multiparty call.
     *
     * \since 6.24.0
     */
    QDBusPendingReply<> leaveMultiparty();

    /*!
     * Send a DTMF tone (Dual Tone Multi-Frequency) (only on supported modems).
     *
     * \a dtmf is the DTMF tone identifier [0-9A-D*#].
     *
     * Applicable only if state is \c MM_CALL_STATE_ACTIVE.
     */
    QDBusPendingReply<> sendDtmf(const QString &dtmf);

    /*!
     * Returns the current MMCallState value describing the state of the call.
     */
    MMCallState state() const;

    /*!
     * Returns the MMCallStateReason value describing why the state changed.
     */
    MMCallStateReason stateReason() const;

    /*!
     * Returns the MMCallDirection value describing the direction of the call.
     */
    MMCallDirection direction() const;

    /*!
     * Returns the remote phone number.
     */
    QString number() const;

    /*!
     * Returns whether the call is currently part of a multiparty conference call.
     *
     * \since 6.24.0
     */
    bool isMultiparty() const;

    /*!
     * If call audio is routed via the host, returns the name of the kernel
     * device that provides the audio. For example, with certain Huawei USB
     * modems, this might be "ttyUSB2" indicating audio is available via
     * ttyUSB2 in the format described by the audioFormat property.
     *
     * \since 6.24.0
     */
    QString audioPort() const;

    /*!
     * If call audio is routed via the host, returns a description of the audio
     * format supported by the audio port.
     *
     * The returned map may include the following items:
     * \li \c "encoding" - The audio encoding format (e.g. "pcm").
     * \li \c "resolution" - The sampling precision and encoding format (e.g. "s16le").
     * \li \c "rate" - The sampling rate as an unsigned integer (e.g. 8000).
     *
     * \since 6.24.0
     */
    QVariantMap audioFormat() const;

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
    void stateChanged(MMCallState oldState, MMCallState newState, MMCallStateReason reason);
    /*!
     */
    void numberChanged(const QString &number);
    /*!
     * \since 6.24.0
     */
    void multipartyChanged(bool multiparty);
    /*!
     * \since 6.24.0
     */
    void audioPortChanged(const QString &audioPort);
    /*!
     * \since 6.24.0
     */
    void audioFormatChanged(const QVariantMap &audioFormat);
    /*!
     */
    void dtmfReceived(const QString &dtmf);

private:
    CallPrivate *const d_ptr;
};

} // namespace ModemManager

#endif
