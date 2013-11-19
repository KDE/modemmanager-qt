/*
    Copyright 2008 Will Stephenson <wstephenson@kde.org>
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

#include "modemlocation.h"
#include "modemlocation_p.h"
#include "dbus/dbus.h"
#include "mmdebug.h"

ModemLocationPrivate::ModemLocationPrivate(const QString &path)
    : InterfacePrivate(path)
    , modemLocationIface(MM_DBUS_SERVICE, path, QDBusConnection::systemBus())
{
}

ModemManager::ModemLocation::ModemLocation(const QString &path, QObject *parent)
    : Interface(*new ModemLocationPrivate(path), parent)
{
    Q_D(ModemLocation);

    QDBusConnection::systemBus().connect(MM_DBUS_SERVICE, d->uni, DBUS_INTERFACE_PROPS, "PropertiesChanged", this,
                                         SLOT(onPropertiesChanged(QString,QVariantMap,QStringList)));
}

ModemManager::ModemLocation::~ModemLocation()
{
}

void ModemManager::ModemLocation::onPropertiesChanged(const QString &interface, const QVariantMap &properties, const QStringList &invalidatedProps)
{
    mmDebug() << interface << properties.keys();

    if (interface == QString(MM_DBUS_INTERFACE_MODEM_LOCATION)) {
        QLatin1String capabilities(MM_MODEM_LOCATION_PROPERTY_CAPABILITIES);
        QLatin1String enabled(MM_MODEM_LOCATION_PROPERTY_ENABLED);
        QLatin1String signalsLocation(MM_MODEM_LOCATION_PROPERTY_SIGNALSLOCATION);
        QLatin1String location(MM_MODEM_LOCATION_PROPERTY_LOCATION);

        QVariantMap::const_iterator it = properties.constFind(capabilities);
        if ( it != properties.constEnd()) {
            emit capabilitiesChanged((ModemManager::ModemLocation::LocationSources)it->toUInt());
        }
        it = properties.constFind(enabled);
        if ( it != properties.constEnd()) {
            emit isEnabledChanged(it->toBool());
        }
        it = properties.constFind(signalsLocation);
        if ( it != properties.constEnd()) {
            emit signalsLocationChanged(it->toBool());
        }
        it = properties.constFind(location);
        if ( it != properties.constEnd()) {
            QVariant v = it.value();  // FIXME demarshall properly
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

void ModemManager::ModemLocation::setup(ModemManager::ModemLocation::LocationSources sources, bool signalLocation)
{
    Q_D(ModemLocation);
    d->modemLocationIface.Setup(sources, signalLocation);
}

LocationInformationMap ModemManager::ModemLocation::location()
{
    Q_D(ModemLocation);
    QDBusReply<LocationInformationMap> location = d->modemLocationIface.GetLocation();

    if (location.isValid()) {
        return location.value();
    }

    return LocationInformationMap();
}

ModemManager::ModemLocation::LocationSources ModemManager::ModemLocation::capabilities() const
{
    Q_D(const ModemLocation);
    return (LocationSources)d->modemLocationIface.capabilities();
}

ModemManager::ModemLocation::LocationSources ModemManager::ModemLocation::enabledCapabilities() const
{
    Q_D(const ModemLocation);
    return (LocationSources)d->modemLocationIface.enabled();
}

bool ModemManager::ModemLocation::isEnabled() const
{
    Q_D(const ModemLocation);
    return d->modemLocationIface.enabled() > MM_MODEM_LOCATION_SOURCE_NONE;
}

bool ModemManager::ModemLocation::signalsLocation() const
{
    Q_D(const ModemLocation);
    return d->modemLocationIface.signalsLocation();
}
