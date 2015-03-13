/*
    Copyright 2013 Lukas Tinkl <ltinkl@redhat.com>
    Copyright 2013-2015 Jan Grulich <jgrulich@redhat.com>

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
#include "mmdebug_p.h"
#ifdef MMQT_STATIC
#include "dbus/fakedbus.h"
#else
#include "dbus/dbus.h"
#endif

ModemManager::ModemTimePrivate::ModemTimePrivate(const QString &path, ModemTime *q)
    : InterfacePrivate(path, q)
#ifdef MMQT_STATIC
    , modemTimeIface(MMQT_DBUS_SERVICE, path, QDBusConnection::sessionBus())
#else
    , modemTimeIface(MMQT_DBUS_SERVICE, path, QDBusConnection::systemBus())
#endif
    , q_ptr(q)
{
    if (modemTimeIface.isValid()) {
        networkTimeZone = variantMapToTimeZone(modemTimeIface.networkTimezone());
    }
}

ModemManager::ModemTime::ModemTime(const QString &path, QObject *parent)
    : Interface(*new ModemTimePrivate(path, this), parent)
{
    Q_D(ModemTime);

    connect(&d->modemTimeIface, &OrgFreedesktopModemManager1ModemTimeInterface::NetworkTimeChanged, d, &ModemTimePrivate::onNetworkTimeChanged);
#ifdef MMQT_STATIC
    QDBusConnection::sessionBus().connect(MMQT_DBUS_SERVICE, d->uni, DBUS_INTERFACE_PROPS, QStringLiteral("PropertiesChanged"), d,
                                         SLOT(onPropertiesChanged(QString,QVariantMap,QStringList)));
#else
    QDBusConnection::systemBus().connect(MMQT_DBUS_SERVICE, d->uni, DBUS_INTERFACE_PROPS, QStringLiteral("PropertiesChanged"), d,
                                         SLOT(onPropertiesChanged(QString,QVariantMap,QStringList)));
#endif
}

ModemManager::ModemTime::~ModemTime()
{
}

QDBusPendingReply<QString> ModemManager::ModemTime::networkTime()
{
    Q_D(ModemTime);

    return d->modemTimeIface.GetNetworkTime();
}

ModemManager::ModemTime::NetworkTimeZone ModemManager::ModemTime::networkTimeZone() const
{
    Q_D(const ModemTime);

    return d->networkTimeZone;
}

ModemManager::ModemTime::NetworkTimeZone ModemManager::ModemTimePrivate::variantMapToTimeZone(const QVariantMap &map)
{
    ModemManager::ModemTime::NetworkTimeZone result;
    if (map.contains("offset")) {
        result.offset = map.value("offset").toInt();
    } if (map.contains("dst-offset")) {
        result.dst_offset = map.value("dst-offset").toInt();
    } if (map.contains("leap-seconds")) {
        result.leap_seconds = map.value("leap-seconds").toInt();
    }

    return result;
}

void ModemManager::ModemTimePrivate::onNetworkTimeChanged(const QString &isoDateTime)
{
    Q_Q(ModemTime);

    const QDateTime result = QDateTime::fromString(isoDateTime, Qt::ISODate);
    if (result.isValid())
        Q_EMIT q->networkTimeChanged(result);
}

void ModemManager::ModemTimePrivate::onPropertiesChanged(const QString &interface, const QVariantMap &properties, const QStringList &invalidatedProps)
{
    Q_Q(ModemTime);
    Q_UNUSED(invalidatedProps);
    qCDebug(MMQT) << interface << properties.keys();

    if (interface == QString(MMQT_DBUS_INTERFACE_MODEM_TIME)) {
        QVariantMap::const_iterator it = properties.constFind(QLatin1String(MM_MODEM_TIME_PROPERTY_NETWORKTIMEZONE));
        if (it != properties.constEnd()) {
            networkTimeZone = variantMapToTimeZone(it->toMap());
            Q_EMIT q->networkTimeZoneChanged(networkTimeZone);
        }
    }
}
