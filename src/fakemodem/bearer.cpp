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

#include "bearer.h"

Bearer::Bearer(QObject* parent)
    : QObject(parent)
    , m_connected(false)
    , m_ipTimeout(0)
    , m_suspended(false)
{
}

Bearer::~Bearer()
{
}

bool Bearer::connected() const
{
    return m_connected;
}

QString Bearer::interface() const
{
    return m_interface;
}

QVariantMap Bearer::ip4Config() const
{
    return m_ip4Config;
}

QVariantMap Bearer::ip6Config() const
{
    return m_ip6Config;
}

uint Bearer::ipTimeout() const
{
    return m_ipTimeout;
}

QVariantMap Bearer::properties() const
{
    return m_properties;
}

bool Bearer::suspended() const
{
    return m_suspended;
}

QString Bearer::bearerPath() const
{
    return m_bearerPath;
}

void Bearer::setBearerPath(const QString& path)
{
    m_bearerPath = path;
}

void Bearer::setEnableNotifications(bool enable)
{
    m_enabledNotifications = enable;
}

void Bearer::setConnected(bool connected)
{
    m_connected = connected;

    if (m_enabledNotifications) {
        QVariantMap map;
        map.insert(QLatin1String("Connected"), m_connected);
        QDBusMessage message = QDBusMessage::createSignal(m_bearerPath, QLatin1String("org.freedesktop.DBus.Properties"), QLatin1String("PropertiesChanged"));
        message << QLatin1String("org.kde.fakemodem.Bearer") << map << QStringList();
        QDBusConnection::sessionBus().send(message);
    }
}

void Bearer::setInterface(const QString& interface)
{
    m_interface = interface;

    if (m_enabledNotifications) {
        QVariantMap map;
        map.insert(QLatin1String("Interface"), m_interface);
        QDBusMessage message = QDBusMessage::createSignal(m_bearerPath, QLatin1String("org.freedesktop.DBus.Properties"), QLatin1String("PropertiesChanged"));
        message << QLatin1String("org.kde.fakemodem.Bearer") << map << QStringList();
        QDBusConnection::sessionBus().send(message);
    }
}

void Bearer::setIp4Config(const QVariantMap& config)
{
    m_ip4Config = config;

    if (m_enabledNotifications) {
        QVariantMap map;
        map.insert(QLatin1String("Ip4Config"), QVariant::fromValue<QVariantMap>(m_ip4Config));
        QDBusMessage message = QDBusMessage::createSignal(m_bearerPath, QLatin1String("org.freedesktop.DBus.Properties"), QLatin1String("PropertiesChanged"));
        message << QLatin1String("org.kde.fakemodem.Bearer") << map << QStringList();
        QDBusConnection::sessionBus().send(message);
    }
}

void Bearer::setIp6Config(const QVariantMap& config)
{
    m_ip6Config = config;

    if (m_enabledNotifications) {
        QVariantMap map;
        map.insert(QLatin1String("Ip6Config"), QVariant::fromValue<QVariantMap>(m_ip6Config));
        QDBusMessage message = QDBusMessage::createSignal(m_bearerPath, QLatin1String("org.freedesktop.DBus.Properties"), QLatin1String("PropertiesChanged"));
        message << QLatin1String("org.kde.fakemodem.Bearer") << map << QStringList();
        QDBusConnection::sessionBus().send(message);
    }
}

void Bearer::setIpTimeout(uint timeout)
{
    m_ipTimeout = timeout;

    if (m_enabledNotifications) {
        QVariantMap map;
        map.insert(QLatin1String("IpTimeout"), m_ipTimeout);
        QDBusMessage message = QDBusMessage::createSignal(m_bearerPath, QLatin1String("org.freedesktop.DBus.Properties"), QLatin1String("PropertiesChanged"));
        message << QLatin1String("org.kde.fakemodem.Bearer") << map << QStringList();
        QDBusConnection::sessionBus().send(message);
    }
}

void Bearer::setProperties(const QVariantMap& properties)
{
    m_properties = properties;

    if (m_enabledNotifications) {
        QVariantMap map;
        map.insert(QLatin1String("Properties"), QVariant::fromValue<QVariantMap>(m_properties));
        QDBusMessage message = QDBusMessage::createSignal(m_bearerPath, QLatin1String("org.freedesktop.DBus.Properties"), QLatin1String("PropertiesChanged"));
        message << QLatin1String("org.kde.fakemodem.Bearer") << map << QStringList();
        QDBusConnection::sessionBus().send(message);
    }
}

void Bearer::setSuspended(bool suspended)
{
    m_suspended = suspended;

    if (m_enabledNotifications) {
        QVariantMap map;
        map.insert(QLatin1String("Suspended"), m_suspended);
        QDBusMessage message = QDBusMessage::createSignal(m_bearerPath, QLatin1String("org.freedesktop.DBus.Properties"), QLatin1String("PropertiesChanged"));
        message << QLatin1String("org.kde.fakemodem.Bearer") << map << QStringList();
        QDBusConnection::sessionBus().send(message);
    }
}

void Bearer::Connect()
{
    // TODO
}

void Bearer::Disconnect()
{
    // TODO
}
