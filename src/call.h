/*
    Copyright 2018 Aleksander Morgado <aleksander@aleksander.es>

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

#ifndef MODEMMANAGERQT_CALL_H
#define MODEMMANAGERQT_CALL_H

#include <modemmanagerqt_export.h>

#include <QObject>
#include <QSharedPointer>
#include <QDBusPendingReply>

#include "generictypes.h"

namespace ModemManager
{

class CallPrivate;

/**
* Provides an interface to manipulate and control a call
*
* Note: MMCallState, MMCallStateReason and MMCallDirection enums are defined in <ModemManager/ModemManager-enums.h>
* See http://www.freedesktop.org/software/ModemManager/api/latest/ModemManager-Flags-and-Enumerations.html
*/
class MODEMMANAGERQT_EXPORT Call : public QObject
{
Q_OBJECT
Q_DECLARE_PRIVATE(Call)

public:
    typedef QSharedPointer<Call> Ptr;
    typedef QList<Ptr> List;

    explicit Call(const QString &path, QObject *parent = nullptr);
    ~Call();

    QString uni() const;

    /**
    * Start a call
    */
    QDBusPendingReply<> start();

    /**
    * Accept a call
    */
    QDBusPendingReply<> accept();

    /**
    * Hangup a call
    */
    QDBusPendingReply<> hangup();

    /**
    * Send DTMF
    */
    QDBusPendingReply<> sendDtmf(const QString &dtmf);

    /**
    * This method returns the state of the call
    */
    MMCallState state() const;

    /**
    * This method returns the reason for the call state change
    */
    MMCallStateReason stateReason() const;

    /**
    * This method returns the direction of the call
    */
    MMCallDirection direction() const;

    /**
    * This method returns the remote phone number
    */
    QString number() const;

Q_SIGNALS:
    void stateChanged(MMCallState oldState, MMCallState newState, MMCallStateReason reason);
    void numberChanged(const QString &number);
    void dtmfReceived(const QString &dtmf);

private:
    CallPrivate *const d_ptr;
};

} // namespace ModemManager

#endif
