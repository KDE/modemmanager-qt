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

#include "modemtime.h"

ModemTime::ModemTime(QObject* parent)
    : QDBusAbstractAdaptor(parent)
    , m_enabledNotifications(false)
{
}

ModemTime::~ModemTime()
{
}

QVariantMap ModemTime::networkTimezone() const
{
    return m_timezone;
}

QString ModemTime::GetNetworkTime()
{
    return "2010-08-06T18:34:14.3+02:00";
}

void ModemTime::setModemPath(const QString& path)
{
    m_modemPath = path;
}

void ModemTime::setEnableNotifications(bool enable)
{
    m_enabledNotifications = enable;
}

void ModemTime::setNetworkTimezone(const QVariantMap& timezone)
{
    m_timezone = timezone;

    if (m_enabledNotifications) {
        QVariantMap map;
        map.insert(QLatin1String("NetworkTimezone"), QVariant::fromValue<QVariantMap>(m_timezone));
        QDBusMessage message = QDBusMessage::createSignal(m_modemPath, QLatin1String("org.freedesktop.DBus.Properties"), QLatin1String("PropertiesChanged"));
        message << QLatin1String("org.kde.fakemodem.Modem.Time") << map << QStringList();
        QDBusConnection::sessionBus().send(message);
    }
}

QVariantMap ModemTime::toMap() const
{
    QVariantMap map;
    map.insert(QLatin1String(MM_MODEM_TIME_PROPERTY_NETWORKTIMEZONE), QVariant::fromValue<QVariantMap>(m_timezone));
    return map;
}
