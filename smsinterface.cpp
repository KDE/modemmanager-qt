/*
Copyright 2013 Anant Kamath <kamathanant@gmail.com>

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

#include <QDebug>

SmsInterfacePrivate::SmsInterfacePrivate(const QString &path, QObject *owner)
    : ModemInterfacePrivate(path, owner), smsIface(ModemManager::DBUS_SERVICE, path, QDBusConnection::systemBus()), propertiesIface(ModemManager::DBUS_SERVICE, path, QLatin1String("org.freedesktop.DBus.Properties"), QDBusConnection::systemBus())
{
}

ModemManager::SmsInterface::SmsInterface(const QString & path, QObject * parent)
    : ModemInterface(*new SmsInterfacePrivate(path, this), parent)
{
    Q_D(const SmsInterface);
    
    d->state = (ModemManager::SmsInterface::SmsState) d->smsIface.state();
    d->pduType = (ModemManager::SmsInterface::SmsPduType) d->smsIface.pduType();
    d->number = d->smsIface.number();
    d->text = d->smsIface.text();
    d->SMSC = d->smsIface.sMSC();
    d->data = (QByteArray) d->smsIface.data();
    d->validity = ModemManager::SmsInterface::SmsValidity d->smsIface.validity();
    d->smsClass = d->smsIface.Class();
    d->deliveryReportRequest = d->smsIface.deliveryReportRequest();
    d->messageReference = d->smsIface.messageReference();
    d->timestamp =  d->smsIface.timestamp();
    d->dischargeTimestamp = d->smsIface.dischargeTimestamp();
    d->deliveryState = (ModemManager::SmsInterface::SmsDeliveryState) d->smsIface.deliveryState();
    d->storage = (ModemManager::SmsInterface::SmsStorage) d->smsIface.storage();

    connect(&d->propertyManagerIface, SIGNAL(PropertiesChanged(QString,QMap<QString,QVariant>,QStringList)),
                this, SIGNAL(propertiesChanged(QString,QVariantMap,QStringList)));
}

ModemManager::SmsInterface::~SmsInterface()
{
}

ModemManager::SmsInterface::propertiesChanged(QString interfaceName, QVariantMap changedProperties, QStringList invalidatedProperties)
{
    //if(interfaceName == "org.freedesktop.ModemManager1.Sms"){
    if (interfaceName == QString("org.freedesktop.ModemManager1.Sms")) {
    
        QVariantMap::const_iterator it = changedProperties.find(QLatin1String("State"));
        if ( it != properties.end()) {
            d->state = (ModemManager::SmsInterface::SmsState) it->toInt();
            emit stateChanged((ModemManager::SmsInterface::SmsState) it->toInt());
        }
        it = changeProperties.find(QLatin1String("PduType"));
        if ( it != properties.end()) {
            d->pduType = (ModemManager::SmsInterface::SmsPduType) it->toInt();
        }
        it = changeProperties.find(QLatin1String("Number"));
        if ( it != properties.end()) {
            d->number = it->toQString();
        }
        it = changeProperties.find(QLatin1String("SMSC"));
        if ( it != properties.end()) {
            d->SMSC = it->toQString();
        }
        it = changeProperties.find(QLatin1String("Data"));
        if ( it != properties.end()) {
            d->data = it->toQByteArray();
        }
        it = changeProperties.find(QLatin1String("Text"));
        if ( it != properties.end()) {
            d->text = it->toQString();
        }
        it = changeProperties.find(QLatin1String("Validity"));
        if ( it != properties.end()) {
            d->validity = (ModemManager::SmsInterface::SmsValidityType) it->toInt();
        }
        it = changeProperties.find(QLatin1String("Class"));
        if ( it != properties.end()) {
            d->smsClass = it->toInt();
        }
        it = changeProperties.find(QLatin1String("DeliveryReportRequest"));
        if ( it != properties.end()) {
            d->deliveryReportRequest = it->toBool();
        }
        it = changeProperties.find(QLatin1String("MessageReference"));
        if ( it != properties.end()) {
            d->messageReference = it->toInt();
        }
        it = changeProperties.find(QLatin1String("Timestamp"));
        if ( it != properties.end()) {
            d->timestamp = it->toQString();
        }
        it = changeProperties.find(QLatin1String("DischargeTimestamp"));
        if ( it != properties.end()) {
            d->dischargeTimestamp = it->toQString();
        }
        it = changeProperties.find(QLatin1String("DeliveryState"));
        if ( it != properties.end()) {
            d->deliveryState = (ModemManager::SmsInterface::SmsDeliveryState) it->toInt();
            emit deliveryStateChanged((ModemManager::SmsInterface::SmsDeliveryState) it->toInt());
        }
        it = changeProperties.find(QLatin1String("Storage"));
        if ( it != properties.end()) {
            d->store = (ModemManager::SmsInterface::SmsStorage) it->toInt();
            emit storageChanged((ModemManager::SmsInterface::SmsStorage) it->toInt());
        }
    }

void ModemManager::SmsInterface::send()
{
    Q_D(SmsInterface);
    d->smsIface.Send();
}

void ModemManager::SmsInterface::store(ModemManager::SmsInterface::SmsStorage storage)
{
    Q_D(SmsInterface);
    d->smsIface.Store(storage);
}

ModemManager::SmsInterface::SmsState ModemManager::SmsInterface::state()
{
    Q_D(const SmsInterface);
    return d->state;
}

ModemManager::SmsInterface::SmsPduType ModemManager::SmsInterface::pduType()
{
    Q_D(const SmsInterface);
    return d->pduType;
}

QString ModemManager::SmsInterface::number()
{
    Q_D(const SmsInterface);
    return d->number;
}
 
QString ModemManager::SmsInterface::text()
{
    Q_D(const SmsInterface);
    return d->text;
}

QString ModemManager::SmsInterface::SMSC()
{
    Q_D(const SmsInterface);
    return d->SMSC;
}

QByteArray ModemManager::SmsInterface::data()
{
    Q_D(const SmsInterface);
    return d->data;
}

ModemManager::SmsInterface::SmsValidity validity()
{
    Q_D(const SmsInterface);
    return d->validity;
}

int ModemManager::SmsInterface::smsClass()
{
    Q_D(const SmsInterface);
    return d->smsClass;
}

bool ModemManager::SmsInterface::deliveryReportRequest()
{
    Q_D(const SmsInterface);
    return d->deliveryReportRequest;
}

uint ModemManager::SmsInterface::MessageReference()
{
    Q_D(const SmsInterface);
    return d->messageReference;
}

QString ModemManager::SmsInterface::timestamp()
{
    Q_D(const SmsInterface);
    return d->timestamp;
}

QString ModemManager::SmsInterface::dischargeTimestamp()
{
    Q_D(const SmsInterface);
    return d->dischargeTimestamp;
}

ModemManager::SmsInterface::SmsDeliveryState ModemManager::SmsInterface::deliveryState()
{
    Q_D(const SmsInterface);
    return d->deliveryState;
}

ModemManager::SmsInterface::SmsStorage ModemManager::SmsInterface::storage()
{
    Q_D(const SmsInterface);
    return d->storage;
}
