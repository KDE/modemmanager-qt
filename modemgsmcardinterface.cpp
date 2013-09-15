/*
Copyright 2008,2011 Will Stephenson <wstephenson@kde.org>
Copyright 2010-2011 Lamarque Souza <lamarque@kde.org>

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

#include "modemgsmcardinterface.h"
#include "modemgsmcardinterface_p.h"
#include "manager.h"
#include "mmdebug.h"

ModemGsmCardInterfacePrivate::ModemGsmCardInterfacePrivate(const QString &path, QObject *owner)
    : ModemInterfacePrivate(path, owner), modemGsmCardIface(ModemManager::DBUS_SERVICE, path, QDBusConnection::systemBus())
{
}

ModemManager::ModemGsmCardInterface::ModemGsmCardInterface(const QString & path, QObject * parent)
    : ModemInterface(*new ModemGsmCardInterfacePrivate(path, this), parent)
{
    Q_D(ModemGsmCardInterface);

    d->supportedBands = (ModemManager::ModemInterface::Band) d->modemGsmCardIface.supportedBands();
    d->supportedModes = (ModemManager::ModemInterface::Mode) d->modemGsmCardIface.supportedModes();
    d->simIdentifier = d->modemGsmCardIface.simIdentifier();

    d->modemGsmCardIface.connection().connect(ModemManager::DBUS_SERVICE,
        path, QLatin1String("org.freedesktop.DBus.Properties"),
        QLatin1String("MmPropertiesChanged"), QLatin1String("sa{sv}"),
        this, SLOT(propertiesChanged(QString,QVariantMap)));
}

ModemManager::ModemGsmCardInterface::~ModemGsmCardInterface()
{

}

void ModemManager::ModemGsmCardInterface::propertiesChanged(const QString & interface, const QVariantMap & properties)
{
    mmDebug() << interface << properties.keys();

    if (interface == QString("org.freedesktop.ModemManager.Modem.Gsm.Card")) {
        QLatin1String supportedBands("SupportedBands");
        QLatin1String supportedModes("SupportedModes");
        QLatin1String simIdentifier("SimIdentifier");

        Q_D(ModemGsmCardInterface);

        QVariantMap::const_iterator it = properties.find(supportedBands);
        if ( it != properties.end()) {
            d->supportedBands = (ModemManager::ModemInterface::Band) it->toInt();
            emit supportedBandsChanged(d->supportedBands);
        }
        it = properties.find(supportedModes);
        if ( it != properties.end()) {
            d->supportedModes = (ModemManager::ModemInterface::Mode) it->toInt();
            emit supportedModesChanged(d->supportedModes);
        }
        it = properties.find(simIdentifier);
        if ( it != properties.end()) {
            d->simIdentifier = it->toString();
            emit simIdentifierChanged(d->simIdentifier);
        }
    }
}

QString ModemManager::ModemGsmCardInterface::getImei()
{
    Q_D(ModemGsmCardInterface);
    QDBusReply<QString> imei = d->modemGsmCardIface.GetImei();

    if (imei.isValid()) {
        return imei.value();
    }

    return QString();
}

QString ModemManager::ModemGsmCardInterface::getImsi()
{
    Q_D(ModemGsmCardInterface);
    QDBusReply<QString> imsi = d->modemGsmCardIface.GetImsi();

    if (imsi.isValid()) {
        return imsi.value();
    }

    return QString();
}

QDBusPendingReply<QString> ModemManager::ModemGsmCardInterface::operatorId()
{
    Q_D(ModemGsmCardInterface);
    return d->modemGsmCardIface.GetOperatorId();
}

QDBusPendingReply<QString> ModemManager::ModemGsmCardInterface::serviceProviderName()
{
    Q_D(ModemGsmCardInterface);
    return d->modemGsmCardIface.GetSpn();
}

QDBusPendingReply<> ModemManager::ModemGsmCardInterface::sendPuk(const QString & puk, const QString & pin)
{
    Q_D(ModemGsmCardInterface);
    return d->modemGsmCardIface.SendPuk(puk, pin);
}

QDBusPendingReply<> ModemManager::ModemGsmCardInterface::sendPin(const QString & pin)
{
    Q_D(ModemGsmCardInterface);
    return d->modemGsmCardIface.SendPin(pin);
}

QDBusPendingReply<> ModemManager::ModemGsmCardInterface::enablePin(const QString & pin, bool enabled)
{
    Q_D(ModemGsmCardInterface);
    return d->modemGsmCardIface.EnablePin(pin, enabled);
}

QDBusPendingReply<> ModemManager::ModemGsmCardInterface::changePin(const QString & oldPin, const QString & newPin)
{
    Q_D(ModemGsmCardInterface);
    return d->modemGsmCardIface.ChangePin(oldPin, newPin);
}

ModemManager::ModemInterface::Band ModemManager::ModemGsmCardInterface::getSupportedBands() const
{
    Q_D(const ModemGsmCardInterface);
    return d->supportedBands;
}

ModemManager::ModemInterface::Mode ModemManager::ModemGsmCardInterface::getSupportedModes() const
{
    Q_D(const ModemGsmCardInterface);
    return d->supportedModes;
}

QString ModemManager::ModemGsmCardInterface::simIdentifier() const
{
    Q_D(const ModemGsmCardInterface);
    return d->simIdentifier;
}
