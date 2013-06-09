/*
Copyright 2008,2011 Will Stephenson <wstephenson@kde.org>
Copyright 2010 Lamarque Souza <lamarque@kde.org>

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

#include "modemgsmnetworkinterface.h"
#include "modemgsmnetworkinterface_p.h"
#include "manager.h"
#include "mmdebug.h"

ModemManager::ModemGsmNetworkInterface::RegistrationInfoType::RegistrationInfoType()
{
    status = StatusUnknown;
}

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
        this, SLOT(propertiesChanged(QString,QVariantMap)));

    connect( &d->modemGsmNetworkIface, SIGNAL(RegistrationInfo(uint,QString,QString)),
             this, SLOT(slotRegistrationInfoChanged(uint,QString,QString)));
    connect( &d->modemGsmNetworkIface, SIGNAL(SignalQuality(uint)),
             this, SLOT(slotSignalQualityChanged(uint)));

    d->signalQuality = 0;
    d->accessTechnology = (ModemManager::ModemInterface::AccessTechnology)d->modemGsmNetworkIface.accessTechnology();
    d->allowedMode = (ModemManager::ModemInterface::AllowedMode)d->modemGsmNetworkIface.allowedMode();

#define getProperty(property) QDBusPendingCall reply##property = d->modemGsmNetworkIface.Get##property(); \
    QDBusPendingCallWatcher *watcher##property = new QDBusPendingCallWatcher(reply##property); \
    connect(watcher##property, SIGNAL(finished(QDBusPendingCallWatcher*)), SLOT(onGet##property##Arrived(QDBusPendingCallWatcher*)));

    getProperty(SignalQuality);
    getProperty(RegistrationInfo);
}

ModemManager::ModemGsmNetworkInterface::~ModemGsmNetworkInterface()
{

}

void ModemManager::ModemGsmNetworkInterface::propertiesChanged(const QString & interface, const QVariantMap & properties)
{
    Q_D(ModemGsmNetworkInterface);
    mmDebug() << interface << properties.keys();

    if (interface == QString("org.freedesktop.ModemManager.Modem.Gsm.Network")) {
        QLatin1String allowedMode("AllowedMode");
        QLatin1String accessTechnology("AccessTechnology");

        QVariantMap::const_iterator it = properties.find(allowedMode);
        if ( it != properties.end()) {
            d->allowedMode = (ModemManager::ModemInterface::AllowedMode) it->toUInt();
            emit allowedModeChanged(d->allowedMode);
        }
        it = properties.find(accessTechnology);
        if ( it != properties.end()) {
            d->accessTechnology = (ModemManager::ModemInterface::AccessTechnology) it->toUInt();
            emit accessTechnologyChanged(d->accessTechnology);

            // Some modems, like Sony Ericsson MD300, do not update signal quality when on 3G access technology mode, so
            // update the value here.
            getProperty(SignalQuality);
        }
    }
}

void ModemManager::ModemGsmNetworkInterface::onGetSignalQualityArrived(QDBusPendingCallWatcher *watcher)
{
    if (!watcher) {
        return;
    }

    QDBusPendingReply<uint> reply = *watcher;
    if (reply.isValid()) {
        slotSignalQualityChanged(reply.value());
    }
    watcher->deleteLater();
}

void ModemManager::ModemGsmNetworkInterface::slotSignalQualityChanged(uint signalQuality)
{
    Q_D(ModemGsmNetworkInterface);
    d->signalQuality = signalQuality;
    emit signalQualityChanged(d->signalQuality);
}

void ModemManager::ModemGsmNetworkInterface::onGetRegistrationInfoArrived(QDBusPendingCallWatcher *watcher)
{
    if (!watcher) {
        return;
    }

    QDBusPendingReply<RegistrationInfoType> reply = *watcher;
    if (reply.isValid()) {
        RegistrationInfoType v = reply.value();
        slotRegistrationInfoChanged(v.status, v.operatorCode, v.operatorName);
    }
    watcher->deleteLater();
}

void ModemManager::ModemGsmNetworkInterface::slotRegistrationInfoChanged(uint status, const QString & operatorCode, const QString &operatorName)
{
    Q_D(ModemGsmNetworkInterface);

    d->registrationInfo.status = (ModemManager::ModemGsmNetworkInterface::RegistrationStatus) status;
    d->registrationInfo.operatorCode = operatorCode;
    d->registrationInfo.operatorName = operatorName;

    emit registrationInfoChanged(d->registrationInfo);
}

ModemManager::ModemInterface::AllowedMode ModemManager::ModemGsmNetworkInterface::getAllowedMode() const
{
    Q_D(const ModemGsmNetworkInterface);
    return d->allowedMode;
}

ModemManager::ModemInterface::AccessTechnology ModemManager::ModemGsmNetworkInterface::getAccessTechnology() const
{
    Q_D(const ModemGsmNetworkInterface);
    return d->accessTechnology;
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

ModemManager::ModemGsmNetworkInterface::RegistrationInfoType ModemManager::ModemGsmNetworkInterface::getRegistrationInfo() const
{
    Q_D(const ModemGsmNetworkInterface);
    return d->registrationInfo;
}

uint ModemManager::ModemGsmNetworkInterface::getSignalQuality() const
{
    Q_D(const ModemGsmNetworkInterface);
    return d->signalQuality;
}

void ModemManager::ModemGsmNetworkInterface::setAllowedMode(const ModemManager::ModemInterface::AllowedMode mode)
{
    Q_D(ModemGsmNetworkInterface);
    d->modemGsmNetworkIface.SetAllowedMode(mode);
}
