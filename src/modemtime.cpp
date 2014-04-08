/*
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

#include "modemtime.h"
#include "modemtime_p.h"
#include "mmdebug.h"

ModemTimePrivate::ModemTimePrivate(const QString &path)
    : InterfacePrivate(path)
    , modemTimeIface(MM_DBUS_SERVICE, path, QDBusConnection::systemBus())
{
}

ModemManager::ModemTime::ModemTime(const QString &path, QObject *parent)
    : Interface(*new ModemTimePrivate(path), parent)
{
    Q_D(ModemTime);

    connect(&d->modemTimeIface, &OrgFreedesktopModemManager1ModemTimeInterface::NetworkTimeChanged, this, &ModemTime::onNetworkTimeChanged);
}

ModemManager::ModemTime::~ModemTime()
{
}

QDateTime ModemManager::ModemTime::networkTime()
{
    Q_D(ModemTime);
    QDBusPendingReply<QString> reply = d->modemTimeIface.GetNetworkTime();
    reply.waitForFinished();
    if (reply.isValid()) {
        return QDateTime::fromString(reply.value(), Qt::ISODate);
    }

    return QDateTime();
}

ModemManager::ModemTime::NetworkTimeZone ModemManager::ModemTime::networkTimeZone() const
{
    Q_D(const ModemTime);

    NetworkTimeZone result;
    const QVariantMap map = d->modemTimeIface.networkTimezone();
    if (map.contains("offset"))
        result.offset = map.value("offset").toInt();
    if (map.contains("dst-offset"))
        result.dst_offset = map.value("dst-offset").toInt();
    if (map.contains("leap-seconds"))
        result.leap_seconds = map.value("leap-seconds").toInt();

    return result;
}


void ModemManager::ModemTime::onNetworkTimeChanged(const QString &isoDateTime)
{
    const QDateTime result = QDateTime::fromString(isoDateTime, Qt::ISODate);
    if (result.isValid())
        emit networkTimeChanged(result);
}
