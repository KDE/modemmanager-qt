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

#include "modem.h"
#include "modem_p.h"
#include "manager.h"
#include "mmdebug.h"
#include "dbus/dbus.h"

#include "modeminterface.h"
#include "modemcdmainterface.h"
#include "modemgsmcardinterface.h"
#include "modemgsmnetworkinterface.h"
#include "modemgsmussdinterface.h"
#include "modemlocationinterface.h"
#include "modemmessaginginterface.h"
#include "modemtimeinterface.h"

#include "generic-types.h"

ModemPrivate::ModemPrivate(const QString & path) :
    uni(path)
{
}

ModemPrivate::~ModemPrivate()
{
}

ModemManager::Modem::Modem(const QString & path, QObject * parent) :
    QObject(parent),
    d_ptr(new ModemPrivate(path))
{
    init();
}

ModemManager::Modem::Modem(ModemPrivate & dd, QObject * parent) :
    QObject(parent),
    d_ptr(&dd)
{
    init();
}

ModemManager::Modem::~Modem()
{
    delete d_ptr;
}

void ModemManager::Modem::init()
{
    Q_D(Modem);

    QDBusConnection::systemBus().connect(MM_DBUS_SERVICE, MM_DBUS_PATH, DBUS_INTERFACE_MANAGER, "InterfacesAdded",
                                        this, SLOT(onInterfacesAdded(QDBusObjectPath,NMVariantMapMap)));
    QDBusConnection::systemBus().connect(MM_DBUS_SERVICE, MM_DBUS_PATH, DBUS_INTERFACE_MANAGER, "InterfacesRemoved",
                                        this, SLOT(onInterfacesRemoved(QDBusObjectPath,QStringList)));

    initInterfaces();
}

void ModemManager::Modem::initInterfaces()
{
    Q_D(Modem);
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
                d->interfaceList.insert(ModemManager::Modem::ModemInterface, ModemManager::ModemInterface::Ptr());
            } else if (name == QLatin1String(MM_DBUS_INTERFACE_MODEM_MODEM3GPP)) {
                d->interfaceList.insert(ModemManager::Modem::GsmInterface, ModemManager::Modem3gppInterface::Ptr());
            } else if (name == QLatin1String(MM_DBUS_INTERFACE_MODEM_MODEM3GPP_USSD)) {
                d->interfaceList.insert(ModemManager::Modem::GsmUssdInterface, ModemManager::Modem3gppUssdInterface::Ptr());
            } else if (name == QLatin1String(MM_DBUS_INTERFACE_MODEM_MODEMCDMA)) {
                d->interfaceList.insert(ModemManager::Modem::CdmaInterface, ModemManager::ModemCdmaInterface::Ptr());
            } else if (name == QLatin1String(MM_DBUS_INTERFACE_MODEM_MESSAGING)) {
                d->interfaceList.insert(ModemManager::Modem::MessagingInterface, ModemManager::ModemMessagingInterface::Ptr());
            } else if (name == QLatin1String(MM_DBUS_INTERFACE_MODEM_LOCATION)) {
                d->interfaceList.insert(ModemManager::Modem::LocationInterface, ModemManager::ModemLocationInterface::Ptr());
            } else if (name == QLatin1String(MM_DBUS_INTERFACE_MODEM_TIME)) {
                d->interfaceList.insert(ModemManager::Modem::TimeInterface, ModemManager::ModemTimeInterface::Ptr());
            } // TODO
            /* else if (name == QLatin1String(MM_DBUS_INTERFACE_MODEM_FIRMWARE)) {
                d->interfaceList.insert(ModemManager::Modem::FirmwareInterface, ModemManager::ModemFirmwareInterface::Ptr());
            }*/
        }
    }

    mmDebug() << d->uni << "has interfaces:" << d->interfaceList.keys();
}

QString ModemManager::Modem::introspect() const
{
    Q_D(const Modem);
    QDBusMessage call = QDBusMessage::createMethodCall(MM_DBUS_SERVICE, d->uni, DBUS_INTERFACE_INTROSPECT, "Introspect");
    QDBusPendingReply<QString> reply = QDBusConnection::systemBus().call(call);

    if (reply.isValid())
        return reply.value();
    else {
        return QString();
    }
}

ModemManager::Interface::List ModemPrivate::interfaces()
{
    ModemManager::Interface::List list;

    QMap<ModemManager::Modem::InterfaceType, ModemManager::Interface::Ptr>::const_iterator i;
    for (i = interfaceList.constBegin(); i != interfaceList.constEnd(); ++i) {
        ModemManager::Interface::Ptr modemInterface = interface(i.key());
        if (!modemInterface.isNull()) {
            list.append(modemInterface);
        } else {
            qWarning() << "warning: null network Interface for" << i.key();
        }
    }

    return list;
}

ModemManager::Interface::Ptr ModemPrivate::interface(ModemManager::Modem::InterfaceType type)
{
    ModemManager::Interface::Ptr modemInterface;

    if (interfaceList.contains(type)) {
        if (interfaceList.contains(type)) {
            if (interfaceList.value(type)) {
                modemInterface = interfaceList.value(type);
            } else {
                modemInterface = createInterface(type);
                interfaceList[type] = modemInterface;
            }
        }
    }

    return modemInterface;
}

ModemManager::Interface::Ptr ModemPrivate::createInterface(ModemManager::Modem::InterfaceType type)
{
    ModemManager::Interface::Ptr createdInterface;
    switch (type) {
        case ModemManager::Modem::ModemInterface:
            createdInterface = ModemManager::Interface::Ptr(new ModemManager::ModemInterface(uni), &QObject::deleteLater);
            break;
        case ModemManager::Modem::GsmInterface:
            createdInterface = ModemManager::Interface::Ptr(new ModemManager::Modem3gppInterface(uni), &QObject::deleteLater);
            break;
        case ModemManager::Modem::GsmUssdInterface:
            createdInterface = ModemManager::Interface::Ptr(new ModemManager::Modem3gppUssdInterface(uni), &QObject::deleteLater);
            break;
        case ModemManager::Modem::CdmaInterface:
            createdInterface = ModemManager::Interface::Ptr(new ModemManager::ModemCdmaInterface(uni), &QObject::deleteLater);
            break;
        case ModemManager::Modem::MessagingInterface:
            createdInterface = ModemManager::Interface::Ptr(new ModemManager::ModemMessagingInterface(uni), &QObject::deleteLater);
            break;
        case ModemManager::Modem::LocationInterface:
            createdInterface = ModemManager::Interface::Ptr(new ModemManager::ModemLocationInterface(uni), &QObject::deleteLater);
            break;
        case ModemManager::Modem::TimeInterface:
            createdInterface = ModemManager::Interface::Ptr(new ModemManager::ModemTimeInterface(uni), &QObject::deleteLater);
            break;
        // TODO - firmware
    }
    return createdInterface;
}

QString ModemManager::Modem::uni() const
{
    Q_D(const Modem);
    return d->uni;
}

bool ModemManager::Modem::hasInterface(ModemManager::Modem::InterfaceType type) const
{
    Q_D(const Modem);
    return d->interfaceList.contains(type);
}

ModemManager::Interface::Ptr ModemManager::Modem::interface(ModemManager::Modem::InterfaceType type) const
{
    Q_D(const Modem);

    return const_cast<ModemPrivate*>(d)->interface(type);
}

ModemManager::Interface::List ModemManager::Modem::interfaces() const
{
    Q_D(const Modem);

    return const_cast<ModemPrivate*>(d)->interfaces();
}

bool ModemManager::Modem::isGsmModem() const
{
    return hasInterface(ModemManager::Modem::GsmInterface);
}

bool ModemManager::Modem::isCdmaModem() const
{
    return hasInterface(ModemManager::Modem::CdmaInterface);
}

void ModemManager::Modem::onInterfacesAdded(const QDBusObjectPath &object_path, const NMVariantMapMap &interfaces_and_properties)
{
    Q_D(Modem);
    if (object_path.path() != d->uni) {
        return;
    }

    foreach(const QString & iface, interfaces_and_properties.keys()) {
        /* Don't store generic DBus interfaces */
        if (iface.startsWith(MM_DBUS_SERVICE)) {
            if (iface == QLatin1String(MM_DBUS_INTERFACE_MODEM)) {
                d->interfaceList.insert(ModemManager::Modem::ModemInterface, ModemManager::ModemInterface::Ptr());
            } else if (iface == QLatin1String(MM_DBUS_INTERFACE_MODEM_MODEM3GPP)) {
                d->interfaceList.insert(ModemManager::Modem::GsmInterface, ModemManager::Modem3gppInterface::Ptr());
            } else if (iface == QLatin1String(MM_DBUS_INTERFACE_MODEM_MODEM3GPP_USSD)) {
                d->interfaceList.insert(ModemManager::Modem::GsmUssdInterface, ModemManager::Modem3gppUssdInterface::Ptr());
            } else if (iface == QLatin1String(MM_DBUS_INTERFACE_MODEM_MODEMCDMA)) {
                d->interfaceList.insert(ModemManager::Modem::CdmaInterface, ModemManager::ModemCdmaInterface::Ptr());
            } else if (iface == QLatin1String(MM_DBUS_INTERFACE_MODEM_MESSAGING)) {
                d->interfaceList.insert(ModemManager::Modem::MessagingInterface, ModemManager::ModemMessagingInterface::Ptr());
            } else if (iface == QLatin1String(MM_DBUS_INTERFACE_MODEM_LOCATION)) {
                d->interfaceList.insert(ModemManager::Modem::LocationInterface, ModemManager::ModemLocationInterface::Ptr());
            } else if (iface == QLatin1String(MM_DBUS_INTERFACE_MODEM_TIME)) {
                d->interfaceList.insert(ModemManager::Modem::TimeInterface, ModemManager::ModemTimeInterface::Ptr());
            } // TODO
            /* else if (iface == QLatin1String(MM_DBUS_INTERFACE_MODEM_FIRMWARE)) {
                d->interfaceList.insert(ModemManager::Modem::FirmwareInterface, ModemManager::ModemFirmwareInterface::Ptr());
            }*/
        }
    }
}

void ModemManager::Modem::onInterfacesRemoved(const QDBusObjectPath &object_path, const QStringList &interfaces)
{
    Q_D(Modem);
    if (object_path.path() != d->uni) {
        return;
    }

    foreach(const QString & iface, interfaces) {
        if (iface == QLatin1String(MM_DBUS_INTERFACE_MODEM)) {
            d->interfaceList.remove(ModemManager::Modem::ModemInterface);
        } else if (iface == QLatin1String(MM_DBUS_INTERFACE_MODEM_MODEM3GPP)) {
            d->interfaceList.remove(ModemManager::Modem::GsmInterface);
        } else if (iface == QLatin1String(MM_DBUS_INTERFACE_MODEM_MODEM3GPP_USSD)) {
            d->interfaceList.remove(ModemManager::Modem::GsmUssdInterface);
        } else if (iface == QLatin1String(MM_DBUS_INTERFACE_MODEM_MODEMCDMA)) {
            d->interfaceList.remove(ModemManager::Modem::CdmaInterface);
        } else if (iface == QLatin1String(MM_DBUS_INTERFACE_MODEM_MESSAGING)) {
            d->interfaceList.remove(ModemManager::Modem::MessagingInterface);
        } else if (iface == QLatin1String(MM_DBUS_INTERFACE_MODEM_LOCATION)) {
            d->interfaceList.remove(ModemManager::Modem::LocationInterface);
        } else if (iface == QLatin1String(MM_DBUS_INTERFACE_MODEM_TIME)) {
            d->interfaceList.remove(ModemManager::Modem::TimeInterface);
        } // TODO
        /* else if (iface == QLatin1String(MM_DBUS_INTERFACE_MODEM_FIRMWARE)) {
            d->interfaceList.remove(ModemManager::Modem::FirmwareInterface);
        }*/
    }
}
