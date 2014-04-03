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

#ifndef MODEMMANAGERQT_MODEM3GPPUSSD_H
#define MODEMMANAGERQT_MODEM3GPPUSSD_H

#include "ModemManagerQt-export.h"

#include <QObject>
#include <QSharedPointer>

#include "generictypes.h"
#include "interface.h"

class Modem3gppUssdPrivate;

namespace ModemManager
{
/**
 * @brief The Modem3gppUssd class
 *
 * This class provides access to actions based on the USSD protocol.
 */
class MODEMMANAGERQT_EXPORT Modem3gppUssd : public Interface
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(Modem3gppUssd)

public:
    typedef QSharedPointer<Modem3gppUssd> Ptr;
    typedef QList<Ptr> List;

    explicit Modem3gppUssd(const QString &path, QObject *parent = 0);
    ~Modem3gppUssd();

    /**
     * Sends a USSD @p command string to the network initiating a USSD session.
     *
     * When the request is handled by the network, the method returns the
     * response or an appropriate error. The network may be awaiting further
     * response from the ME after returning from this method and no new command
     * can be initiated until this one is cancelled or ended.
     */
    QString initiate(const QString &command);

    /**
     * Respond to a USSD request that is either initiated by the mobile network,
     * or that is awaiting further input after initiate() was called.
     */
    QString respond(const QString &response);

    /**
     * Cancel an ongoing USSD session, either mobile or network initiated.
     */
    void cancel();

    /**
     * @return the state of any ongoing USSD session
     */
    MMModem3gppUssdSessionState state() const;

    /**
     * @return any network-initiated request to which no USSD response is required
     *
     * When no USSD session is active, or when there is no network- initiated request, this property will be an empty string.
     */
    QString networkNotification() const;

    /**
     * @return any pending network-initiated request for a response. Client
     * should call respond() with the appropriate response to this request.
     *
     * When no USSD session is active, or when there is no pending
     * network-initiated request, this property will be an empty string.
     */
    QString networkRequest() const;

Q_SIGNALS:
    void stateChanged(MMModem3gppUssdSessionState state);
    void networkNotificationChanged(const QString &networkNotification);
    void networkRequestChanged(const QString &networkRequest);

private Q_SLOTS:
    void onPropertiesChanged(const QString &interface, const QVariantMap &properties, const QStringList &invalidatedProps);
};

} // namespace ModemManager

#endif
