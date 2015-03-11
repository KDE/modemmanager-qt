/*
    Copyright 2013 Anant Kamath <kamathanant@gmail.com>
    Copyright 2013 Lukas Tinkl <ltinkl@redhat.com>
    Copyright 2013-2015 Jan Grulich <jgrulich@redhat.com>

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

#include "modemmessaging.h"
#include "modemmessaging_p.h"

#ifdef MMQT_STATIC
#include "dbus/fakedbus.h"
#else
#include "dbus/dbus.h"
#endif
#include "mmdebug.h"

#include "sms.h"
#include <ModemManager/ModemManager.h>

ModemManager::ModemMessagingPrivate::ModemMessagingPrivate(const QString &path, ModemMessaging *q)
    : InterfacePrivate(path, q)
#ifdef MMQT_STATIC
    , modemMessagingIface(MMQT_DBUS_SERVICE, path, QDBusConnection::sessionBus())
#else
    , modemMessagingIface(MMQT_DBUS_SERVICE, path, QDBusConnection::systemBus())
#endif
    , q_ptr(q)
{
    if (modemMessagingIface.isValid()) {
        QList<MMSmsStorage> storages;
        Q_FOREACH (uint storage, modemMessagingIface.supportedStorages()) {
            storages.append((MMSmsStorage)storage);
        }
        supportedStorages = storages;
        defaultStorage = (MMSmsStorage) modemMessagingIface.defaultStorage();

        connect(&modemMessagingIface, &OrgFreedesktopModemManager1ModemMessagingInterface::Added, this, &ModemMessagingPrivate::onMessageAdded);
        connect(&modemMessagingIface, &OrgFreedesktopModemManager1ModemMessagingInterface::Deleted, this, &ModemMessagingPrivate::onMessageDeleted);
    }
}

ModemManager::ModemMessaging::ModemMessaging(const QString &path, QObject *parent)
    : Interface(*new ModemMessagingPrivate(path, this), parent)
{
    Q_D(ModemMessaging);
#ifdef MMQT_STATIC
    QDBusConnection::sessionBus().connect(MMQT_DBUS_SERVICE, path, DBUS_INTERFACE_PROPS, QStringLiteral("PropertiesChanged"), this,
                                         SLOT(onPropertiesChanged(QString,QVariantMap,QStringList)));
#else
    QDBusConnection::systemBus().connect(MMQT_DBUS_SERVICE, path, DBUS_INTERFACE_PROPS, QStringLiteral("PropertiesChanged"), this,
                                         SLOT(onPropertiesChanged(QString,QVariantMap,QStringList)));
#endif
    QList <QDBusObjectPath> messages = d->modemMessagingIface.messages();
    Q_FOREACH (const QDBusObjectPath &op, messages) {
        const QString path = op.path();
        d->messageList.insert(path, ModemManager::Sms::Ptr());
        Q_EMIT messageAdded(path, false);
    }
}

ModemManager::ModemMessaging::~ModemMessaging()
{
}

ModemManager::Sms::Ptr ModemManager::ModemMessagingPrivate::findMessage(const QString &uni)
{
    ModemManager::Sms::Ptr sms;
    if (messageList.contains(uni)) {
        if (messageList.value(uni)) {
            sms = messageList.value(uni);
        } else {
            sms = ModemManager::Sms::Ptr(new ModemManager::Sms(uni), &QObject::deleteLater);
            messageList[uni] = sms;
        }
    }
    return sms;
}

ModemManager::Sms::List ModemManager::ModemMessagingPrivate::ModemMessagingPrivate::messages()
{
    ModemManager::Sms::List list;

    QMap<QString, ModemManager::Sms::Ptr>::const_iterator i;
    for (i = messageList.constBegin(); i != messageList.constEnd(); ++i) {
        ModemManager::Sms::Ptr sms = findMessage(i.key());
        if (sms) {
            list.append(sms);
        } else {
            qCWarning(MMQT) << "warning: null message for" << i.key();
        }
    }

    return list;
}

void ModemManager::ModemMessagingPrivate::onPropertiesChanged(const QString &interfaceName, const QVariantMap &changedProperties, const QStringList &invalidatedProperties)
{
    Q_UNUSED(invalidatedProperties);
    Q_Q(ModemMessaging);

    if (interfaceName == QLatin1String(MMQT_DBUS_INTERFACE_MODEM_MESSAGING)) {
        QVariantMap::const_iterator it = changedProperties.constFind(QLatin1String(MM_MODEM_MESSAGING_PROPERTY_SUPPORTEDSTORAGES));
        if (it != changedProperties.constEnd()) {
            QList<MMSmsStorage> storages;
            Q_FOREACH (uint storage, it->value<UIntList>()) {
                storages.append((MMSmsStorage)storage);
            }
            supportedStorages = storages;
        }
        it = changedProperties.constFind(QLatin1String(MM_MODEM_MESSAGING_PROPERTY_DEFAULTSTORAGE));
        if (it != changedProperties.constEnd()) {
            defaultStorage = (MMSmsStorage) it->toInt();
        }
        // MESSAGES? should be handled by Added/Deleted signals
    }
}

void ModemManager::ModemMessagingPrivate::onMessageAdded(const QDBusObjectPath &path, bool received)
{
    Q_Q(ModemMessaging);
    messageList.insert(path.path(), ModemManager::Sms::Ptr());
    Q_EMIT q->messageAdded(path.path(), received);
}

void ModemManager::ModemMessagingPrivate::onMessageDeleted(const QDBusObjectPath &path)
{
    Q_Q(ModemMessaging);
    messageList.remove(path.path());
    Q_EMIT q->messageDeleted(path.path());
}

QList<MMSmsStorage> ModemManager::ModemMessaging::supportedStorages() const
{
    Q_D(const ModemMessaging);
    return d->supportedStorages;
}

MMSmsStorage ModemManager::ModemMessaging::defaultStorage() const
{
    Q_D(const ModemMessaging);
    return d->defaultStorage;
}

ModemManager::Sms::List ModemManager::ModemMessaging::messages()
{
    Q_D(ModemMessaging);
    return d->messages();
}

QDBusPendingReply<QDBusObjectPath> ModemManager::ModemMessaging::createMessage(const Message &message)
{
    QVariantMap map;
    map.insert("number", message.number);
    map.insert("text", message.text);
    map.insert("data", message.data);

    return createMessage(map);
}

QDBusPendingReply<QDBusObjectPath> ModemManager::ModemMessaging::createMessage(const QVariantMap& message)
{
    Q_D(ModemMessaging);

    if (!message.contains("number") || (!message.contains("text") && !message.contains("data"))) {
        qCDebug(MMQT) << "Unable to create message, missing some property";
        return QDBusPendingReply<QDBusObjectPath>();
    }

    return d->modemMessagingIface.Create(message);
}

QDBusPendingReply<void> ModemManager::ModemMessaging::deleteMessage(const QString &uni)
{
    Q_D(ModemMessaging);
    return d->modemMessagingIface.Delete(QDBusObjectPath(uni));
}

ModemManager::Sms::Ptr ModemManager::ModemMessaging::findMessage(const QString& uni)
{
    Q_D(ModemMessaging);
    return d->findMessage(uni);
}
