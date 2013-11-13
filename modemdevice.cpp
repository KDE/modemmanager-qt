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

#include <QDomDocument>

#include "modemdevice.h"
#include "modemdevice_p.h"
#include "manager.h"
#include "mmdebug.h"
#include "dbus/dbus.h"

#include "modem.h"
#include "modemcdma.h"
#include "sim.h"
#include "modem3gpp.h"
#include "modem3gppussd.h"
#include "modemlocation.h"
#include "modemmessaging.h"
#include "modemtime.h"

#include "generic-types.h"

ModemDevicePrivate::ModemDevicePrivate(const QString &path)
    : uni(path)
{
}

ModemDevicePrivate::~ModemDevicePrivate()
{
}

ModemManager::ModemDevice::ModemDevice(const QString &path, QObject *parent)
    : QObject(parent)
    , d_ptr(new ModemDevicePrivate(path))
{
    init();
}

ModemManager::ModemDevice::ModemDevice(ModemDevicePrivate &dd, QObject *parent)
    : QObject(parent)
    , d_ptr(&dd)
{
    init();
}

ModemManager::ModemDevice::~ModemDevice()
{
    delete d_ptr;
}

void ModemManager::ModemDevice::init()
{
    Q_D(ModemDevice);

    QDBusConnection::systemBus().connect(MM_DBUS_SERVICE, MM_DBUS_PATH, DBUS_INTERFACE_MANAGER, "InterfacesAdded",
                                        this, SLOT(onInterfacesAdded(QDBusObjectPath,NMVariantMapMap)));
    QDBusConnection::systemBus().connect(MM_DBUS_SERVICE, MM_DBUS_PATH, DBUS_INTERFACE_MANAGER, "InterfacesRemoved",
                                        this, SLOT(onInterfacesRemoved(QDBusObjectPath,QStringList)));

    initInterfaces();
}

void ModemManager::ModemDevice::initInterfaces()
{
    Q_D(ModemDevice);
    d->interfaceList.clear();

    const QString xmlData = introspect();
    if (xmlData.isEmpty()) {
        mmDebug() << d->uni << "has no interfaces!";
        return;
    }

    QDomDocument dom;
    dom.setContent(xmlData);

    QDomNodeList ifaceNodeList = dom.elementsByTagName("interface");
    for (int i = 0; i < ifaceNodeList.count(); i++) {
        QDomElement ifaceElem = ifaceNodeList.item(i).toElement();
        /* Accept only MM interfaces so that when the device is unplugged,
         * d->interfaces goes empty and we can easily verify that the device is gone. */
        if (!ifaceElem.isNull() && ifaceElem.attribute("name").startsWith(MM_DBUS_SERVICE)) {
            QString name = ifaceElem.attribute("name");
            if (name == QLatin1String(MM_DBUS_INTERFACE_MODEM)) {
                d->interfaceList.insert(ModemManager::ModemDevice::ModemInterface, ModemManager::Modem::Ptr());
                if (hasInterface(ModemDevice::ModemInterface)) {
                    ModemManager::Modem::Ptr modemInterface = interface(ModemDevice::ModemInterface).objectCast<ModemManager::Modem>();
                    if (!modemInterface->simPath().isEmpty()) {
                        d->simList.insert(modemInterface->simPath(), ModemManager::Sim::Ptr());
                        emit simAdded(modemInterface->simPath());

                        connect(modemInterface.data(), SIGNAL(simPathChanged(QString,QString)),
                                SLOT(onSimPathChanged(QString,QString)));
                    }
                }
            } else if (name == QLatin1String(MM_DBUS_INTERFACE_MODEM_MODEM3GPP)) {
                d->interfaceList.insert(ModemManager::ModemDevice::GsmInterface, ModemManager::Modem3gpp::Ptr());
            } else if (name == QLatin1String(MM_DBUS_INTERFACE_MODEM_MODEM3GPP_USSD)) {
                d->interfaceList.insert(ModemManager::ModemDevice::GsmUssdInterface, ModemManager::Modem3gppUssd::Ptr());
            } else if (name == QLatin1String(MM_DBUS_INTERFACE_MODEM_MODEMCDMA)) {
                d->interfaceList.insert(ModemManager::ModemDevice::CdmaInterface, ModemManager::ModemCdma::Ptr());
            } else if (name == QLatin1String(MM_DBUS_INTERFACE_MODEM_MESSAGING)) {
                d->interfaceList.insert(ModemManager::ModemDevice::MessagingInterface, ModemManager::ModemMessaging::Ptr());
            } else if (name == QLatin1String(MM_DBUS_INTERFACE_MODEM_LOCATION)) {
                d->interfaceList.insert(ModemManager::ModemDevice::LocationInterface, ModemManager::ModemLocation::Ptr());
            } else if (name == QLatin1String(MM_DBUS_INTERFACE_MODEM_TIME)) {
                d->interfaceList.insert(ModemManager::ModemDevice::TimeInterface, ModemManager::ModemTime::Ptr());
            } // TODO
            /* else if (name == QLatin1String(MM_DBUS_INTERFACE_MODEM_FIRMWARE)) {
                d->interfaceList.insert(ModemManager::ModemDevice::FirmwareInterface, ModemManager::ModemFirmwareInterface::Ptr());
            }*/
        }
    }

    mmDebug() << d->uni << "has interfaces:" << d->interfaceList.keys();
}

QString ModemManager::ModemDevice::introspect() const
{
    Q_D(const ModemDevice);
    QDBusMessage call = QDBusMessage::createMethodCall(MM_DBUS_SERVICE, d->uni, DBUS_INTERFACE_INTROSPECT, "Introspect");
    QDBusPendingReply<QString> reply = QDBusConnection::systemBus().call(call);

    if (reply.isValid())
        return reply.value();
    else {
        return QString();
    }
}

ModemManager::Interface::List ModemDevicePrivate::interfaces()
{
    ModemManager::Interface::List list;

    QMap<ModemManager::ModemDevice::InterfaceType, ModemManager::Interface::Ptr>::const_iterator i;
    for (i = interfaceList.constBegin(); i != interfaceList.constEnd(); ++i) {
        ModemManager::Interface::Ptr modemInterface = interface(i.key());
        if (modemInterface) {
            list.append(modemInterface);
        } else {
            qWarning() << "warning: null network Interface for" << i.key();
        }
    }

    return list;
}

ModemManager::Interface::Ptr ModemDevicePrivate::interface(ModemManager::ModemDevice::InterfaceType type)
{
    ModemManager::Interface::Ptr modemInterface;

    if (interfaceList.contains(type)) {
        if (interfaceList.value(type)) {
            modemInterface = interfaceList.value(type);
        } else {
            modemInterface = createInterface(type);
            interfaceList[type] = modemInterface;
        }
    }

    return modemInterface;
}

ModemManager::Interface::Ptr ModemDevicePrivate::createInterface(ModemManager::ModemDevice::InterfaceType type)
{
    ModemManager::Interface::Ptr createdInterface;
    switch (type) {
        case ModemManager::ModemDevice::ModemInterface:
            createdInterface = ModemManager::Interface::Ptr(new ModemManager::Modem(uni), &QObject::deleteLater);
            break;
        case ModemManager::ModemDevice::GsmInterface:
            createdInterface = ModemManager::Interface::Ptr(new ModemManager::Modem3gpp(uni), &QObject::deleteLater);
            break;
        case ModemManager::ModemDevice::GsmUssdInterface:
            createdInterface = ModemManager::Interface::Ptr(new ModemManager::Modem3gppUssd(uni), &QObject::deleteLater);
            break;
        case ModemManager::ModemDevice::CdmaInterface:
            createdInterface = ModemManager::Interface::Ptr(new ModemManager::ModemCdma(uni), &QObject::deleteLater);
            break;
        case ModemManager::ModemDevice::MessagingInterface:
            createdInterface = ModemManager::Interface::Ptr(new ModemManager::ModemMessaging(uni), &QObject::deleteLater);
            break;
        case ModemManager::ModemDevice::LocationInterface:
            createdInterface = ModemManager::Interface::Ptr(new ModemManager::ModemLocation(uni), &QObject::deleteLater);
            break;
        case ModemManager::ModemDevice::TimeInterface:
            createdInterface = ModemManager::Interface::Ptr(new ModemManager::ModemTime(uni), &QObject::deleteLater);
            break;
        // TODO - firmware
    }
    return createdInterface;
}

ModemManager::Bearer::Ptr ModemDevicePrivate::findBearer(const QString &uni)
{
    ModemManager::Bearer::Ptr bearer;
    if (bearerList.contains(uni)) {
        if (bearerList.value(uni)) {
            bearer = bearerList.value(uni);
        } else {
            bearer = ModemManager::Bearer::Ptr(new ModemManager::Bearer(uni), &QObject::deleteLater);
            bearerList[uni] = bearer;
        }
    }
    return bearer;
}

ModemManager::Bearer::List ModemDevicePrivate::bearers()
{
    ModemManager::Bearer::List list;

    QMap<QString, ModemManager::Bearer::Ptr>::const_iterator i;
    for (i = bearerList.constBegin(); i != bearerList.constEnd(); ++i) {
        ModemManager::Bearer::Ptr modemBearer = findBearer(i.key());
        if (!modemBearer.isNull()) {
            list.append(modemBearer);
        } else {
            qWarning() << "warning: null bearer Interface for" << i.key();
        }
    }

    return list;
}

ModemManager::Sim::Ptr ModemDevicePrivate::findSim(const QString &uni)
{
    ModemManager::Sim::Ptr sim;
    if (simList.contains(uni)) {
        if (simList.value(uni)) {
            sim = simList.value(uni);
        } else {
            sim = ModemManager::Sim::Ptr(new ModemManager::Sim(uni), &QObject::deleteLater);
            simList[uni] = sim;
        }
    }
    return sim;
}

ModemManager::Sim::List ModemDevicePrivate::sims()
{
    ModemManager::Sim::List list;

    QMap<QString, ModemManager::Sim::Ptr>::const_iterator i;
    for (i = simList.constBegin(); i != simList.constEnd(); ++i) {
        ModemManager::Sim::Ptr modemSim = findSim(i.key());
        if (!modemSim.isNull()) {
            list.append(modemSim);
        } else {
            qWarning() << "warning: null sim Interface for" << i.key();
        }
    }

    return list;
}

QString ModemManager::ModemDevice::uni() const
{
    Q_D(const ModemDevice);
    return d->uni;
}

bool ModemManager::ModemDevice::hasInterface(ModemManager::ModemDevice::InterfaceType type) const
{
    Q_D(const ModemDevice);
    return d->interfaceList.contains(type);
}

ModemManager::Interface::Ptr ModemManager::ModemDevice::interface(ModemManager::ModemDevice::InterfaceType type) const
{
    Q_D(const ModemDevice);

    return const_cast<ModemDevicePrivate*>(d)->interface(type);
}

ModemManager::Interface::List ModemManager::ModemDevice::interfaces() const
{
    Q_D(const ModemDevice);

    return const_cast<ModemDevicePrivate*>(d)->interfaces();
}

ModemManager::Bearer::Ptr ModemManager::ModemDevice::findBearer(const QString &uni)
{
    Q_D(ModemDevice);

    return d->findBearer(uni);
}

ModemManager::Bearer::List ModemManager::ModemDevice::bearers() const
{
    Q_D(const ModemDevice);

    return const_cast<ModemDevicePrivate*>(d)->bearers();
}

ModemManager::Sim::Ptr ModemManager::ModemDevice::findSim(const QString &uni)
{
    Q_D(ModemDevice);

    return d->findSim(uni);
}

ModemManager::Sim::List ModemManager::ModemDevice::sims() const
{
    Q_D(const ModemDevice);

    return const_cast<ModemDevicePrivate*>(d)->sims();
}

bool ModemManager::ModemDevice::isGsmModem() const
{
    return hasInterface(ModemManager::ModemDevice::GsmInterface);
}

bool ModemManager::ModemDevice::isCdmaModem() const
{
    return hasInterface(ModemManager::ModemDevice::CdmaInterface);
}

void ModemManager::ModemDevice::onInterfacesAdded(const QDBusObjectPath &object_path, const NMVariantMapMap &interfaces_and_properties)
{
    Q_D(ModemDevice);
    if (object_path.path() != d->uni) {
        return;
    }

    foreach(const QString & iface, interfaces_and_properties.keys()) {
        /* Don't store generic DBus interfaces */
        if (iface.startsWith(MM_DBUS_SERVICE)) {
            if (iface == QLatin1String(MM_DBUS_INTERFACE_MODEM)) {
                d->interfaceList.insert(ModemManager::ModemDevice::ModemInterface, ModemManager::Modem::Ptr());
            } else if (iface == QLatin1String(MM_DBUS_INTERFACE_MODEM_MODEM3GPP)) {
                d->interfaceList.insert(ModemManager::ModemDevice::GsmInterface, ModemManager::Modem3gpp::Ptr());
            } else if (iface == QLatin1String(MM_DBUS_INTERFACE_MODEM_MODEM3GPP_USSD)) {
                d->interfaceList.insert(ModemManager::ModemDevice::GsmUssdInterface, ModemManager::Modem3gppUssd::Ptr());
            } else if (iface == QLatin1String(MM_DBUS_INTERFACE_MODEM_MODEMCDMA)) {
                d->interfaceList.insert(ModemManager::ModemDevice::CdmaInterface, ModemManager::ModemCdma::Ptr());
            } else if (iface == QLatin1String(MM_DBUS_INTERFACE_MODEM_MESSAGING)) {
                d->interfaceList.insert(ModemManager::ModemDevice::MessagingInterface, ModemManager::ModemMessaging::Ptr());
            } else if (iface == QLatin1String(MM_DBUS_INTERFACE_MODEM_LOCATION)) {
                d->interfaceList.insert(ModemManager::ModemDevice::LocationInterface, ModemManager::ModemLocation::Ptr());
            } else if (iface == QLatin1String(MM_DBUS_INTERFACE_MODEM_TIME)) {
                d->interfaceList.insert(ModemManager::ModemDevice::TimeInterface, ModemManager::ModemTime::Ptr());
            } // TODO
            /* else if (iface == QLatin1String(MM_DBUS_INTERFACE_MODEM_FIRMWARE)) {
                d->interfaceList.insert(ModemManager::ModemDevice::FirmwareInterface, ModemManager::ModemFirmwareInterface::Ptr());
            }*/
        }
    }
}

void ModemManager::ModemDevice::onInterfacesRemoved(const QDBusObjectPath &object_path, const QStringList &interfaces)
{
    Q_D(ModemDevice);
    if (object_path.path() != d->uni) {
        return;
    }

    foreach(const QString & iface, interfaces) {
        if (iface == QLatin1String(MM_DBUS_INTERFACE_MODEM)) {
            d->interfaceList.remove(ModemManager::ModemDevice::ModemInterface);
        } else if (iface == QLatin1String(MM_DBUS_INTERFACE_MODEM_MODEM3GPP)) {
            d->interfaceList.remove(ModemManager::ModemDevice::GsmInterface);
        } else if (iface == QLatin1String(MM_DBUS_INTERFACE_MODEM_MODEM3GPP_USSD)) {
            d->interfaceList.remove(ModemManager::ModemDevice::GsmUssdInterface);
        } else if (iface == QLatin1String(MM_DBUS_INTERFACE_MODEM_MODEMCDMA)) {
            d->interfaceList.remove(ModemManager::ModemDevice::CdmaInterface);
        } else if (iface == QLatin1String(MM_DBUS_INTERFACE_MODEM_MESSAGING)) {
            d->interfaceList.remove(ModemManager::ModemDevice::MessagingInterface);
        } else if (iface == QLatin1String(MM_DBUS_INTERFACE_MODEM_LOCATION)) {
            d->interfaceList.remove(ModemManager::ModemDevice::LocationInterface);
        } else if (iface == QLatin1String(MM_DBUS_INTERFACE_MODEM_TIME)) {
            d->interfaceList.remove(ModemManager::ModemDevice::TimeInterface);
        } // TODO
        /* else if (iface == QLatin1String(MM_DBUS_INTERFACE_MODEM_FIRMWARE)) {
            d->interfaceList.remove(ModemManager::ModemDevice::FirmwareInterface);
        }*/
    }
}

void ModemManager::ModemDevice::onSimPathChanged(const QString &oldPath, const QString &newPath)
{
    Q_D(ModemDevice);

    if (!oldPath.isEmpty() && d->simList.contains(oldPath)) {
        emit simRemoved(oldPath);
        d->simList.remove(oldPath);
    }

    if (!newPath.isEmpty()) {
        d->simList.insert(newPath, ModemManager::Sim::Ptr());
        emit simAdded(newPath);
    }
}
