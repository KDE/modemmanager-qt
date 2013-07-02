/*
Copyright 2008 Will Stephenson <wstephenson@kde.org>
Copyright 2010 Lamarque Souza <lamarque@kde.org>
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

#include "modemlocationinterface.h"
#include "modemlocationinterface_p.h"
#include "manager.h"
#include "mmdebug.h"

ModemLocationInterfacePrivate::ModemLocationInterfacePrivate(const QString &path, QObject *owner)
    : ModemInterfacePrivate(path, owner),
      modemLocationIface(MM_DBUS_SERVICE, path, QDBusConnection::systemBus(), this)
{
}

ModemManager::ModemLocationInterface::ModemLocationInterface(const QString & path, QObject * parent)
    : ModemInterface(*new ModemLocationInterfacePrivate(path, this), parent)
{
    Q_D(ModemLocationInterface);

    QDBusConnection::systemBus().connect(MM_DBUS_SERVICE, d->udi, DBUS_INTERFACE_PROPS, "PropertiesChanged", this,
                                         SLOT(onPropertiesChanged(QString,QVariantMap,QStringList)));
}

ModemManager::ModemLocationInterface::~ModemLocationInterface()
{
}

void ModemManager::ModemLocationInterface::onPropertiesChanged(const QString & interface, const QVariantMap & properties, const QStringList &invalidatedProps)
{
    mmDebug() << interface << properties.keys();

    if (interface == QString(MM_DBUS_INTERFACE_MODEM_LOCATION)) {
        QLatin1String capabilities(MM_MODEM_LOCATION_PROPERTY_CAPABILITIES);
        QLatin1String enabled(MM_MODEM_LOCATION_PROPERTY_ENABLED);
        QLatin1String signalsLocation(MM_MODEM_LOCATION_PROPERTY_SIGNALSLOCATION);
        QLatin1String location(MM_MODEM_LOCATION_PROPERTY_LOCATION);

        QVariantMap::const_iterator it = properties.constFind(capabilities);
        if ( it != properties.constEnd()) {
            emit capabilitiesChanged((ModemManager::ModemLocationInterface::LocationSources)it->toUInt());
        }
        it = properties.constFind(enabled);
        if ( it != properties.constEnd()) {
            emit enabledChanged(it->toBool());
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

void ModemManager::ModemLocationInterface::setup(ModemManager::ModemLocationInterface::LocationSources sources, bool signalLocation)
{
    Q_D(ModemLocationInterface);
    d->modemLocationIface.Setup(sources, signalLocation);
}

LocationInformationMap ModemManager::ModemLocationInterface::location()
{
    Q_D(ModemLocationInterface);
    QDBusReply<LocationInformationMap> location = d->modemLocationIface.GetLocation();

    if (location.isValid()) {
        return location.value();
    }

    return LocationInformationMap();
}

ModemManager::ModemLocationInterface::LocationSources ModemManager::ModemLocationInterface::capabilities() const
{
    Q_D(const ModemLocationInterface);
    return (LocationSources)d->modemLocationIface.capabilities();
}

bool ModemManager::ModemLocationInterface::isEnabled() const
{
    Q_D(const ModemLocationInterface);
    return d->modemLocationIface.enabled();
}

bool ModemManager::ModemLocationInterface::signalsLocation() const
{
    Q_D(const ModemLocationInterface);
    return d->modemLocationIface.signalsLocation();
}
