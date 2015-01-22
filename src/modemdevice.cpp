/*
    Copyright 2013, 2014 Lukas Tinkl <ltinkl@redhat.com>
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

#include "modemdevice.h"
#include "modemdevice_p.h"

#include "dbus/dbus.h"
#include "generictypes.h"
#include "manager.h"
#include "mmdebug.h"
#include "modem.h"
#include "modemcdma.h"
#include "modem3gpp.h"
#include "modem3gppussd.h"
#include "modemlocation.h"
#if MM_CHECK_VERSION(1, 2, 0)
#include "modemoma.h"
#include "modemsignal.h"
#endif
#include "modemtime.h"
#include "modemfirmware.h"

#include <QDomDocument>


ModemDevicePrivate::ModemDevicePrivate(const QString &path, ModemManager::ModemDevice *q)
    : uni(path)
    , q_ptr(q)
{
    init();
}

ModemDevicePrivate::~ModemDevicePrivate()
{
}

void ModemDevicePrivate::init()
{
    Q_Q(ModemManager::ModemDevice);

    QDBusConnection::systemBus().connect(MM_DBUS_SERVICE, MM_DBUS_PATH, DBUS_INTERFACE_MANAGER, QStringLiteral("InterfacesAdded"),
                                         q, SLOT(onInterfacesAdded(QDBusObjectPath,NMVariantMapMap)));
    QDBusConnection::systemBus().connect(MM_DBUS_SERVICE, MM_DBUS_PATH, DBUS_INTERFACE_MANAGER, QStringLiteral("InterfacesRemoved"),
                                         q, SLOT(onInterfacesRemoved(QDBusObjectPath,QStringList)));

    initInterfaces();
}

void ModemDevicePrivate::initInterfaces()
{
    Q_Q(ModemManager::ModemDevice);

    interfaceList.clear();

    const QString xmlData = introspect();
    if (xmlData.isEmpty()) {
        qCDebug(MMQT) << uni << "has no interfaces!";
        return;
    }

    QDomDocument dom;
    dom.setContent(xmlData);

    QDomNodeList ifaceNodeList = dom.elementsByTagName("interface");
    for (int i = 0; i < ifaceNodeList.count(); i++) {
        QDomElement ifaceElem = ifaceNodeList.item(i).toElement();
        /* Accept only MM interfaces so that when the device is unplugged,
         * interfaceList goes empty and we can easily verify that the device is gone. */
        if (!ifaceElem.isNull() && ifaceElem.attribute("name").startsWith(MM_DBUS_SERVICE)) {
            const QString name = ifaceElem.attribute("name");
            if (name == QLatin1String(MM_DBUS_INTERFACE_MODEM)) {
                interfaceList.insert(ModemManager::ModemDevice::ModemInterface, ModemManager::Modem::Ptr());
                if (interfaceList.contains(ModemManager::ModemDevice::ModemInterface)) {
                    ModemManager::Modem::Ptr modemInterface = interface(ModemManager::ModemDevice::ModemInterface).objectCast<ModemManager::Modem>();
                    if (!modemInterface->simPath().isEmpty()) {
                        simCard = ModemManager::Sim::Ptr(new ModemManager::Sim(modemInterface->simPath()), &QObject::deleteLater);
                        //q->simAdded(modemInterface->simPath());

                        QObject::connect(modemInterface.data(), SIGNAL(simPathChanged(QString,QString)),
                                         q, SLOT(onSimPathChanged(QString,QString)));
                    }
                }
            } else if (name == QLatin1String(MM_DBUS_INTERFACE_MODEM_MODEM3GPP)) {
                interfaceList.insert(ModemManager::ModemDevice::GsmInterface, ModemManager::Modem3gpp::Ptr());
            } else if (name == QLatin1String(MM_DBUS_INTERFACE_MODEM_MODEM3GPP_USSD)) {
                interfaceList.insert(ModemManager::ModemDevice::GsmUssdInterface, ModemManager::Modem3gppUssd::Ptr());
            } else if (name == QLatin1String(MM_DBUS_INTERFACE_MODEM_MODEMCDMA)) {
                interfaceList.insert(ModemManager::ModemDevice::CdmaInterface, ModemManager::ModemCdma::Ptr());
            } else if (name == QLatin1String(MM_DBUS_INTERFACE_MODEM_MESSAGING)) {
                interfaceList.insert(ModemManager::ModemDevice::MessagingInterface, ModemManager::ModemMessaging::Ptr());
            } else if (name == QLatin1String(MM_DBUS_INTERFACE_MODEM_LOCATION)) {
                interfaceList.insert(ModemManager::ModemDevice::LocationInterface, ModemManager::ModemLocation::Ptr());
            } else if (name == QLatin1String(MM_DBUS_INTERFACE_MODEM_TIME)) {
                interfaceList.insert(ModemManager::ModemDevice::TimeInterface, ModemManager::ModemTime::Ptr());
            }
#if MM_CHECK_VERSION(1, 2, 0)
              else if (name == QLatin1String(MM_DBUS_INTERFACE_MODEM_OMA)) {
                interfaceList.insert(ModemManager::ModemDevice::OmaInterface, ModemManager::ModemOma::Ptr());
            } else if (name == QLatin1String(MM_DBUS_INTERFACE_MODEM_SIGNAL)) {
                interfaceList.insert(ModemManager::ModemDevice::SignalInterface, ModemManager::ModemSignal::Ptr());
            }
#endif
            else if (name == QLatin1String(MM_DBUS_INTERFACE_MODEM_FIRMWARE)) {
                interfaceList.insert(ModemManager::ModemDevice::FirmwareInterface, ModemManager::ModemFirmware::Ptr());
            }
        }
    }

    qCDebug(MMQT) << uni << "has interfaces:" << interfaceList.keys();
}

QString ModemDevicePrivate::introspect() const
{
    QDBusMessage call = QDBusMessage::createMethodCall(MM_DBUS_SERVICE, uni, DBUS_INTERFACE_INTROSPECT, "Introspect");
    QDBusPendingReply<QString> reply = QDBusConnection::systemBus().call(call);

    if (reply.isValid())
        return reply.value();
    else {
        return QString();
    }
}

ModemManager::ModemDevice::ModemDevice(const QString &path, QObject *parent)
    : QObject(parent)
    , d_ptr(new ModemDevicePrivate(path, this))
{
}

ModemManager::ModemDevice::ModemDevice(ModemDevicePrivate &dd, QObject *parent)
    : QObject(parent)
    , d_ptr(&dd)
{
}

ModemManager::ModemDevice::~ModemDevice()
{
    delete d_ptr;
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
#if MM_CHECK_VERSION(1, 2, 0)
        case ModemManager::ModemDevice::OmaInterface:
            createdInterface = ModemManager::Interface::Ptr(new ModemManager::ModemOma(uni), &QObject::deleteLater);
            break;
        case ModemManager::ModemDevice::SignalInterface:
            createdInterface = ModemManager::Interface::Ptr(new ModemManager::ModemSignal(uni), &QObject::deleteLater);
            break;
#endif
        case ModemManager::ModemDevice::FirmwareInterface:
            createdInterface = ModemManager::Interface::Ptr(new ModemManager::ModemFirmware(uni), &QObject::deleteLater);
            break;
    }
    return createdInterface;
}

ModemManager::Bearer::Ptr ModemDevicePrivate::findBearer(const QString &uni)
{
    ModemManager::Bearer::Ptr result;
    foreach (const ModemManager::Bearer::Ptr & bearer, bearers()) {
        if (bearer->uni() == uni) {
            result = bearer;
            break;
        }
    }

    return result;
}

ModemManager::Bearer::List ModemDevicePrivate::bearers()
{
    Q_Q(ModemManager::ModemDevice);
    ModemManager::Bearer::List result;
    foreach (const QString & path, q->modemInterface()->listBearers()) {
        ModemManager::Bearer::Ptr modemBearer = ModemManager::Bearer::Ptr(new ModemManager::Bearer(path), &QObject::deleteLater);
        if (modemBearer) {
            result.append(modemBearer);
        } else {
            qCDebug(MMQT) << "warning: null bearer interface for" << path;
        }
    }

    return result;
}

ModemManager::Sim::Ptr ModemDevicePrivate::sim()
{
    return simCard;
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

ModemManager::ModemMessaging::Ptr ModemManager::ModemDevice::messagingInterface()
{
    Q_D(ModemDevice);

    return d->interface(ModemManager::ModemDevice::MessagingInterface).objectCast<ModemManager::ModemMessaging>();
}

ModemManager::Modem::Ptr ModemManager::ModemDevice::modemInterface()
{
    Q_D(ModemDevice);

    return d->interface(ModemManager::ModemDevice::ModemInterface).objectCast<ModemManager::Modem>();
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

ModemManager::Sim::Ptr ModemManager::ModemDevice::sim() const
{
    Q_D(const ModemDevice);

    return const_cast<ModemDevicePrivate*>(d)->sim();
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
            }
#if MM_CHECK_VERSION(1, 2, 0)
              else if (iface == QLatin1String(MM_DBUS_INTERFACE_MODEM_OMA)) {
                d->interfaceList.insert(ModemManager::ModemDevice::OmaInterface, ModemManager::ModemOma::Ptr());
            } else if (iface == QLatin1String(MM_DBUS_INTERFACE_MODEM_SIGNAL)) {
                d->interfaceList.insert(ModemManager::ModemDevice::SignalInterface, ModemManager::ModemSignal::Ptr());
            }
#endif
            else if (iface == QLatin1String(MM_DBUS_INTERFACE_MODEM_FIRMWARE)) {
                d->interfaceList.insert(ModemManager::ModemDevice::FirmwareInterface, ModemManager::ModemFirmware::Ptr());
            }
        }
    }
}

void ModemManager::ModemDevice::onInterfacesRemoved(const QDBusObjectPath &object_path, const QStringList &interfaces)
{
    Q_D(ModemDevice);
    if (object_path.path() != d->uni) {
        return;
    }

    if (interfaces.contains(MM_DBUS_INTERFACE_MODEM) || interfaces.isEmpty()) {
        if (d->simCard) {
            emit simRemoved(d->simCard->uni());
            d->simCard = ModemManager::Sim::Ptr();
        }
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
        }
#if MM_CHECK_VERSION(1, 2, 0)
          else if (iface == QLatin1String(MM_DBUS_INTERFACE_MODEM_OMA)) {
            d->interfaceList.remove(ModemManager::ModemDevice::OmaInterface);
        } else if (iface == QLatin1String(MM_DBUS_INTERFACE_MODEM_SIGNAL)) {
            d->interfaceList.remove(ModemManager::ModemDevice::SignalInterface);
        }
#endif
          else if (iface == QLatin1String(MM_DBUS_INTERFACE_MODEM_FIRMWARE)) {
            d->interfaceList.remove(ModemManager::ModemDevice::FirmwareInterface);
        }
    }
}

void ModemManager::ModemDevice::onSimPathChanged(const QString &oldPath, const QString &newPath)
{
    Q_D(ModemDevice);

    if (!oldPath.isEmpty() && d->simCard->uni() == oldPath) {
        emit simRemoved(oldPath);
        d->simCard = ModemManager::Sim::Ptr();
    }

    if (!newPath.isEmpty()) {
        d->simCard = ModemManager::Sim::Ptr(new ModemManager::Sim(newPath), &QObject::deleteLater);
        emit simAdded(newPath);
    }
}
