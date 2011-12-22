/*
Copyright 2008 Will Stephenson <wstephenson@kde.org>
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

#include "modemlocationinterface.h"
#include "modemlocationinterface_p.h"
#include "manager.h"
#include "mmdebug.h"

ModemLocationInterfacePrivate::ModemLocationInterfacePrivate(const QString &path, QObject *owner)
    : ModemInterfacePrivate(path, owner), modemLocationIface(ModemManager::DBUS_SERVICE, path, QDBusConnection::systemBus())
{
}

ModemManager::ModemLocationInterface::ModemLocationInterface(const QString & path, QObject * parent)
    : ModemInterface(*new ModemLocationInterfacePrivate(path, this), parent)
{
    Q_D(const ModemLocationInterface);
    d->modemLocationIface.connection().connect(ModemManager::DBUS_SERVICE,
        path, QLatin1String("org.freedesktop.DBus.Properties"),
        QLatin1String("MmPropertiesChanged"), QLatin1String("sa{sv}"),
        this, SLOT(propertiesChanged(QString,QVariantMap)));
}

ModemManager::ModemLocationInterface::~ModemLocationInterface()
{
}

void ModemManager::ModemLocationInterface::propertiesChanged(const QString & interface, const QVariantMap & properties)
{
    mmDebug() << interface << properties.keys();

    if (interface == QString("org.freedesktop.ModemManager.Modem.Location")) {
        QLatin1String capabilities("Capabilities");
        QLatin1String enabled("Enabled");
        QLatin1String signalsLocation("SignalsLocation");
        QLatin1String location("Location");

        QVariantMap::const_iterator it = properties.find(capabilities);
        if ( it != properties.end()) {
            emit capabilitiesChanged((ModemManager::ModemLocationInterface::Capability)it->toInt());
        }
        it = properties.find(enabled);
        if ( it != properties.end()) {
            emit enabledChanged(it->toBool());
        }
        it = properties.find(signalsLocation);
        if ( it != properties.end()) {
            emit signalsLocationChanged(it->toBool());
        }
        it = properties.find(location);
        if ( it != properties.end()) {
            QVariant v = it.value();
            LocationInformationMap map;
            if (v.canConvert<LocationInformationMap>()) {
                map = v.value<LocationInformationMap>();
            } else {
                mmDebug() << "Error converting LocationInformationMap property";
            }
            emit locationChanged(map);
        }
    }
}

void ModemManager::ModemLocationInterface::enableLocation(const bool enable, const bool signalLocation)
{
    Q_D(ModemLocationInterface);
    d->modemLocationIface.Enable(enable, signalLocation);
}

ModemManager::ModemLocationInterface::LocationInformationMap ModemManager::ModemLocationInterface::getLocation()
{
    Q_D(ModemLocationInterface);
    QDBusReply<LocationInformationMap> location = d->modemLocationIface.GetLocation();

    if (location.isValid()) {
        return location.value();
    }

    return LocationInformationMap();
}

ModemManager::ModemLocationInterface::Capability ModemManager::ModemLocationInterface::getCapability() const
{
    Q_D(const ModemLocationInterface);
    return (ModemManager::ModemLocationInterface::Capability) d->modemLocationIface.capabilities();
}

bool ModemManager::ModemLocationInterface::enabled() const
{
    Q_D(const ModemLocationInterface);
    return d->modemLocationIface.enabled();
}

bool ModemManager::ModemLocationInterface::signalsLocation() const
{
    Q_D(const ModemLocationInterface);
    return d->modemLocationIface.signalsLocation();
}



