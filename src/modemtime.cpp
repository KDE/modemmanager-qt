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

namespace ModemManager
{

class ModemManager::NetworkTimezone::Private
{
public:
    Private()
    { }
    int offset;
    int dstOffset;
    int leapSecond;
};

}

ModemManager::NetworkTimezone::NetworkTimezone()
    : d(new Private())
{
}

ModemManager::NetworkTimezone::NetworkTimezone(const ModemManager::NetworkTimezone& other)
    : d(new Private)
{
    *this = other;
}

ModemManager::NetworkTimezone::~NetworkTimezone()
{
    delete d;
}

int ModemManager::NetworkTimezone::offset() const
{
    return d->offset;
}

void ModemManager::NetworkTimezone::setOffset(int offset)
{
    d->offset = offset;
}

int ModemManager::NetworkTimezone::dstOffset() const
{
    return d->dstOffset;
}

void ModemManager::NetworkTimezone::setDstOffset(int dstOffset)
{
    d->dstOffset = dstOffset;
}

int ModemManager::NetworkTimezone::leapSecond() const
{
    return d->leapSecond;
}

void ModemManager::NetworkTimezone::setLeapSecond(int leapSecond)
{
    d->leapSecond = leapSecond;
}

ModemManager::NetworkTimezone& ModemManager::NetworkTimezone::operator=(const ModemManager::NetworkTimezone& other)
{
    if (this == &other) {
        return *this;
    }

    *d = *other.d;
    return *this;
}

ModemManager::ModemTimePrivate::ModemTimePrivate(const QString &path, ModemTime *q)
    : InterfacePrivate(path, q)
#ifdef MMQT_STATIC
    , modemTimeIface(QLatin1String(MMQT_DBUS_SERVICE), path, QDBusConnection::sessionBus())
#else
    , modemTimeIface(QLatin1String(MMQT_DBUS_SERVICE), path, QDBusConnection::systemBus())
#endif
    , q_ptr(q)
{
    if (modemTimeIface.isValid()) {
        networkTimezone = variantMapToTimezone(modemTimeIface.networkTimezone());
    }
}

ModemManager::ModemTime::ModemTime(const QString &path, QObject *parent)
    : Interface(*new ModemTimePrivate(path, this), parent)
{
    Q_D(ModemTime);

    connect(&d->modemTimeIface, &OrgFreedesktopModemManager1ModemTimeInterface::NetworkTimeChanged, d, &ModemTimePrivate::onNetworkTimeChanged);
#ifdef MMQT_STATIC
    QDBusConnection::sessionBus().connect(QLatin1String(MMQT_DBUS_SERVICE), d->uni, QLatin1String(DBUS_INTERFACE_PROPS), QStringLiteral("PropertiesChanged"), d,
                                         SLOT(onPropertiesChanged(QString,QVariantMap,QStringList)));
#else
    QDBusConnection::systemBus().connect(QLatin1String(MMQT_DBUS_SERVICE), d->uni, QLatin1String(DBUS_INTERFACE_PROPS), QStringLiteral("PropertiesChanged"), d,
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

ModemManager::NetworkTimezone ModemManager::ModemTime::networkTimezone() const
{
    Q_D(const ModemTime);

    return d->networkTimezone;
}

ModemManager::NetworkTimezone ModemManager::ModemTimePrivate::variantMapToTimezone(const QVariantMap &map)
{
    ModemManager::NetworkTimezone result;
    if (map.contains(QLatin1String("offset"))) {
        result.setOffset(map.value(QStringLiteral("offset")).toInt());
    } if (map.contains(QLatin1String("dst-offset"))) {
        result.setDstOffset(map.value(QStringLiteral("dst-offset")).toInt());
    } if (map.contains(QLatin1String("leap-seconds"))) {
        result.setLeapSecond(map.value(QStringLiteral("leap-seconds")).toInt());
    }

    return result;
}

void ModemManager::ModemTime::setTimeout(int timeout)
{
    Q_D(ModemTime);
    d->modemTimeIface.setTimeout(timeout);
}

int ModemManager::ModemTime::timeout() const
{
    Q_D(const ModemTime);
    return d->modemTimeIface.timeout();
}

void ModemManager::ModemTimePrivate::onNetworkTimeChanged(const QString &isoDateTime)
{
    Q_Q(ModemTime);

    const QDateTime result = QDateTime::fromString(isoDateTime, Qt::ISODate);
    if (result.isValid()) {
        Q_EMIT q->networkTimeChanged(result);
    }
}

void ModemManager::ModemTimePrivate::onPropertiesChanged(const QString &interface, const QVariantMap &properties, const QStringList &invalidatedProps)
{
    Q_Q(ModemTime);
    Q_UNUSED(invalidatedProps);
    qCDebug(MMQT) << interface << properties.keys();

    if (interface == QLatin1String(MMQT_DBUS_INTERFACE_MODEM_TIME)) {
        QVariantMap::const_iterator it = properties.constFind(QLatin1String(MM_MODEM_TIME_PROPERTY_NETWORKTIMEZONE));
        if (it != properties.constEnd()) {
            networkTimezone = variantMapToTimezone(qdbus_cast<QVariantMap>(*it));
            Q_EMIT q->networkTimezoneChanged(networkTimezone);
        }
    }
}
