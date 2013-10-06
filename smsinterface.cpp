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

#include "smsinterface.h"
#include "smsinterface_p.h"

#include "dbus/dbus.h"

#include <ModemManager/ModemManager.h>

SmsInterfacePrivate::SmsInterfacePrivate(const QString &path)
    : smsIface(MM_DBUS_SERVICE, path, QDBusConnection::systemBus())
{
}

ModemManager::SmsInterface::SmsInterface(const QString &path, QObject *parent)
    : QObject(parent)
    , d_ptr(new SmsInterfacePrivate(path))
{
    Q_D(SmsInterface);

    d->dBusPath = path;
    d->state = (MMSmsState) d->smsIface.state();
    d->pduType = (MMSmsPduType) d->smsIface.pduType();
    d->number = d->smsIface.number();
    d->text = d->smsIface.text();
    d->smsc = d->smsIface.sMSC();
    d->data = d->smsIface.data();
    d->validity = d->smsIface.validity();
    d->smsClass = d->smsIface.smsClass();
    d->deliveryReportRequest = d->smsIface.deliveryReportRequest();
    d->messageReference = d->smsIface.messageReference();
    d->timestamp =  QDateTime::fromString(d->smsIface.timestamp(), Qt::ISODate);
    d->dischargeTimestamp = QDateTime::fromString(d->smsIface.dischargeTimestamp(), Qt::ISODate);
    d->deliveryState = (MMSmsDeliveryState) d->smsIface.deliveryState();
    d->storage =  (MMSmsStorage) d->smsIface.storage();

    QDBusConnection::systemBus().connect(MM_DBUS_SERVICE, path, DBUS_INTERFACE_PROPS, "PropertiesChanged", this,
                                         SLOT(onPropertiesChanged(QString,QVariantMap,QStringList)));
}

ModemManager::SmsInterface::~SmsInterface()
{
    delete d_ptr;
}

void ModemManager::SmsInterface::onPropertiesChanged(const QString &interfaceName, const QVariantMap &changedProperties, const QStringList &invalidatedProperties)
{
    if (interfaceName == QString(MM_DBUS_INTERFACE_SMS)) {
        QLatin1String state(MM_SMS_PROPERTY_STATE);
        QLatin1String pduType(MM_SMS_PROPERTY_PDUTYPE);
        QLatin1String number(MM_SMS_PROPERTY_NUMBER);
        QLatin1String smsc(MM_SMS_PROPERTY_SMSC);
        QLatin1String data(MM_SMS_PROPERTY_DATA);
        QLatin1String text(MM_SMS_PROPERTY_TEXT);
        QLatin1String validity(MM_SMS_PROPERTY_VALIDITY);
        QLatin1String smsClass(MM_SMS_PROPERTY_CLASS);
        QLatin1String deliveryReportRequest(MM_SMS_PROPERTY_DELIVERYREPORTREQUEST);
        QLatin1String messageReference(MM_SMS_PROPERTY_MESSAGEREFERENCE);
        QLatin1String timestamp(MM_SMS_PROPERTY_TIMESTAMP);
        QLatin1String dischargeTimestamp(MM_SMS_PROPERTY_DISCHARGETIMESTAMP);
        QLatin1String deliveryState(MM_SMS_PROPERTY_DELIVERYSTATE);
        QLatin1String storage(MM_SMS_PROPERTY_STORAGE);

        Q_D(SmsInterface);

        QVariantMap::const_iterator it = changedProperties.constFind(state);
        if (it != changedProperties.constEnd()) {
            d->state = (MMSmsState) it->toInt();
            emit stateChanged(d->state);
        }
        it = changedProperties.constFind(pduType);
        if (it != changedProperties.constEnd()) {
            d->pduType = (MMSmsPduType) it->toInt();
        }
        it = changedProperties.constFind(number);
        if (it != changedProperties.constEnd()) {
            d->number = it->toString();
        }
        it = changedProperties.constFind(smsc);
        if (it != changedProperties.constEnd()) {
            d->smsc = it->toString();
        }
        it = changedProperties.constFind(data);
        if (it != changedProperties.constEnd()) {
            d->data = it->toByteArray();
        }
        it = changedProperties.constFind(text);
        if (it != changedProperties.constEnd()) {
            d->text = it->toString();
        }
        it = changedProperties.constFind(validity);
        if (it != changedProperties.constEnd()) {
            d->validity = it->value<ValidityPair>();
        }
        it = changedProperties.constFind(smsClass);
        if (it != changedProperties.constEnd()) {
            d->smsClass = it->toInt();
        }
        it = changedProperties.constFind(deliveryReportRequest);
        if (it != changedProperties.constEnd()) {
            d->deliveryReportRequest = it->toBool();
        }
        it = changedProperties.constFind(messageReference);
        if (it != changedProperties.constEnd()) {
            d->messageReference = it->toInt();
        }
        it = changedProperties.constFind(timestamp);
        if (it != changedProperties.constEnd()) {
            d->timestamp = QDateTime::fromString(it->toString(), Qt::ISODate);
        }
        it = changedProperties.constFind(dischargeTimestamp);
        if (it != changedProperties.constEnd()) {
            d->dischargeTimestamp = QDateTime::fromString(it->toString(), Qt::ISODate);
        }
        it = changedProperties.constFind(deliveryState);
        if (it != changedProperties.constEnd()) {
            d->deliveryState = (MMSmsDeliveryState) it->toInt();
            emit deliveryStateChanged(d->deliveryState);
        }
        it = changedProperties.constFind(storage);
        if (it != changedProperties.constEnd()) {
            d->storage = (MMSmsStorage) it->toInt();
        }
    }
}

QDBusPendingReply<> ModemManager::SmsInterface::send()
{
    Q_D(SmsInterface);
    return d->smsIface.Send();
}

QDBusPendingReply<> ModemManager::SmsInterface::store(MMSmsStorage storage)
{
    Q_D(SmsInterface);
    return d->smsIface.Store(storage);
}

MMSmsState ModemManager::SmsInterface::state() const
{
    Q_D(const SmsInterface);
    return d->state;
}

MMSmsPduType ModemManager::SmsInterface::pduType() const
{
    Q_D(const SmsInterface);
    return d->pduType;
}

QString ModemManager::SmsInterface::number() const
{
    Q_D(const SmsInterface);
    return d->number;
}

QString ModemManager::SmsInterface::text() const
{
    Q_D(const SmsInterface);
    return d->text;
}

QString ModemManager::SmsInterface::smsc() const
{
    Q_D(const SmsInterface);
    return d->smsc;
}

QByteArray ModemManager::SmsInterface::data() const
{
    Q_D(const SmsInterface);
    return d->data;
}

ValidityPair ModemManager::SmsInterface::validity() const
{
    Q_D(const SmsInterface);
    return d->validity;
}

int ModemManager::SmsInterface::smsClass() const
{
    Q_D(const SmsInterface);
    return d->smsClass;
}

bool ModemManager::SmsInterface::deliveryReportRequest() const
{
    Q_D(const SmsInterface);
    return d->deliveryReportRequest;
}

uint ModemManager::SmsInterface::messageReference() const
{
    Q_D(const SmsInterface);
    return d->messageReference;
}

QDateTime ModemManager::SmsInterface::timestamp() const
{
    Q_D(const SmsInterface);
    return d->timestamp;
}

QDateTime ModemManager::SmsInterface::dischargeTimestamp() const
{
    Q_D(const SmsInterface);
    return d->dischargeTimestamp;
}

MMSmsDeliveryState ModemManager::SmsInterface::deliveryState() const
{
    Q_D(const SmsInterface);
    return d->deliveryState;
}

MMSmsStorage ModemManager::SmsInterface::storage() const
{
    Q_D(const SmsInterface);
    return d->storage;
}
