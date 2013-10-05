/*
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

#include "modemtimeinterface.h"
#include "modemtimeinterface_p.h"
#include "mmdebug.h"

ModemTimeInterfacePrivate::ModemTimeInterfacePrivate(const QString &path)
    : InterfacePrivate(path), modemTimeIface(MM_DBUS_SERVICE, path, QDBusConnection::systemBus())
{
}

ModemManager::ModemTimeInterface::ModemTimeInterface(const QString & path, QObject * parent)
    : Interface(*new ModemTimeInterfacePrivate(path), parent)
{
    Q_D(ModemTimeInterface);

    connect(&d->modemTimeIface, SIGNAL(NetworkTimeChanged(QString)), SLOT(onNetworkTimeChanged(QString)));
}

ModemManager::ModemTimeInterface::~ModemTimeInterface()
{
}

QDateTime ModemManager::ModemTimeInterface::networkTime()
{
    Q_D(ModemTimeInterface);
    QDBusPendingReply<QString> reply = d->modemTimeIface.GetNetworkTime();
    reply.waitForFinished();
    if (reply.isValid()) {
        return QDateTime::fromString(reply.value(), Qt::ISODate);
    }

    return QDateTime();
}

ModemManager::ModemTimeInterface::NetworkTimeZone ModemManager::ModemTimeInterface::networkTimeZone() const
{
    Q_D(const ModemTimeInterface);

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


void ModemManager::ModemTimeInterface::onNetworkTimeChanged(const QString &isoDateTime)
{
    Q_D(ModemTimeInterface);

    const QDateTime result = QDateTime::fromString(isoDateTime, Qt::ISODate);
    if (result.isValid())
        emit networkTimeChanged(result);
}
