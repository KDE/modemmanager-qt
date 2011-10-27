/*
Copyright 2008,2011 Will Stephenson <wstephenson@kde.org>
Copyright 2010 Lamarque Souza <lamarque@kde.org>

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 2 of
the License or (at your option) version 3 or any later version
accepted by the membership of KDE e.V. (or its successor approved
by the membership of KDE e.V.), which shall act as a proxy
defined in Section 14 of version 3 of the license.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "manager.h"
#include "modemgsmnetworkinterface.h"
#include "modemgsmnetworkinterface_p.h"

#include "mmdebug.h"

ModemGsmNetworkInterfacePrivate::ModemGsmNetworkInterfacePrivate(const QString &path, QObject *owner)
    : ModemInterfacePrivate(path, owner), modemGsmNetworkIface(ModemManager::DBUS_SERVICE, path, QDBusConnection::systemBus())
{
}

ModemManager::ModemGsmNetworkInterface::ModemGsmNetworkInterface(const QString & path, QObject * parent)
    : ModemInterface(*new ModemGsmNetworkInterfacePrivate(path, this), parent)
{
    Q_D(ModemGsmNetworkInterface);

    d->modemGsmNetworkIface.connection().connect(ModemManager::DBUS_SERVICE,
        path, QLatin1String("org.freedesktop.DBus.Properties"),
        QLatin1String("MmPropertiesChanged"), QLatin1String("sa{sv}"),
        this, SLOT(propertiesChanged(const QString &,const QVariantMap &)));

    connect( &d->modemGsmNetworkIface, SIGNAL(RegistrationInfo(uint, const QString &, const QString &)),
                this, SLOT(slotRegistrationInfoChanged(uint, const QString &, const QString &)));
    connect( &d->modemGsmNetworkIface, SIGNAL(SignalQuality(uint)),
                this, SIGNAL(signalQualityChanged(uint)));
}

ModemManager::ModemGsmNetworkInterface::~ModemGsmNetworkInterface()
{

}

void ModemManager::ModemGsmNetworkInterface::propertiesChanged(const QString & interface, const QVariantMap & properties)
{
    mmDebug() << interface << properties.keys();

    if (interface == QString("org.freedesktop.ModemManager.Modem.Gsm.Network")) {
        QLatin1String allowedMode("AllowedMode");
        QLatin1String accessTechnology("AccessTechnology");

        QVariantMap::const_iterator it = properties.find(allowedMode);
        if ( it != properties.end()) {
            emit allowedModeChanged((ModemManager::ModemInterface::AllowedMode) it->toInt());
        }
        it = properties.find(accessTechnology);
        if ( it != properties.end()) {
            emit accessTechnologyChanged((ModemManager::ModemInterface::AccessTechnology) it->toInt());
        }
    }
}

void ModemManager::ModemGsmNetworkInterface::slotRegistrationInfoChanged(uint status, const QString & operatorCode, const QString &operatorName)
{
    RegistrationInfoType r;

    r.status = (ModemManager::ModemGsmNetworkInterface::RegistrationStatus) status;
    r.operatorCode = operatorCode;
    r.operatorName = operatorName;

    emit registrationInfoChanged(r);
}

ModemManager::ModemInterface::AllowedMode ModemManager::ModemGsmNetworkInterface::getAllowedMode() const
{
    Q_D(const ModemGsmNetworkInterface);
    return (ModemManager::ModemInterface::AllowedMode) d->modemGsmNetworkIface.allowedMode();
}

ModemManager::ModemInterface::AccessTechnology ModemManager::ModemGsmNetworkInterface::getAccessTechnology() const
{
    Q_D(const ModemGsmNetworkInterface);
    return (ModemManager::ModemInterface::AccessTechnology) d->modemGsmNetworkIface.accessTechnology();
}

void ModemManager::ModemGsmNetworkInterface::registerToNetwork(const QString & networkId)
{
    Q_D(ModemGsmNetworkInterface);
    d->modemGsmNetworkIface.Register(networkId);
}

ModemManager::ModemGsmNetworkInterface::ScanResultsType ModemManager::ModemGsmNetworkInterface::scan()
{
    Q_D(ModemGsmNetworkInterface);
    return d->modemGsmNetworkIface.Scan();
}

void ModemManager::ModemGsmNetworkInterface::setApn(const QString & apn)
{
    Q_D(ModemGsmNetworkInterface);
    d->modemGsmNetworkIface.SetApn(apn);
}

void ModemManager::ModemGsmNetworkInterface::setBand(const ModemManager::ModemInterface::Band band)
{
    Q_D(ModemGsmNetworkInterface);
    d->modemGsmNetworkIface.SetBand(band);
}

ModemManager::ModemInterface::Band ModemManager::ModemGsmNetworkInterface::getBand()
{
    Q_D(ModemGsmNetworkInterface);
    QDBusReply< uint > band = d->modemGsmNetworkIface.GetBand();

    if (band.isValid())
        return (ModemManager::ModemInterface::Band) band.value();

    mmDebug() << "Error getting band setting info: " << band.error().name() << ": " << band.error().message();
    return ModemManager::ModemInterface::UnknownBand;
}

ModemManager::ModemGsmNetworkInterface::RegistrationInfoType ModemManager::ModemGsmNetworkInterface::getRegistrationInfo()
{
    Q_D(ModemGsmNetworkInterface);
    QDBusReply< RegistrationInfoType > registrationInfo = d->modemGsmNetworkIface.GetRegistrationInfo();

    if (registrationInfo.isValid())
        return registrationInfo.value();

    mmDebug() << "Error getting registration info for operator: " << registrationInfo.error().name() << ": " << registrationInfo.error().message();
    return RegistrationInfoType();
}

uint ModemManager::ModemGsmNetworkInterface::getSignalQuality()
{
    Q_D(ModemGsmNetworkInterface);
    QDBusReply< uint > signalQuality = d->modemGsmNetworkIface.GetSignalQuality();

    if (signalQuality.isValid())
        return signalQuality.value();

    mmDebug() << "Error getting signal quality: " << signalQuality.error().name() << ": " << signalQuality.error().message();
    return 0;
}

void ModemManager::ModemGsmNetworkInterface::setAllowedMode(const ModemManager::ModemInterface::AllowedMode mode)
{
    Q_D(ModemGsmNetworkInterface);
    d->modemGsmNetworkIface.SetAllowedMode(mode);
}



