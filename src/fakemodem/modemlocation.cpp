/*
    Copyright 2015 Jan Grulich <jgrulich@redhat.com>

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

ModemLocation::ModemLocation(QObject* parent)
    : QDBusAbstractAdaptor(parent)
    , m_enabledNotifications(false)
    , m_capabilities(0)
    , m_enabled(0)
    , m_signalsLocation(false)
{
}

ModemLocation::~ModemLocation()
{
}

uint ModemLocation::capabilities() const
{
    return m_capabilities;
}

uint ModemLocation::enabled() const
{
    return m_enabled;
}

ModemManager::LocationInformationMap ModemLocation::location() const
{
    return m_location;
}

bool ModemLocation::signalsLocation() const
{
    return m_signalsLocation;
}

ModemManager::LocationInformationMap ModemLocation::GetLocation()
{
    return m_location;
}

void ModemLocation::Setup(uint sources, bool signal_location)
{
    Q_UNUSED(sources);
    Q_UNUSED(signal_location)
}

void ModemLocation::setModemPath(const QString& path)
{
    m_modemPath = path;
}

void ModemLocation::setEnableNotifications(bool enable)
{
    m_enabledNotifications = enable;
}

void ModemLocation::setCapabilities(uint capabilities)
{
    m_capabilities = capabilities;

    if (m_enabledNotifications) {
        QVariantMap map;
        map.insert(QLatin1String("Capabilities"), m_capabilities);
        QDBusMessage message = QDBusMessage::createSignal(m_modemPath, QLatin1String("org.freedesktop.DBus.Properties"), QLatin1String("PropertiesChanged"));
        message << QLatin1String("org.kde.fakemodem.Modem.Location") << map << QStringList();
        QDBusConnection::sessionBus().send(message);
    }
}

void ModemLocation::setEnabled(uint enabled)
{
    m_enabled = enabled;

    if (m_enabledNotifications) {
        QVariantMap map;
        map.insert(QLatin1String("Enabled"), m_enabled);
        QDBusMessage message = QDBusMessage::createSignal(m_modemPath, QLatin1String("org.freedesktop.DBus.Properties"), QLatin1String("PropertiesChanged"));
        message << QLatin1String("org.kde.fakemodem.Modem.Location") << map << QStringList();
        QDBusConnection::sessionBus().send(message);
    }
}

void ModemLocation::setLocation(const ModemManager::LocationInformationMap& location)
{
    m_location = location;

    if (m_enabledNotifications) {
        QVariantMap map;
        map.insert(QLatin1String("Location"), QVariant::fromValue<ModemManager::LocationInformationMap>(location));
        QDBusMessage message = QDBusMessage::createSignal(m_modemPath, QLatin1String("org.freedesktop.DBus.Properties"), QLatin1String("PropertiesChanged"));
        message << QLatin1String("org.kde.fakemodem.Modem.Location") << map << QStringList();
        QDBusConnection::sessionBus().send(message);
    }
}

void ModemLocation::setSignalsLocation(bool signalsLocation)
{
    m_signalsLocation = signalsLocation;

    if (m_enabledNotifications) {
        QVariantMap map;
        map.insert(QLatin1String("SignalsLocation"), m_signalsLocation);
        QDBusMessage message = QDBusMessage::createSignal(m_modemPath, QLatin1String("org.freedesktop.DBus.Properties"), QLatin1String("PropertiesChanged"));
        message << QLatin1String("org.kde.fakemodem.Modem.Location") << map << QStringList();
        QDBusConnection::sessionBus().send(message);
    }
}

QVariantMap ModemLocation::toMap() const
{
    QVariantMap map;
    map.insert(QLatin1String(MM_MODEM_LOCATION_PROPERTY_CAPABILITIES), m_capabilities);
    map.insert(QLatin1String(MM_MODEM_LOCATION_PROPERTY_ENABLED), m_enabled);
    map.insert(QLatin1String(MM_MODEM_LOCATION_PROPERTY_SIGNALSLOCATION), QVariant::fromValue<ModemManager::LocationInformationMap>(m_location));
    map.insert(QLatin1String(MM_MODEM_LOCATION_PROPERTY_LOCATION), m_signalsLocation);
    return map;
}
