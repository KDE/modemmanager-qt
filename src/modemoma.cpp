/*
    Copyright 2014 Lukas Tinkl <ltinkl@redhat.com>

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

ModemOmaPrivate::ModemOmaPrivate(const QString &path)
    : InterfacePrivate(path)
    , omaIface(MM_DBUS_SERVICE, path, QDBusConnection::systemBus())
{
}

ModemManager::ModemOma::ModemOma(const QString &path, QObject *parent)
    : Interface(*new ModemOmaPrivate(path), parent)
{
}

ModemManager::ModemOma::~ModemOma()
{
}

ModemManager::ModemOma::Features ModemManager::ModemOma::features() const
{
    Q_D(const ModemOma);

    return (Features)d->omaIface.features();
}

OmaSessionTypes ModemManager::ModemOma::pendingNetworkInitiatedSessions() const
{
    Q_D(const ModemOma);

    OmaSessionTypes result;
    return result;  // TODO
}

MMOmaSessionType ModemManager::ModemOma::sessionType() const
{
    Q_D(const ModemOma);
    return (MMOmaSessionType)d->omaIface.sessionType();
}

MMOmaSessionState ModemManager::ModemOma::sessionState() const
{
    Q_D(const ModemOma);
    return (MMOmaSessionState)d->omaIface.sessionState();
}

void ModemManager::ModemOma::setup(Features features)
{
    Q_D(ModemOma);
    d->omaIface.Setup((uint)features);
}

void ModemManager::ModemOma::startClientInitiatedSession(MMOmaSessionType sessionType)
{
    Q_D(ModemOma);
    d->omaIface.StartClientInitiatedSession((uint)sessionType);
}

void ModemManager::ModemOma::acceptNetworkInitiatedSession(uint sessionId, bool accept)
{
    Q_D(ModemOma);
    d->omaIface.AcceptNetworkInitiatedSession(sessionId, accept);
}

void ModemManager::ModemOma::cancelSession()
{
    Q_D(ModemOma);
    d->omaIface.CancelSession();
}
