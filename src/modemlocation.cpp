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
#include "mmdebug_p.h"
#ifdef MMQT_STATIC
#include "dbus/fakedbus.h"
#else
#include "dbus/dbus.h"
#endif

ModemManager::ModemLocationPrivate::ModemLocationPrivate(const QString &path, ModemLocation *q)
    : InterfacePrivate(path, q)
#ifdef MMQT_STATIC
    , modemLocationIface(MMQT_DBUS_SERVICE, path, QDBusConnection::sessionBus())
#else
    , modemLocationIface(MMQT_DBUS_SERVICE, path, QDBusConnection::systemBus())
#endif
    , q_ptr(q)
{
    if (modemLocationIface.isValid()) {
        capabilities = (ModemManager::ModemLocation::LocationSources)modemLocationIface.capabilities();
        enabledCapabilities = (ModemManager::ModemLocation::LocationSources)modemLocationIface.enabled();
        signalsLocation = modemLocationIface.signalsLocation();
        location = modemLocationIface.location();
    }
}

ModemManager::ModemLocation::ModemLocation(const QString &path, QObject *parent)
    : Interface(*new ModemLocationPrivate(path, this), parent)
{
    Q_D(ModemLocation);

    qRegisterMetaType<LocationSources>();

#ifdef MMQT_STATIC
    QDBusConnection::sessionBus().connect(MMQT_DBUS_SERVICE, d->uni, DBUS_INTERFACE_PROPS, QStringLiteral("PropertiesChanged"), d,
                                         SLOT(onPropertiesChanged(QString,QVariantMap,QStringList)));
#else
    QDBusConnection::systemBus().connect(MMQT_DBUS_SERVICE, d->uni, DBUS_INTERFACE_PROPS, QStringLiteral("PropertiesChanged"), d,
                                         SLOT(onPropertiesChanged(QString,QVariantMap,QStringList)));
#endif
}

ModemManager::ModemLocation::~ModemLocation()
{
}

QDBusPendingReply<void> ModemManager::ModemLocation::setup(ModemManager::ModemLocation::LocationSources sources, bool signalLocation)
{
    Q_D(ModemLocation);
    return d->modemLocationIface.Setup(sources, signalLocation);
}

QDBusPendingReply<ModemManager::LocationInformationMap> ModemManager::ModemLocation::getLocation()
{
    Q_D(ModemLocation);
    return d->modemLocationIface.GetLocation();
}

ModemManager::ModemLocation::LocationSources ModemManager::ModemLocation::capabilities() const
{
    Q_D(const ModemLocation);
    return (LocationSources)d->capabilities;
}

ModemManager::ModemLocation::LocationSources ModemManager::ModemLocation::enabledCapabilities() const
{
    Q_D(const ModemLocation);
    return (LocationSources)d->enabledCapabilities;
}

bool ModemManager::ModemLocation::isEnabled() const
{
    Q_D(const ModemLocation);
    return d->enabledCapabilities > MM_MODEM_LOCATION_SOURCE_NONE;
}

bool ModemManager::ModemLocation::signalsLocation() const
{
    Q_D(const ModemLocation);
    return d->signalsLocation;
}

ModemManager::LocationInformationMap ModemManager::ModemLocation::location() const
{
    Q_D(const ModemLocation);
    return d->location;
}

void ModemManager::ModemLocationPrivate::onPropertiesChanged(const QString &interface, const QVariantMap &properties, const QStringList &invalidatedProps)
{
    Q_Q(ModemLocation);
    Q_UNUSED(invalidatedProps);
    qCDebug(MMQT) << interface << properties.keys();

    if (interface == QString(MMQT_DBUS_INTERFACE_MODEM_LOCATION)) {
        QVariantMap::const_iterator it = properties.constFind(QLatin1String(MM_MODEM_LOCATION_PROPERTY_CAPABILITIES));
        if ( it != properties.constEnd()) {
            capabilities = (ModemManager::ModemLocation::LocationSources)it->toUInt();
            Q_EMIT q->capabilitiesChanged(capabilities);
        }
        it = properties.constFind(QLatin1String(MM_MODEM_LOCATION_PROPERTY_ENABLED));
        if ( it != properties.constEnd()) {
            enabledCapabilities = (ModemManager::ModemLocation::LocationSources)it->toUInt();
            Q_EMIT q->enabledCapabilitiesChanged(enabledCapabilities);
        }
        it = properties.constFind(QLatin1String(MM_MODEM_LOCATION_PROPERTY_SIGNALSLOCATION));
        if ( it != properties.constEnd()) {
            signalsLocation = it->toBool();
            Q_EMIT q->signalsLocationChanged(signalsLocation);
        }
        it = properties.constFind(QLatin1String(MM_MODEM_LOCATION_PROPERTY_LOCATION));
        if ( it != properties.constEnd()) {
            location = qdbus_cast<ModemManager::LocationInformationMap>(*it);
            Q_EMIT q->locationChanged(location);
        }
    }
}
