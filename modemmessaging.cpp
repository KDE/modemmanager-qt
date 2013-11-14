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
    d->messageList = d->modemMessagingIface.List();

    connect(&d->modemMessagingIface, SIGNAL(Added(QDBusObjectPath,bool)), SIGNAL(messageAdded(QDBusObjectPath,bool)));
    connect(&d->modemMessagingIface, SIGNAL(Added(QDBusObjectPath,bool)), this, SLOT(onMessageAdded(QDBusObjectPath,bool)));
    connect(&d->modemMessagingIface, SIGNAL(Deleted(QDBusObjectPath)), SIGNAL(messageDeleted(QDBusObjectPath)));
    connect(&d->modemMessagingIface, SIGNAL(Deleted(QDBusObjectPath)), this, SLOT(onMessageDeleted(QDBusObjectPath)));

    QDBusConnection::systemBus().connect(MM_DBUS_SERVICE, path, DBUS_INTERFACE_PROPS, "PropertiesChanged", this,
                                         SLOT(onPropertiesChanged(QString,QVariantMap,QStringList)));
}

ModemManager::ModemMessaging::~ModemMessaging()
{
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
    d->messageList.append(path);
}

void ModemManager::ModemMessaging::onMessageDeleted(const QDBusObjectPath &path)
{
    Q_D(ModemMessaging);
    d->messageList.removeOne(path);
}

QList<MMSmsStorage> ModemManager::ModemMessaging::supportedStorages() const
{
    Q_D(const ModemMessaging);
    QList<MMSmsStorage> result;

    foreach (uint storage, d->modemMessagingIface.supportedStorages()) {
        result.append((MMSmsStorage)storage);
    }

    return result;
}

MMSmsStorage ModemManager::ModemMessaging::defaultStorage() const
{
    Q_D(const ModemMessaging);
    return d->defaultStorage;
}

QList<QDBusObjectPath> ModemManager::ModemMessaging::listMessages()
{
    Q_D(ModemMessaging);
    return d->messageList;
}

QDBusPendingReply<QDBusObjectPath> ModemManager::ModemMessaging::createMessage(const QVariantMap &properties)
{
    Q_D(ModemMessaging);
    return d->modemMessagingIface.Create(properties);
}

QDBusPendingReply<> ModemManager::ModemMessaging::deleteMessage(const QDBusObjectPath &path)
{
    Q_D(ModemMessaging);
    return d->modemMessagingIface.Delete(path);
}
