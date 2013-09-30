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

#include "modemmessaginginterface.h"
#include "modemmessaginginterface_p.h"

#include "dbus/dbus.h"
#include "manager.h"
#include "mmdebug.h"

#include "smsinterface.h"
#include <ModemManager/ModemManager.h>

ModemMessagingInterfacePrivate::ModemMessagingInterfacePrivate(const QString &path, QObject *owner)
    : ModemInterfacePrivate(path, owner),
      modemMessagingIface(MM_DBUS_SERVICE, path, QDBusConnection::systemBus(), this)
{
}

ModemManager::ModemMessagingInterface::ModemMessagingInterface(const QString & path, QObject * parent)
    : ModemInterface(*new ModemMessagingInterfacePrivate(path, this), parent)
{
    Q_D(ModemMessagingInterface);

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

ModemManager::ModemMessagingInterface::~ModemMessagingInterface()
{
}

void ModemManager::ModemMessagingInterface::onPropertiesChanged(const QString &interfaceName, const QVariantMap &changedProperties, const QStringList &invalidatedProperties)
{
    if (interfaceName == QString(MM_DBUS_INTERFACE_MODEM_MESSAGING)) {
        QLatin1String supportedStorages(MM_MODEM_MESSAGING_PROPERTY_SUPPORTEDSTORAGES);
        QLatin1String defaultStorage(MM_MODEM_MESSAGING_PROPERTY_DEFAULTSTORAGE);

        Q_D(ModemMessagingInterface);

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

void ModemManager::ModemMessagingInterface::onMessageAdded(const QDBusObjectPath &path, bool received)
{
    Q_D(ModemMessagingInterface);
    d->messageList.append(path);
}

void ModemManager::ModemMessagingInterface::onMessageDeleted(const QDBusObjectPath &path)
{
    Q_D(ModemMessagingInterface);
    d->messageList.removeOne(path);
}

QList<MMSmsStorage> ModemManager::ModemMessagingInterface::supportedStorages() const
{
    Q_D(const ModemMessagingInterface);
    QList<MMSmsStorage> result;

    foreach (uint storage, d->modemMessagingIface.supportedStorages()) {
        result.append((MMSmsStorage)storage);
    }

    return result;
}

MMSmsStorage ModemManager::ModemMessagingInterface::defaultStorage() const
{
    Q_D(const ModemMessagingInterface);
    return d->defaultStorage;
}

QList<QDBusObjectPath> ModemManager::ModemMessagingInterface::listMessages()
{
    Q_D(ModemMessagingInterface);
    return d->messageList;
}

QDBusPendingReply<QDBusObjectPath> ModemManager::ModemMessagingInterface::createMessage(const QVariantMap &properties)
{
    Q_D(ModemMessagingInterface);
    return d->modemMessagingIface.Create(properties);
}

QDBusPendingReply<> ModemManager::ModemMessagingInterface::deleteMessage(const QDBusObjectPath &path)
{
    Q_D(ModemMessagingInterface);
    return d->modemMessagingIface.Delete(path);
}

ModemManager::SmsInterface::Ptr ModemManager::ModemMessagingInterface::createSmsInterface(const QDBusObjectPath& path)
{
    Q_D(ModemMessagingInterface);
    return SmsInterface::Ptr(new ModemManager::SmsInterface(path.path(), this));
}
