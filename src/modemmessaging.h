/*
    Copyright 2013 Anant Kamath <kamathanant@gmail.com>
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

#ifndef MODEMMANAGERQT_MODEMMESSAGING_H
#define MODEMMANAGERQT_MODEMMESSAGING_H

#include "ModemManagerQt-export.h"

#include <QObject>
#include <QSharedPointer>

#include "sms.h"
#include "generictypes.h"
#include "interface.h"

class ModemMessagingPrivate;

namespace ModemManager
{
/**
 * @brief The ModemMessaging class
 *
 * The Messaging interface handles sending SMS messages and notification of new incoming messages.
 */
class MODEMMANAGERQT_EXPORT ModemMessaging : public Interface
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(ModemMessaging)

public:
    struct Message {
        QString number;
        QString text;
        QByteArray data;
    };

    typedef QSharedPointer<ModemMessaging> Ptr;
    typedef QList<Ptr> List;

    explicit ModemMessaging(const QString &path, QObject *parent = 0);
    ~ModemMessaging();

    /**
     * @return A list of MMSmsStorage values, specifying the storages supported by this
     * modem for storing and receiving SMS.
     */
    QList<MMSmsStorage> supportedStorages() const;

    /**
     * @return A MMSmsStorage value, specifying the storage to be used when receiving or storing SMS.
     */
    MMSmsStorage defaultStorage() const;

    /**
     * Retrieve all SMS messages.
     *
     * This method should only be used once and subsequent information retrieved
     * either by listening for the messageAdded() signal, or by
     * querying the specific SMS object of interest using findMessage()
     */
    ModemManager::Sms::List messages();

    /**
     * Creates a new message object.
     * @param message Message structure with the 'number' and either 'text' or 'data' properties
     */
    QString createMessage(const Message &message);
    /**
     * Creates a new message object.
     * @param message QVariantMap containing message properties
     * The 'number' and either 'text' or 'data' properties are mandatory, others are optional.
     */
    QString createMessage(const QVariantMap &message);

    /**
     * Delete an SMS message.
     *
     * @param uni path to the Sms object
     */
    void deleteMessage(const QString &uni);

    /**
     * @param uni path to the Sms object
     * @return pointer to the found Sms (may be null if not found)
     */
    ModemManager::Sms::Ptr findMessage(const QString &uni);

private Q_SLOTS:
    void onPropertiesChanged(const QString &interface, const QVariantMap &changedProperties, const QStringList &invalidatedProps);
    void onMessageAdded(const QDBusObjectPath &path, bool received);
    void onMessageDeleted(const QDBusObjectPath &path);

Q_SIGNALS:
    /**
     * Emitted when any part of a new SMS has been received or added (but not
     * for subsequent parts, if any). For messages received from the network,
     * not all parts may have been received and the message may not be
     * complete.
     *
     * Check the 'State' property to determine if the message is complete.
     *
     * @param uni path to the Sms object
     * @param received @p true if the message was received from the network, as opposed to being added locally.
     */
    void messageAdded(const QString &uni, bool received);

    /**
     * Emitted when a message has been deleted.
     * @param uni path to the Sms object
     */
    void messageDeleted(const QString &uni);
};

} // namespace ModemManager

#endif
