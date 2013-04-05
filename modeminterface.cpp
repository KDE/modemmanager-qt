/*
Copyright 2008,2011 Will Stephenson <wstephenson@kde.org>
Copyright 2010 Lamarque Souza <lamarque@kde.org>
Copyright 2013 Daniel Nicoletti <dantti12@gmail.com>

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

#include "modeminterface.h"
#include "modeminterface_p.h"
#include "manager.h"
#include "mmdebug.h"

ModemInterfacePrivate::ModemInterfacePrivate( const QString & path, QObject * owner ) : modemIface(ModemManager::DBUS_SERVICE, path, QDBusConnection::systemBus()), modemSimpleIface(ModemManager::DBUS_SERVICE, path, QDBusConnection::systemBus()), udi(path)
{
    Q_UNUSED(owner);
}

ModemInterfacePrivate::~ModemInterfacePrivate()
{

}

ModemManager::ModemInterface::ModemInterface(const QString & path, QObject * parent) : QObject(parent), d_ptr(new ModemInterfacePrivate(path, this))
{
    init();
}

ModemManager::ModemInterface::ModemInterface(ModemInterfacePrivate & dd, QObject * parent) : QObject(parent), d_ptr(&dd)
{
    init();
}

ModemManager::ModemInterface::~ModemInterface()
{
    delete d_ptr;
}

void ModemManager::ModemInterface::init()
{
    Q_D(ModemInterface);
    d->device = d->modemIface.device();
    d->masterDevice = d->modemIface.masterDevice();
    d->driver = d->modemIface.driver();
    d->type = (ModemManager::ModemInterface::Type) d->modemIface.type();
    d->enabled = d->modemIface.enabled();
    d->unlockRequired = d->modemIface.unlockRequired();
    d->ipMethod = (ModemManager::ModemInterface::Method) d->modemIface.ipMethod();

    d->modemIface.connection().connect(ModemManager::DBUS_SERVICE,
                                       d->udi, QLatin1String("org.freedesktop.DBus.Properties"),
                                       QLatin1String("MmPropertiesChanged"), QLatin1String("sa{sv}"),
                                       this, SLOT(propertiesChanged(QString,QVariantMap)));
}

QString ModemManager::ModemInterface::udi() const
{
    Q_D(const ModemInterface);
    return d->udi;
}


/*** From org.freedesktop.ModemManager.Modem ***/

void ModemManager::ModemInterface::enable(const bool enable)
{
    Q_D(ModemInterface);
    d->modemIface.Enable(enable);
}

void ModemManager::ModemInterface::connectModem(const QString & number)
{
    Q_D(ModemInterface);
    d->modemIface.Connect(number);
}

void ModemManager::ModemInterface::disconnectModem()
{
    Q_D(ModemInterface);
    d->modemIface.Disconnect();
}

ModemManager::ModemInterface::Ip4ConfigType ModemManager::ModemInterface::getIp4Config()
{
    Q_D(ModemInterface);
    QDBusReply<Ip4ConfigType> config = d->modemIface.GetIP4Config();

    if (config.isValid()) {
        return config.value();
    }

    return Ip4ConfigType();
}

ModemManager::ModemInterface::InfoType ModemManager::ModemInterface::getInfo()
{
    Q_D(ModemInterface);
    QDBusReply<InfoType> info = d->modemIface.GetInfo();

    if (info.isValid()) {
        return info.value();
    }

    return InfoType();
}

QString ModemManager::ModemInterface::device() const
{
    Q_D(const ModemInterface);
    return d->device;
}

QString ModemManager::ModemInterface::masterDevice() const
{
    Q_D(const ModemInterface);
    return d->masterDevice;
}

QString ModemManager::ModemInterface::driver() const
{
    Q_D(const ModemInterface);
    return d->driver;
}

ModemManager::ModemInterface::Type ModemManager::ModemInterface::type() const
{
    Q_D(const ModemInterface);
    return d->type;
}

bool ModemManager::ModemInterface::enabled() const
{
    Q_D(const ModemInterface);
    return d->enabled;
}

QString ModemManager::ModemInterface::unlockRequired() const
{
    Q_D(const ModemInterface);
    return d->unlockRequired;
}

ModemManager::ModemInterface::Method ModemManager::ModemInterface::ipMethod() const
{
    Q_D(const ModemInterface);
    return d->ipMethod;
}

void ModemManager::ModemInterface::propertiesChanged(const QString & interface, const QVariantMap & properties)
{
    Q_D(ModemInterface);
    mmDebug() << interface << properties.keys();

    if (interface == QString("org.freedesktop.ModemManager.Modem")) {
        QLatin1String device("Device");
        QLatin1String masterDevice("MasterDevice");
        QLatin1String driver("Driver");
        QLatin1String type("Type");
        QLatin1String enabled("Enabled");
        QLatin1String unlockRequired("UnlockRequired");
        QLatin1String ipMethod("IpMethod");

        QVariantMap::const_iterator it = properties.find(device);
        if ( it != properties.end()) {
            d->device = it->toString();
            emit deviceChanged(d->device);
        }
        it = properties.find(masterDevice);
        if ( it != properties.end()) {
            d->masterDevice = it->toString();
            emit masterDeviceChanged(d->masterDevice);
        }
        it = properties.find(driver);
        if ( it != properties.end()) {
            d->driver = it->toString();
            emit driverChanged(d->driver);
        }
        it = properties.find(type);
        if ( it != properties.end()) {
            d->type = (ModemManager::ModemInterface::Type) it->toInt();
            emit typeChanged(d->type);
        }
        it = properties.find(enabled);
        if ( it != properties.end()) {
            d->enabled = it->toBool();
            emit enabledChanged(d->enabled);
        }
        it = properties.find(unlockRequired);
        if ( it != properties.end()) {
            d->unlockRequired = it->toString();
            emit unlockRequiredChanged(d->unlockRequired);
        }
        it = properties.find(ipMethod);
        if ( it != properties.end()) {
            d->ipMethod = (ModemManager::ModemInterface::Method) it->toInt();
            emit ipMethodChanged(d->ipMethod);
        }
    }
}


/*** From org.freedesktop.ModemManager.Modem.Simple ***/

void ModemManager::ModemInterface::connectModem(const QVariantMap & properties)
{
    Q_D(ModemInterface);
    d->modemSimpleIface.Connect(properties);
}

QVariantMap ModemManager::ModemInterface::getStatus()
{
    Q_D(ModemInterface);

    QDBusReply<QVariantMap> status = d->modemSimpleIface.GetStatus();

    if (status.isValid()) {
        return status.value();
    }

    return QVariantMap();
}

