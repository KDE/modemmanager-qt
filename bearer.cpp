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

#include "bearer.h"
#include "bearer_p.h"
#include "mmdebug.h"
#include "dbus/dbus.h"

BearerPrivate::BearerPrivate(const QString &path)
    : bearerIface(MM_DBUS_SERVICE, path, QDBusConnection::systemBus())
    , uni(path)
{
}

ModemManager::Bearer::Bearer(const QString &path, QObject *parent)
    : QObject(parent)
    , d_ptr(new Bearer(path))
{
    Q_D(Bearer);

    QDBusConnection::systemBus().connect(MM_DBUS_SERVICE, path, DBUS_INTERFACE_PROPS, "PropertiesChanged", this,
                                         SLOT(onPropertiesChanged(QString,QVariantMap,QStringList)));
}

ModemManager::Bearer::~Bearer()
{
}

QString ModemManager::Bearer::interface() const
{
    Q_D(const Bearer);
    return d->bearerIface.interface();
}

bool ModemManager::Bearer::isConnected() const
{
    Q_D(const Bearer);
    return d->bearerIface.connected();
}

bool ModemManager::Bearer::isSuspended() const
{
    Q_D(const Bearer);
    return d->bearerIface.suspended();
}

ModemManager::Bearer::IpConfig ModemManager::Bearer::ip4Config() const
{
    Q_D(const Bearer);
    IpConfig result;
    const QVariantMap map = d->bearerIface.ip4Config();
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

ModemManager::Bearer::IpConfig ModemManager::Bearer::ip6Config() const
{
    Q_D(const Bearer);
    IpConfig result;
    const QVariantMap map = d->bearerIface.ip6Config();
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

uint ModemManager::Bearer::ipTimeout() const
{
    Q_D(const Bearer);
    return d->bearerIface.ipTimeout();
}

QVariantMap ModemManager::Bearer::properties() const
{
    Q_D(const Bearer);
    return d->bearerIface.properties();
}

void ModemManager::Bearer::connectBearer()
{
    Q_D(Bearer);
    d->bearerIface.Connect();
}

void ModemManager::Bearer::disconnectBearer()
{
    Q_D(Bearer);
    d->bearerIface.Disconnect();
}

void ModemManager::Bearer::onPropertiesChanged(const QString &interface, const QVariantMap &properties, const QStringList &invalidatedProps)
{
    mmDebug() << interface << properties.keys();

    if (interface == QString(MM_DBUS_INTERFACE_BEARER)) {
        QLatin1String interface(MM_BEARER_PROPERTY_INTERFACE);
        QLatin1String connected(MM_BEARER_PROPERTY_CONNECTED);
        QLatin1String suspended(MM_BEARER_PROPERTY_SUSPENDED);
        QLatin1String ip4Config(MM_BEARER_PROPERTY_IP4CONFIG);
        QLatin1String ip6Config(MM_BEARER_PROPERTY_IP6CONFIG);

        QVariantMap::const_iterator it = properties.constFind(interface);
        if (it != properties.constEnd()) {
            emit interfaceChanged(it->toString());
        }
        it = properties.constFind(connected);
        if (it != properties.constEnd()) {
            emit connectedChanged(it->toBool());
        }
        it = properties.constFind(suspended);
        if (it != properties.constEnd()) {
            emit suspendedChanged(it->toBool());
        }
        it = properties.constFind(ip4Config);
        if (it != properties.constEnd()) {
            emit ip4ConfigChanged();
        }
        it = properties.constFind(ip6Config);
        if (it != properties.constEnd()) {
            emit ip6ConfigChanged();
        }
    }
}

QString ModemManager::Bearer::uni() const
{
    Q_D(const Bearer);
    return d->uni;
}
