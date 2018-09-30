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

#include "modemsimple_p.h"

#include "generictypes.h"
#include "mmdebug_p.h"
#ifdef MMQT_STATIC
#include "dbus/fakedbus.h"
#else
#include "dbus/dbus.h"
#endif

ModemManager::ModemSimplePrivate::ModemSimplePrivate(const QString &path, ModemSimple *q)
    : InterfacePrivate(path, q)
#ifdef MMQT_STATIC
    , modemSimpleIface(QLatin1String(MMQT_DBUS_SERVICE), path, QDBusConnection::sessionBus())
#else
    , modemSimpleIface(QLatin1String(MMQT_DBUS_SERVICE), path, QDBusConnection::systemBus())
#endif
    , q_ptr(q)
{
}

ModemManager::ModemSimple::ModemSimple(const QString &path, QObject *parent)
    : Interface(*new ModemSimplePrivate(path, this), parent)
{
}

ModemManager::ModemSimple::~ModemSimple()
{
}

QString ModemManager::ModemSimple::uni() const
{
    Q_D(const ModemSimple);
    return d->uni;
}

QDBusPendingReply<QDBusObjectPath> ModemManager::ModemSimple::connectModem(const QVariantMap &properties)
{
    Q_D(ModemSimple);

    return d->modemSimpleIface.Connect(properties);
}

QDBusPendingReply<QVariantMap> ModemManager::ModemSimple::getStatus()
{
    Q_D(ModemSimple);
    return d->modemSimpleIface.GetStatus();
}

QDBusPendingReply<void> ModemManager::ModemSimple::disconnectModem(const QString &bearer)
{
    Q_D(ModemSimple);
    return d->modemSimpleIface.Disconnect(QDBusObjectPath(bearer));
}

QDBusPendingReply<void> ModemManager::ModemSimple::disconnectAllModems()
{
    return disconnectModem(QStringLiteral("/"));
}
