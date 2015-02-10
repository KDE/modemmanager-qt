/*
    Copyright 2014 Lukas Tinkl <ltinkl@redhat.com>
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
#include "modemoma_p.h"
#include "mmdebug.h"
#include "dbus/dbus.h"

ModemManager::ModemOmaPrivate::ModemOmaPrivate(const QString &path, ModemOma *q)
    : InterfacePrivate(path, q)
    , omaIface(MM_DBUS_SERVICE, path, QDBusConnection::systemBus())
    , q_ptr(q)
{
    if (omaIface.isValid()) {
        features = (ModemOma::Features)omaIface.features();
        pendingNetworkInitiatedSessions = omaIface.pendingNetworkInitiatedSessions();
        sessionType = (MMOmaSessionType)omaIface.sessionType();
        sessionState = (MMOmaSessionState)omaIface.sessionState();
    }
}

ModemManager::ModemOma::ModemOma(const QString &path, QObject *parent)
    : Interface(*new ModemOmaPrivate(path, this), parent)
{
    Q_D(ModemOma);
    connect(&d->omaIface, &OrgFreedesktopModemManager1ModemOmaInterface::SessionStateChanged, d, &ModemOmaPrivate::onSessionStateChanged);
    QDBusConnection::systemBus().connect(MM_DBUS_SERVICE, d->uni, DBUS_INTERFACE_PROPS, QStringLiteral("PropertiesChanged"), d,
                                        SLOT(onPropertiesChanged(QString,QVariantMap,QStringList)));
}

ModemManager::ModemOma::~ModemOma()
{
}

ModemManager::ModemOma::Features ModemManager::ModemOma::features() const
{
    Q_D(const ModemOma);

    return d->features;
}

OmaSessionTypes ModemManager::ModemOma::pendingNetworkInitiatedSessions() const
{
    Q_D(const ModemOma);
    return d->pendingNetworkInitiatedSessions;
}

MMOmaSessionType ModemManager::ModemOma::sessionType() const
{
    Q_D(const ModemOma);
    return d->sessionType;
}

MMOmaSessionState ModemManager::ModemOma::sessionState() const
{
    Q_D(const ModemOma);
    return d->sessionState;
}

QDBusPendingReply<void> ModemManager::ModemOma::setup(Features features)
{
    Q_D(ModemOma);
    return d->omaIface.Setup((uint)features);
}

QDBusPendingReply<void> ModemManager::ModemOma::startClientInitiatedSession(MMOmaSessionType sessionType)
{
    Q_D(ModemOma);
    return d->omaIface.StartClientInitiatedSession((uint)sessionType);
}

QDBusPendingReply<void> ModemManager::ModemOma::acceptNetworkInitiatedSession(uint sessionId, bool accept)
{
    Q_D(ModemOma);
    return d->omaIface.AcceptNetworkInitiatedSession(sessionId, accept);
}

QDBusPendingReply<void> ModemManager::ModemOma::cancelSession()
{
    Q_D(ModemOma);
    return d->omaIface.CancelSession();
}

void ModemManager::ModemOmaPrivate::onSessionStateChanged(int oldState, int newState, uint failedReason)
{
    Q_Q(ModemOma);
    Q_EMIT q->sessionStateChanged((MMOmaSessionState)oldState, (MMOmaSessionState)newState, (MMOmaSessionStateFailedReason)failedReason);
}
void ModemManager::ModemOmaPrivate::onPropertiesChanged(const QString &interface, const QVariantMap &properties, const QStringList &invalidatedProps)
{
    Q_Q(ModemOma);
    Q_UNUSED(invalidatedProps);
    qCDebug(MMQT) << interface << properties.keys();

    if (interface == QString(MM_DBUS_INTERFACE_MODEM_OMA)) {
        QVariantMap::const_iterator it = properties.constFind(QLatin1String(MM_MODEM_OMA_PROPERTY_FEATURES));
        if (it != properties.constEnd()) {
            features = (ModemOma::Features)it->toUInt();
            Q_EMIT q->featuresChanged(features);
        }
        it = properties.constFind(QLatin1String(MM_MODEM_OMA_PROPERTY_PENDINGNETWORKINITIATEDSESSIONS));
        if (it != properties.constEnd()) {
            pendingNetworkInitiatedSessions = qdbus_cast<OmaSessionTypes>(it.value());
            Q_EMIT q->pendingNetworkInitiatedSessionsChanged(pendingNetworkInitiatedSessions);
        }
        it = properties.constFind(QLatin1String(MM_MODEM_OMA_PROPERTY_SESSIONTYPE));
        if (it != properties.constEnd()) {
            sessionType = (MMOmaSessionType)it->toUInt();
            Q_EMIT q->sessionTypeChanged(sessionType);
        }
        it = properties.constFind(QLatin1String(MM_MODEM_OMA_PROPERTY_SESSIONSTATE));
        if (it != properties.constEnd()) {
            // Should be handled by sessionStateChanged() signal
        }
    }
}
