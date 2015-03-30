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

#include "modemoma.h"

ModemOma::ModemOma(QObject* parent)
    : QDBusAbstractAdaptor(parent)
    , m_enabledNotifications(false)
    , m_features(0)
    , m_sessionState(0)
    , m_sessionType(0)
{
}

ModemOma::~ModemOma()
{
}

uint ModemOma::features() const
{
    return m_features;
}

ModemManager::OmaSessionTypes ModemOma::pendingNetworkInitiatedSessions() const
{
    return m_sessions;
}

int ModemOma::sessionState() const
{
    return m_sessionState;
}

uint ModemOma::sessionType() const
{
    return m_sessionType;
}

void ModemOma::AcceptNetworkInitiatedSession(uint session_id, bool accept)
{
    Q_UNUSED(session_id);
    Q_UNUSED(accept);
}

void ModemOma::CancelSession()
{
}

void ModemOma::Setup(uint features)
{
    Q_UNUSED(features);
}

void ModemOma::StartClientInitiatedSession(uint session_type)
{
    Q_UNUSED(session_type);
}

void ModemOma::setModemPath(const QString& path)
{
    m_modemPath = path;
}

void ModemOma::setEnableNotifications(bool enable)
{
    m_enabledNotifications = enable;
}

void ModemOma::setFeatures(uint features)
{
    m_features = features;

    if (m_enabledNotifications) {
        QVariantMap map;
        map.insert(QLatin1String("Features"), m_features);
        QDBusMessage message = QDBusMessage::createSignal(m_modemPath, QLatin1String("org.freedesktop.DBus.Properties"), QLatin1String("PropertiesChanged"));
        message << QLatin1String("org.kde.fakemodem.Modem.Oma") << map << QStringList();
        QDBusConnection::sessionBus().send(message);
    }
}

void ModemOma::setPendingNetworkInitiatedSessions(const ModemManager::OmaSessionTypes& sessions)
{
    m_sessions = sessions;

    if (m_enabledNotifications) {
        QVariantMap map;
        map.insert(QLatin1String("PendingNetworkInitiatedSessions"), QVariant::fromValue<ModemManager::OmaSessionTypes>(m_sessions));
        QDBusMessage message = QDBusMessage::createSignal(m_modemPath, QLatin1String("org.freedesktop.DBus.Properties"), QLatin1String("PropertiesChanged"));
        message << QLatin1String("org.kde.fakemodem.Modem.Oma") << map << QStringList();
        QDBusConnection::sessionBus().send(message);
    }
}

void ModemOma::setSessionState(int state)
{
    int previousState = m_sessionState;
    m_sessionState = state;

    if (m_enabledNotifications) {
        Q_EMIT SessionStateChanged(previousState, m_sessionState, MM_OMA_SESSION_STATE_UNKNOWN);
    }
}

void ModemOma::setSessionType(uint type)
{
    m_sessionType = type;

    if (m_enabledNotifications) {
        QVariantMap map;
        map.insert(QLatin1String("SessionType"), m_sessionType);
        QDBusMessage message = QDBusMessage::createSignal(m_modemPath, QLatin1String("org.freedesktop.DBus.Properties"), QLatin1String("PropertiesChanged"));
        message << QLatin1String("org.kde.fakemodem.Modem.Oma") << map << QStringList();
        QDBusConnection::sessionBus().send(message);
    }
}

QVariantMap ModemOma::toMap() const
{
    QVariantMap map;
    map.insert(QLatin1String(MM_MODEM_OMA_PROPERTY_FEATURES), m_features);
    map.insert(QLatin1String(MM_MODEM_OMA_PROPERTY_PENDINGNETWORKINITIATEDSESSIONS), QVariant::fromValue<ModemManager::OmaSessionTypes>(m_sessions));
    map.insert(QLatin1String(MM_MODEM_OMA_PROPERTY_SESSIONTYPE), m_sessionType);
    map.insert(QLatin1String(MM_MODEM_OMA_PROPERTY_SESSIONSTATE), m_sessionState);
    return map;
}
