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

#include "modemmessaging.h"
#include "modemmessaging_p.h"

#include "dbus/dbus.h"
#include "mmdebug.h"

#include "sms.h"
#include <ModemManager/ModemManager.h>

ModemMessagingPrivate::ModemMessagingPrivate(const QString &path)
    : InterfacePrivate(path)
    , modemMessagingIface(MM_DBUS_SERVICE, path, QDBusConnection::systemBus())
{
}

ModemManager::ModemMessaging::ModemMessaging(const QString &path, QObject *parent)
    : Interface(*new ModemMessagingPrivate(path), parent)
{
    Q_D(ModemMessaging);

    QList<MMSmsStorage> storages;
    foreach (uint storage, d->modemMessagingIface.supportedStorages()) {
        storages.append((MMSmsStorage)storage);
    }
    d->supportedStorages = storages;
    d->defaultStorage = (MMSmsStorage) d->modemMessagingIface.defaultStorage();

    connect(&d->modemMessagingIface, SIGNAL(Added(QDBusObjectPath,bool)), this, SLOT(onMessageAdded(QDBusObjectPath,bool)));
    connect(&d->modemMessagingIface, SIGNAL(Deleted(QDBusObjectPath)), this, SLOT(onMessageDeleted(QDBusObjectPath)));

    QDBusConnection::systemBus().connect(MM_DBUS_SERVICE, path, DBUS_INTERFACE_PROPS, "PropertiesChanged", this,
                                         SLOT(onPropertiesChanged(QString,QVariantMap,QStringList)));

    // FIXME We have to do it this way for MM 1.0, in MM 1.2 will be MessageList as a property
    QDBusReply< QList <QDBusObjectPath> > messageList = d->modemMessagingIface.List();
    if (messageList.isValid()) {
        mmDebug() << "Message list";
        QList <QDBusObjectPath> messages = messageList.value();
        foreach (const QDBusObjectPath &op, messages) {
            d->messageList.insert(op.path(), ModemManager::Sms::Ptr());
            emit messageAdded(op.path(), false);
            mmDebug() << "  " << op.path();
        }
    }
}

ModemManager::ModemMessaging::~ModemMessaging()
{
}

ModemManager::Sms::Ptr ModemMessagingPrivate::findMessage(const QString &uni)
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

ModemManager::Sms::List ModemMessagingPrivate::ModemMessagingPrivate::messages()
{
    ModemManager::Sms::List list;

    QMap<QString, ModemManager::Sms::Ptr>::const_iterator i;
    for (i = messageList.constBegin(); i != messageList.constEnd(); ++i) {
        ModemManager::Sms::Ptr sms = findMessage(i.key());
        if (!sms.isNull()) {
            list.append(sms);
        } else {
            qWarning() << "warning: null message for" << i.key();
        }
    }

    return list;
}

void ModemManager::ModemMessaging::onPropertiesChanged(const QString &interfaceName, const QVariantMap &changedProperties, const QStringList &invalidatedProperties)
{
    if (interfaceName == QString(MM_DBUS_INTERFACE_MODEM_MESSAGING)) {
        QLatin1String supportedStorages(MM_MODEM_MESSAGING_PROPERTY_SUPPORTEDSTORAGES);
        QLatin1String defaultStorage(MM_MODEM_MESSAGING_PROPERTY_DEFAULTSTORAGE);

        Q_D(ModemMessaging);

        QVariantMap::const_iterator it = changedProperties.constFind(supportedStorages);
        if (it != changedProperties.constEnd()) {
            QList<MMSmsStorage> storages;
            foreach (uint storage, it->value<UIntList>()) {
                storages.append((MMSmsStorage)storage);
            }
            d->supportedStorages = storages;
        }
        it = changedProperties.constFind(defaultStorage);
        if (it != changedProperties.constEnd()) {
            d->defaultStorage = (MMSmsStorage) it->toInt();
        }
    }
}

void ModemManager::ModemMessaging::onMessageAdded(const QDBusObjectPath &path, bool received)
{
    Q_D(ModemMessaging);
    d->messageList.insert(path.path(), ModemManager::Sms::Ptr());
    emit messageAdded(path.path(), received);
}

void ModemManager::ModemMessaging::onMessageDeleted(const QDBusObjectPath &path)
{
    Q_D(ModemMessaging);
    d->messageList.remove(path.path());
    emit messageDeleted(path.path());
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

QString ModemManager::ModemMessaging::createMessage(const Message &message)
{
    Q_D(ModemMessaging);

    QVariantMap map;
    map.insert("number", message.number);
    map.insert("text", message.text);
    map.insert("data", message.data);

    return createMessage(map);
}

QString ModemManager::ModemMessaging::createMessage(const QVariantMap& message)
{
    Q_D(ModemMessaging);

    if (!message.contains("number") || (!message.contains("text") && !message.contains("data"))) {
        mmDebug() << "Unable to create message, missing some property";
        return QString();
    }

    QDBusReply<QDBusObjectPath> msgPath = d->modemMessagingIface.Create(message);
    if (msgPath.isValid()) {
        const QString path = msgPath.value().path();
        ModemManager::Sms::Ptr msg = ModemManager::Sms::Ptr(new ModemManager::Sms(path), &QObject::deleteLater);
        d->messageList.insert(path, msg);
        return path;
    }

    return QString();
}

void ModemManager::ModemMessaging::deleteMessage(const QString &uni)
{
    Q_D(ModemMessaging);
    d->modemMessagingIface.Delete(QDBusObjectPath(uni));
}

ModemManager::Sms::Ptr ModemManager::ModemMessaging::findMessage(const QString& uni)
{
    Q_D(ModemMessaging);
    return d->findMessage(uni);
}
