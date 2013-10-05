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

#include "modembearerinterface.h"
#include "modembearerinterface_p.h"
#include "mmdebug.h"
#include "dbus/dbus.h"

ModemBearerInterfacePrivate::ModemBearerInterfacePrivate(const QString &path)
    : modemBearerIface(MM_DBUS_SERVICE, path, QDBusConnection::systemBus())
{
}

ModemManager::ModemBearerInterface::ModemBearerInterface(const QString & path, QObject * parent)
    : QObject(parent), d_ptr(new ModemBearerInterface(path))
{
    Q_D(ModemBearerInterface);

    QDBusConnection::systemBus().connect(MM_DBUS_SERVICE, path, DBUS_INTERFACE_PROPS, "PropertiesChanged", this,
                                         SLOT(onPropertiesChanged(QString,QVariantMap,QStringList)));
}

ModemManager::ModemBearerInterface::~ModemBearerInterface()
{
}

QString ModemManager::ModemBearerInterface::interface() const
{
    Q_D(const ModemBearerInterface);
    return d->modemBearerIface.interface();
}

bool ModemManager::ModemBearerInterface::isConnected() const
{
    Q_D(const ModemBearerInterface);
    return d->modemBearerIface.connected();
}

bool ModemManager::ModemBearerInterface::isSuspended() const
{
    Q_D(const ModemBearerInterface);
    return d->modemBearerIface.suspended();
}

ModemManager::ModemBearerInterface::IpConfig ModemManager::ModemBearerInterface::ip4Config() const
{
    Q_D(const ModemBearerInterface);
    IpConfig result;
    const QVariantMap map = d->modemBearerIface.ip4Config();
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

ModemManager::ModemBearerInterface::IpConfig ModemManager::ModemBearerInterface::ip6Config() const
{
    Q_D(const ModemBearerInterface);
    IpConfig result;
    const QVariantMap map = d->modemBearerIface.ip6Config();
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

uint ModemManager::ModemBearerInterface::ipTimeout() const
{
    Q_D(const ModemBearerInterface);
    return d->modemBearerIface.ipTimeout();
}

QVariantMap ModemManager::ModemBearerInterface::properties() const
{
    Q_D(const ModemBearerInterface);
    return d->modemBearerIface.properties();
}

void ModemManager::ModemBearerInterface::connectBearer()
{
    Q_D(ModemBearerInterface);
    d->modemBearerIface.Connect();
}

void ModemManager::ModemBearerInterface::disconnectBearer()
{
    Q_D(ModemBearerInterface);
    d->modemBearerIface.Disconnect();
}

void ModemManager::ModemBearerInterface::onPropertiesChanged(const QString & interface, const QVariantMap & properties, const QStringList &invalidatedProps)
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
