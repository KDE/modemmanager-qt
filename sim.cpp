/*
Copyright 2008,2011 Will Stephenson <wstephenson@kde.org>
Copyright 2010-2011 Lamarque Souza <lamarque@kde.org>
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

#include "sim.h"
#include "sim_p.h"
#include "mmdebug.h"

SimPrivate::SimPrivate(const QString &path)
    : simIface(MM_DBUS_SERVICE, path, QDBusConnection::systemBus())
    , uni(path)
{
}

ModemManager::Sim::Sim(const QString &path, QObject *parent)
    : QObject(parent)
    , d_ptr(new SimPrivate(path))
{
}

ModemManager::Sim::~Sim()
{
    delete d_ptr;
}

QString ModemManager::Sim::simIdentifier() const
{
    Q_D(const Sim);
    return d->simIface.simIdentifier();
}

QString ModemManager::Sim::imsi() const
{
    Q_D(const Sim);
    return d->simIface.imsi();
}

QString ModemManager::Sim::operatorIdentifier() const
{
    Q_D(const Sim);
    return d->simIface.operatorIdentifier();
}

QString ModemManager::Sim::operatorName() const
{
    Q_D(const Sim);
    return d->simIface.operatorName();
}

QDBusPendingReply<> ModemManager::Sim::sendPuk(const QString &puk, const QString &pin)
{
    Q_D(Sim);
    return d->simIface.SendPuk(puk, pin);
}

QDBusPendingReply<> ModemManager::Sim::sendPin(const QString &pin)
{
    Q_D(Sim);
    return d->simIface.SendPin(pin);
}

QDBusPendingReply<> ModemManager::Sim::enablePin(const QString &pin, bool enabled)
{
    Q_D(Sim);
    return d->simIface.EnablePin(pin, enabled);
}

QDBusPendingReply<> ModemManager::Sim::changePin(const QString &oldPin, const QString &newPin)
{
    Q_D(Sim);
    return d->simIface.ChangePin(oldPin, newPin);
}

QString ModemManager::Sim::uni() const
{
    Q_D(const Sim);
    return d->uni;
}
