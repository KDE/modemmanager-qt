/*
Copyright 2013 Anant Kamath <kamathanant@gmail.com>
Copyright 2013 Lukas Tinkl <ltinkl@redhat.com>

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

#ifndef MODEMMANAGER_MODEMMESSAGING_H
#define MODEMMANAGER_MODEMMESSAGING_H

#include "ModemManagerQt-export.h"

#include <QObject>
#include <QSharedPointer>

#include "sms.h"
#include "generic-types.h"
#include "interface.h"

class ModemMessagingPrivate;

namespace ModemManager
{
class MODEMMANAGERQT_EXPORT ModemMessaging : public Interface
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(ModemMessaging)

public:
    typedef QSharedPointer<ModemMessaging> Ptr;
    typedef QList<Ptr> List;

    explicit ModemMessaging(const QString &path, QObject *parent = 0);
    ~ModemMessaging();

    // properties
    QList<MMSmsStorage> supportedStorages() const;
    MMSmsStorage defaultStorage() const;

    //methods
    /**
     * List all SMS messages
     */
    ModemManager::Sms::List messages();

    /**
     * Create a new SMS message
     */
    QDBusPendingReply<QDBusObjectPath> createMessage(const QVariantMap &properties);

    /**
     * Delete an SMS message
     */
    void deleteMessage(const QString &uni);

    ModemManager::Sms::Ptr findMessage(const QString &uni);

private Q_SLOTS:
    void onPropertiesChanged(const QString &interface, const QVariantMap &changedProperties, const QStringList &invalidatedProps);
    void onMessageAdded(const QDBusObjectPath &path, bool received);
    void onMessageDeleted(const QDBusObjectPath &path);

Q_SIGNALS:
    void messageAdded(const QString &uni, bool received);
    void messageDeleted(const QString &uni);
};

} // namespace ModemManager

#endif
