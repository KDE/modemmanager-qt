/*
    Copyright 2008,2011 Will Stephenson <wstephenson@kde.org>
    Copyright 2010 Lamarque Souza <lamarque@kde.org>
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

#include "modem3gpp.h"
#include "modem3gpp_p.h"
#include "mmdebug.h"
#include "dbus/dbus.h"

Modem3gppPrivate::Modem3gppPrivate(const QString &path)
    : InterfacePrivate(path)
    , modem3gppIface(MM_DBUS_SERVICE, path, QDBusConnection::systemBus())
{
}

ModemManager::Modem3gpp::Modem3gpp(const QString &path, QObject *parent)
    : Interface(*new Modem3gppPrivate(path), parent)
{
    Q_D(Modem3gpp);

    QDBusConnection::systemBus().connect(MM_DBUS_SERVICE, d->uni, DBUS_INTERFACE_PROPS, "PropertiesChanged", this,
                                         SLOT(onPropertiesChanged(QString,QVariantMap,QStringList)));
}

ModemManager::Modem3gpp::~Modem3gpp()
{
}

QString ModemManager::Modem3gpp::imei() const
{
    Q_D(const Modem3gpp);
    return d->modem3gppIface.imei();
}

MMModem3gppRegistrationState ModemManager::Modem3gpp::registrationState() const
{
    Q_D(const Modem3gpp);
    return (MMModem3gppRegistrationState)d->modem3gppIface.registrationState();
}

QString ModemManager::Modem3gpp::operatorCode() const
{
    Q_D(const Modem3gpp);
    return d->modem3gppIface.operatorCode();
}

QString ModemManager::Modem3gpp::operatorName() const
{
    Q_D(const Modem3gpp);
    return d->modem3gppIface.operatorName();
}

ModemManager::Modem3gpp::FacilityLocks ModemManager::Modem3gpp::enabledFacilityLocks() const
{
    Q_D(const Modem3gpp);
    return (FacilityLocks)d->modem3gppIface.enabledFacilityLocks();
}

void ModemManager::Modem3gpp::registerToNetwork(const QString &networkId)
{
    Q_D(Modem3gpp);
    d->modem3gppIface.Register(networkId);
}

QDBusPendingReply<QVariantMapList> ModemManager::Modem3gpp::scan()
{
    Q_D(Modem3gpp);
    return d->modem3gppIface.Scan();
}

void ModemManager::Modem3gpp::onPropertiesChanged(const QString &interface, const QVariantMap &properties, const QStringList &invalidatedProps)
{
    Q_D(Modem3gpp);
    mmDebug() << interface << properties.keys();

    if (interface == QString(MM_DBUS_INTERFACE_MODEM_MODEM3GPP)) {
        QVariantMap::const_iterator it = properties.constFind(MM_MODEM_MODEM3GPP_PROPERTY_REGISTRATIONSTATE);
        if (it != properties.constEnd()) {
            emit registrationStateChanged((MMModem3gppRegistrationState) it->toUInt());
        }

        it = properties.constFind(MM_MODEM_MODEM3GPP_PROPERTY_ENABLEDFACILITYLOCKS);
        if (it != properties.constEnd()) {
            emit enabledFacilityLocksChanged((FacilityLocks) it->toUInt());
        }
    }
}
