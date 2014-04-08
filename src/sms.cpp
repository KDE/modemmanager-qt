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

#include "sms.h"
#include "sms_p.h"

#include "dbus/dbus.h"

#include <ModemManager/ModemManager.h>

SmsPrivate::SmsPrivate(const QString &path)
    : smsIface(MM_DBUS_SERVICE, path, QDBusConnection::systemBus())
{
}

ModemManager::Sms::Sms(const QString &path, QObject *parent)
    : QObject(parent)
    , d_ptr(new SmsPrivate(path))
{
    Q_D(Sms);

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
    d->storage = (MMSmsStorage) d->smsIface.storage();
    d->serviceCategory = (MMSmsCdmaServiceCategory) d->smsIface.serviceCategory();
    d->teleserviceId = (MMSmsCdmaTeleserviceId) d->smsIface.teleserviceId();

    QDBusConnection::systemBus().connect(MM_DBUS_SERVICE, path, DBUS_INTERFACE_PROPS, QStringLiteral("PropertiesChanged"), this,
                                         SLOT(onPropertiesChanged(QString,QVariantMap,QStringList)));
}

ModemManager::Sms::~Sms()
{
    delete d_ptr;
}

void ModemManager::Sms::onPropertiesChanged(const QString &interfaceName, const QVariantMap &changedProperties, const QStringList &invalidatedProperties)
{
    Q_UNUSED(invalidatedProperties);
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
#if MM_CHECK_VERSION(1,1,900)
        QLatin1String serviceCategory(MM_SMS_PROPERTY_SERVICECATEGORY);
        QLatin1String teleserviceId(MM_SMS_PROPERTY_TELESERVICEID);
#endif

        Q_D(Sms);

        QVariantMap::const_iterator it = changedProperties.constFind(state);
        if (it != changedProperties.constEnd()) {
            d->state = (MMSmsState) it->toUInt();
            emit stateChanged(d->state);
        }
        it = changedProperties.constFind(pduType);
        if (it != changedProperties.constEnd()) {
            d->pduType = (MMSmsPduType) it->toUInt();
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
            d->messageReference = it->toUInt();
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
            d->deliveryState = (MMSmsDeliveryState) it->toUInt();
            emit deliveryStateChanged(d->deliveryState);
        }
        it = changedProperties.constFind(storage);
        if (it != changedProperties.constEnd()) {
            d->storage = (MMSmsStorage) it->toUInt();
        }
#if MM_CHECK_VERSION(1,1,900)
        it = changedProperties.constFind(serviceCategory);
        if (it != changedProperties.constEnd()) {
            d->serviceCategory = (MMSmsCdmaServiceCategory) it->toUInt();
        }
        it = changedProperties.constFind(teleserviceId);
        if (it != changedProperties.constEnd()) {
            d->teleserviceId = (MMSmsCdmaTeleserviceId) it->toUInt();
        }
#endif
    }
}

QDBusPendingReply<> ModemManager::Sms::send()
{
    Q_D(Sms);
    return d->smsIface.Send();
}

QDBusPendingReply<> ModemManager::Sms::store(MMSmsStorage storage)
{
    Q_D(Sms);
    return d->smsIface.Store(storage);
}

MMSmsState ModemManager::Sms::state() const
{
    Q_D(const Sms);
    return d->state;
}

MMSmsPduType ModemManager::Sms::pduType() const
{
    Q_D(const Sms);
    return d->pduType;
}

QString ModemManager::Sms::number() const
{
    Q_D(const Sms);
    return d->number;
}

QString ModemManager::Sms::text() const
{
    Q_D(const Sms);
    return d->text;
}

QString ModemManager::Sms::smsc() const
{
    Q_D(const Sms);
    return d->smsc;
}

QByteArray ModemManager::Sms::data() const
{
    Q_D(const Sms);
    return d->data;
}

ValidityPair ModemManager::Sms::validity() const
{
    Q_D(const Sms);
    return d->validity;
}

int ModemManager::Sms::smsClass() const
{
    Q_D(const Sms);
    return d->smsClass;
}

bool ModemManager::Sms::deliveryReportRequest() const
{
    Q_D(const Sms);
    return d->deliveryReportRequest;
}

uint ModemManager::Sms::messageReference() const
{
    Q_D(const Sms);
    return d->messageReference;
}

QDateTime ModemManager::Sms::timestamp() const
{
    Q_D(const Sms);
    return d->timestamp;
}

QDateTime ModemManager::Sms::dischargeTimestamp() const
{
    Q_D(const Sms);
    return d->dischargeTimestamp;
}

MMSmsDeliveryState ModemManager::Sms::deliveryState() const
{
    Q_D(const Sms);
    return d->deliveryState;
}

MMSmsStorage ModemManager::Sms::storage() const
{
    Q_D(const Sms);
    return d->storage;
}

MMSmsCdmaServiceCategory ModemManager::Sms::serviceCategory() const
{
    Q_D(const Sms);
    return d->serviceCategory;
}

MMSmsCdmaTeleserviceId ModemManager::Sms::teleserviceId() const
{
    Q_D(const Sms);
    return d->teleserviceId;
}
