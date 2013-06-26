/*
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
#include "manager.h"
#include "mmdebug.h"

ModemMessagingInterfacePrivate::ModemMessagingInterfacePrivate(const QString &path, QObject *owner)
    : ModemInterfacePrivate(path, owner),
      modemMessagingIface(MM_DBUS_SERVICE, path, QDBusConnection::systemBus(), this)
{
}

ModemManager::ModemMessagingInterface::ModemMessagingInterface(const QString & path, QObject * parent)
    : ModemInterface(*new ModemMessagingInterfacePrivate(path, this), parent)
{
    Q_D(ModemMessagingInterface);
    connect(&d->modemMessagingIface, SIGNAL(Added(QDBusObjectPath,bool)), SIGNAL(messageAdded(QDBusObjectPath,bool)));
    connect(&d->modemMessagingIface, SIGNAL(Deleted(QDBusObjectPath)), SIGNAL(messageDeleted(QDBusObjectPath)));
}

ModemManager::ModemMessagingInterface::~ModemMessagingInterface()
{
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
    return (MMSmsStorage)d->modemMessagingIface.defaultStorage();
}

QList<QDBusObjectPath> ModemManager::ModemMessagingInterface::listMessages()
{
    Q_D(ModemMessagingInterface);
    return d->modemMessagingIface.List();
}

void ModemManager::ModemMessagingInterface::createMessage(const QVariantMap &properties)
{
    Q_D(ModemMessagingInterface);
    d->modemMessagingIface.Create(properties);
}

void ModemManager::ModemMessagingInterface::deleteMessage(const QDBusObjectPath &path)
{
    Q_D(ModemMessagingInterface);
    d->modemMessagingIface.Delete(path);
}
