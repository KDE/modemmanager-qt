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

#include "modemgsmsmsinterface.h"
#include "modemgsmsmsinterface_p.h"
#include "manager.h"

ModemSmsInterfacePrivate::ModemSmsInterfacePrivate(const QString &path, QObject *owner)
    : ModemInterfacePrivate(path, owner),
      smsIface(MM_DBUS_SERVICE, path, QDBusConnection::systemBus(), this)
{
}

ModemManager::ModemSmsInterface::ModemSmsInterface(const QString & path, QObject * parent)
    : ModemInterface(*new ModemSmsInterfacePrivate(path, this), parent)
{
}

ModemManager::ModemSmsInterface::~ModemSmsInterface()
{
}

MMSmsState ModemManager::ModemSmsInterface::state() const
{
    Q_D(const ModemSmsInterface);
    return (MMSmsState)d->smsIface.state();
}

MMSmsPduType ModemManager::ModemSmsInterface::pduType() const
{
    Q_D(const ModemSmsInterface);
    return (MMSmsPduType)d->smsIface.pduType();
}

QString ModemManager::ModemSmsInterface::number() const
{
    Q_D(const ModemSmsInterface);
    return d->smsIface.number();
}

QString ModemManager::ModemSmsInterface::text() const
{
    Q_D(const ModemSmsInterface);
    return d->smsIface.text();
}

QByteArray ModemManager::ModemSmsInterface::data() const
{
    Q_D(const ModemSmsInterface);
    return d->smsIface.data();
}

QString ModemManager::ModemSmsInterface::smsc() const
{
    Q_D(const ModemSmsInterface);
    return d->smsIface.sMSC();
}

ValidityPair ModemManager::ModemSmsInterface::validity() const
{
    Q_D(const ModemSmsInterface);
    return d->smsIface.validity();
}

int ModemManager::ModemSmsInterface::smsClass() const
{
    Q_D(const ModemSmsInterface);
    return d->smsIface.smsClass();
}

bool ModemManager::ModemSmsInterface::deliveryReportRequest() const
{
    Q_D(const ModemSmsInterface);
    return d->smsIface.deliveryReportRequest();
}

uint ModemManager::ModemSmsInterface::messageReference() const
{
    Q_D(const ModemSmsInterface);
    return d->smsIface.messageReference();
}

QDateTime ModemManager::ModemSmsInterface::timestamp() const
{
    Q_D(const ModemSmsInterface);
    return QDateTime::fromString(d->smsIface.timestamp(), Qt::ISODate);
}

QDateTime ModemManager::ModemSmsInterface::dischargeTimestamp() const
{
    Q_D(const ModemSmsInterface);
    return QDateTime::fromString(d->smsIface.dischargeTimestamp(), Qt::ISODate);
}

MMSmsDeliveryState ModemManager::ModemSmsInterface::deliveryState() const
{
    Q_D(const ModemSmsInterface);
    return (MMSmsDeliveryState)d->smsIface.deliveryState();
}

MMSmsStorage ModemManager::ModemSmsInterface::storage() const
{
    Q_D(const ModemSmsInterface);
    return (MMSmsStorage)d->smsIface.storage();
}

void ModemManager::ModemSmsInterface::send()
{
    Q_D(ModemSmsInterface);
    d->smsIface.Send();
}

void ModemManager::ModemSmsInterface::store(MMSmsStorage storage)
{
    Q_D(ModemSmsInterface);
    d->smsIface.Store(storage);
}
