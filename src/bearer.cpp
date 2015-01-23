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

#include "bearer_p.h"
#include "mmdebug.h"
#include "dbus/dbus.h"

Q_LOGGING_CATEGORY(MMQT, "modemmanager-qt")

ModemManager::BearerPrivate::BearerPrivate(const QString &path, Bearer *q)
    : bearerIface(MM_DBUS_SERVICE, path, QDBusConnection::systemBus())
    , uni(path)
    , q_ptr(q)
{
    if (bearerIface.isValid()) {
        bearerInterface = bearerIface.interface();
        isConnected = bearerIface.connected();
        isSuspended = bearerIface.suspended();
        ipv4Config = ipConfigFromMap(bearerIface.ip4Config());
        ipv6Config = ipConfigFromMap(bearerIface.ip6Config());
        ipTimeout = bearerIface.ipTimeout();
        bearerProperties = bearerIface.properties();
    }
}

ModemManager::Bearer::Bearer(const QString &path, QObject *parent)
    : QObject(parent)
    , d_ptr(new BearerPrivate(path, this))
{
    Q_D(Bearer);
    QDBusConnection::systemBus().connect(MM_DBUS_SERVICE, path, DBUS_INTERFACE_PROPS, QStringLiteral("PropertiesChanged"), d,
                                         SLOT(onPropertiesChanged(QString,QVariantMap,QStringList)));
}

ModemManager::Bearer::~Bearer()
{
    delete d_ptr;
}

QString ModemManager::Bearer::interface() const
{
    Q_D(const Bearer);
    return d->bearerInterface;
}

bool ModemManager::Bearer::isConnected() const
{
    Q_D(const Bearer);
    return d->isConnected;
}

bool ModemManager::Bearer::isSuspended() const
{
    Q_D(const Bearer);
    return d->isSuspended;
}

ModemManager::Bearer::IpConfig ModemManager::Bearer::ip4Config() const
{
    Q_D(const Bearer);
    return d->ipv4Config;
}

ModemManager::Bearer::IpConfig ModemManager::Bearer::ip6Config() const
{
    Q_D(const Bearer);
    return d->ipv6Config;
}

uint ModemManager::Bearer::ipTimeout() const
{
    Q_D(const Bearer);
    return d->ipTimeout;
}

QVariantMap ModemManager::Bearer::properties() const
{
    Q_D(const Bearer);
    return d->bearerProperties;
}

QDBusPendingReply<void> ModemManager::Bearer::connectBearer()
{
    Q_D(Bearer);
    return d->bearerIface.Connect();
}

QDBusPendingReply<void> ModemManager::Bearer::disconnectBearer()
{
    Q_D(Bearer);
    return d->bearerIface.Disconnect();
}

ModemManager::Bearer::IpConfig ModemManager::BearerPrivate::ipConfigFromMap(const QVariantMap &map)
{
    Bearer::IpConfig result;
    result.method = (MMBearerIpMethod)map.value("method").toUInt();

    if (result.method == MM_BEARER_IP_METHOD_STATIC) {
        result.address = map.value("address").toString();
        result.prefix = map.value("prefix").toUInt();
        result.dns1 = map.value("dns1").toString();
        result.dns2 = map.value("dns2").toString();
        result.dns3 = map.value("dns3").toString();
        result.gateway = map.value("gateway").toString();
    }

    return result;
}

void ModemManager::BearerPrivate::onPropertiesChanged(const QString &interface, const QVariantMap &properties, const QStringList &invalidatedProps)
{
    Q_Q(Bearer);
    Q_UNUSED(invalidatedProps);
    qCDebug(MMQT) << interface << properties.keys();

    if (interface == QString(MM_DBUS_INTERFACE_BEARER)) {
        QVariantMap::const_iterator it = properties.constFind(QLatin1String(MM_BEARER_PROPERTY_INTERFACE));
        if (it != properties.constEnd()) {
            bearerInterface = it->toString();
            emit q->interfaceChanged(bearerInterface);
        }
        it = properties.constFind(QLatin1String(MM_BEARER_PROPERTY_CONNECTED));
        if (it != properties.constEnd()) {
            isConnected = it->toBool();
            emit q->connectedChanged(isConnected);
        }
        it = properties.constFind(QLatin1String(MM_BEARER_PROPERTY_SUSPENDED));
        if (it != properties.constEnd()) {
            isSuspended = it->toBool();
            emit q->suspendedChanged(isSuspended);
        }
        it = properties.constFind(QLatin1String(MM_BEARER_PROPERTY_IP4CONFIG));
        if (it != properties.constEnd()) {
            ipv4Config = ipConfigFromMap(it->toMap());
            emit q->ip4ConfigChanged(ipv4Config);
        }
        it = properties.constFind(QLatin1String(MM_BEARER_PROPERTY_IP6CONFIG));
        if (it != properties.constEnd()) {
            ipv6Config = ipConfigFromMap(it->toMap());
            emit q->ip6ConfigChanged(ipv6Config);
        }
        it = properties.constFind(QLatin1String(MM_BEARER_PROPERTY_IPTIMEOUT));
        if (it != properties.constEnd()) {
            ipTimeout = it->toUInt();
            emit q->ipTimeoutChanged(ipTimeout);
        }
        it = properties.constFind(QLatin1String(MM_BEARER_PROPERTY_PROPERTIES));
        if (it != properties.constEnd()) {
            bearerProperties = it->toMap();
            emit q->propertiesChanged(bearerProperties);
        }
    }
}

QString ModemManager::Bearer::uni() const
{
    Q_D(const Bearer);
    return d->uni;
}
