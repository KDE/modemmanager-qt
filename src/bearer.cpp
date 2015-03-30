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
#include "mmdebug_p.h"
#ifdef MMQT_STATIC
#include "dbus/fakedbus.h"
#else
#include "dbus/dbus.h"
#endif

Q_LOGGING_CATEGORY(MMQT, "modemmanager-qt")

namespace ModemManager {

class ModemManager::IpConfig::Private
{
public:
    Private()
    { }
    MMBearerIpMethod method;
    QString address;
    uint prefix;
    QString dns1;
    QString dns2;
    QString dns3;
    QString gateway;
};

}

ModemManager::IpConfig::IpConfig()
    : d(new Private())
{
}

ModemManager::IpConfig::IpConfig(const ModemManager::IpConfig& other)
    : d(new Private)
{
    *this = other;
}

ModemManager::IpConfig::~IpConfig()
{
    delete d;
}

MMBearerIpMethod ModemManager::IpConfig::method() const
{
    return d->method;
}

void ModemManager::IpConfig::setMethod(MMBearerIpMethod method)
{
    d->method = method;
}

QString ModemManager::IpConfig::address() const
{
    return d->address;
}

void ModemManager::IpConfig::setAddress(const QString& address)
{
    d->address = address;
}

uint ModemManager::IpConfig::prefix() const
{
    return d->prefix;
}

void ModemManager::IpConfig::setPrefix(uint prefix)
{
    d->prefix = prefix;
}

QString ModemManager::IpConfig::dns1() const
{
    return d->dns1;
}

void ModemManager::IpConfig::setDns1(const QString& dns1)
{
    d->dns1 = dns1;
}

QString ModemManager::IpConfig::dns2() const
{
    return d->dns2;
}

void ModemManager::IpConfig::setDns2(const QString& dns2)
{
    d->dns2 = dns2;
}

QString ModemManager::IpConfig::dns3() const
{
    return d->dns3;
}

void ModemManager::IpConfig::setDns3(const QString& dns3)
{
    d->dns3 = dns3;
}

QString ModemManager::IpConfig::gateway() const
{
    return d->gateway;
}

void ModemManager::IpConfig::setGateway(const QString& gateway)
{
    d->gateway = gateway;
}

ModemManager::IpConfig& ModemManager::IpConfig::operator=(const ModemManager::IpConfig& other)
{
    if (this == &other) {
        return *this;
    }

    *d = *other.d;
    return *this;
}

ModemManager::BearerPrivate::BearerPrivate(const QString &path, Bearer *q)
#ifdef MMQT_STATIC
    : bearerIface(MMQT_DBUS_SERVICE, path, QDBusConnection::sessionBus())
#else
    : bearerIface(MMQT_DBUS_SERVICE, path, QDBusConnection::systemBus())
#endif
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

#ifdef MMQT_STATIC
    QDBusConnection::sessionBus().connect(MMQT_DBUS_SERVICE, path, DBUS_INTERFACE_PROPS, QStringLiteral("PropertiesChanged"), d,
                                         SLOT(onPropertiesChanged(QString,QVariantMap,QStringList)));
#else
    QDBusConnection::systemBus().connect(MMQT_DBUS_SERVICE, path, DBUS_INTERFACE_PROPS, QStringLiteral("PropertiesChanged"), d,
                                         SLOT(onPropertiesChanged(QString,QVariantMap,QStringList)));
#endif
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

ModemManager::IpConfig ModemManager::Bearer::ip4Config() const
{
    Q_D(const Bearer);
    return d->ipv4Config;
}

ModemManager::IpConfig ModemManager::Bearer::ip6Config() const
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

ModemManager::IpConfig ModemManager::BearerPrivate::ipConfigFromMap(const QVariantMap &map)
{
    ModemManager::IpConfig result;
    result.setMethod((MMBearerIpMethod)map.value("method").toUInt());

    if (result.method() == MM_BEARER_IP_METHOD_STATIC) {
        result.setAddress(map.value("address").toString());
        result.setPrefix(map.value("prefix").toUInt());
        result.setDns1(map.value("dns1").toString());
        result.setDns2(map.value("dns2").toString());
        result.setDns3(map.value("dns3").toString());
        result.setGateway(map.value("gateway").toString());
    }

    return result;
}

void ModemManager::BearerPrivate::onPropertiesChanged(const QString &interface, const QVariantMap &properties, const QStringList &invalidatedProps)
{
    Q_Q(Bearer);
    Q_UNUSED(invalidatedProps);
    qCDebug(MMQT) << interface << properties.keys();

    if (interface == QString(MMQT_DBUS_INTERFACE_BEARER)) {
        QVariantMap::const_iterator it = properties.constFind(QLatin1String(MM_BEARER_PROPERTY_INTERFACE));
        if (it != properties.constEnd()) {
            bearerInterface = it->toString();
            Q_EMIT q->interfaceChanged(bearerInterface);
        }
        it = properties.constFind(QLatin1String(MM_BEARER_PROPERTY_CONNECTED));
        if (it != properties.constEnd()) {
            isConnected = it->toBool();
            Q_EMIT q->connectedChanged(isConnected);
        }
        it = properties.constFind(QLatin1String(MM_BEARER_PROPERTY_SUSPENDED));
        if (it != properties.constEnd()) {
            isSuspended = it->toBool();
            Q_EMIT q->suspendedChanged(isSuspended);
        }
        it = properties.constFind(QLatin1String(MM_BEARER_PROPERTY_IP4CONFIG));
        if (it != properties.constEnd()) {
            ipv4Config = ipConfigFromMap(qdbus_cast<QVariantMap>(*it));
            Q_EMIT q->ip4ConfigChanged(ipv4Config);
        }
        it = properties.constFind(QLatin1String(MM_BEARER_PROPERTY_IP6CONFIG));
        if (it != properties.constEnd()) {
            ipv6Config = ipConfigFromMap(qdbus_cast<QVariantMap>(*it));
            Q_EMIT q->ip6ConfigChanged(ipv6Config);
        }
        it = properties.constFind(QLatin1String(MM_BEARER_PROPERTY_IPTIMEOUT));
        if (it != properties.constEnd()) {
            ipTimeout = it->toUInt();
            Q_EMIT q->ipTimeoutChanged(ipTimeout);
        }
        it = properties.constFind(QLatin1String(MM_BEARER_PROPERTY_PROPERTIES));
        if (it != properties.constEnd()) {
            bearerProperties = qdbus_cast<QVariantMap>(*it);
            Q_EMIT q->propertiesChanged(bearerProperties);
        }
    }
}

QString ModemManager::Bearer::uni() const
{
    Q_D(const Bearer);
    return d->uni;
}
