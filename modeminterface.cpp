/*
Copyright 2008,2011 Will Stephenson <wstephenson@kde.org>
Copyright 2010 Lamarque Souza <lamarque@kde.org>
Copyright 2013 Daniel Nicoletti <dantti12@gmail.com>
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

#include <QDomDocument>

#include "modeminterface.h"
#include "modeminterface_p.h"
#include "mmdebug.h"
#include "dbus/dbus.h"

#include "generic-types.h"

ModemInterfacePrivate::ModemInterfacePrivate(const QString &path)
    : InterfacePrivate(path)
    , modemIface(MM_DBUS_SERVICE, path, QDBusConnection::systemBus())
    , modemSimpleIface(MM_DBUS_SERVICE, path, QDBusConnection::systemBus())
{
}

ModemManager::ModemInterface::ModemInterface(const QString &path, QObject *parent)
    : Interface(*new ModemInterfacePrivate(path), parent)
{
    init();
}

ModemManager::ModemInterface::~ModemInterface()
{
}

void ModemManager::ModemInterface::init()
{
    Q_D(ModemInterface);
    d->device = d->modemIface.device();
    d->drivers = d->modemIface.drivers();

    if (d->modemIface.isValid()) {
        QDBusConnection::systemBus().connect(MM_DBUS_SERVICE, d->uni, DBUS_INTERFACE_PROPS, "PropertiesChanged", this,
                                             SLOT(onPropertiesChanged(QString,QVariantMap,QStringList)));
    }

    connect(&d->modemIface, SIGNAL(StateChanged(int,int,uint)), SLOT(onStateChanged(int,int,uint)));
}

QString ModemManager::ModemInterface::uni() const
{
    Q_D(const ModemInterface);
    return d->uni;
}

bool ModemManager::ModemInterface::isEnabled() const
{
    Q_D(const ModemInterface);
    return (MMModemPowerState)d->modemIface.powerState() == MM_MODEM_POWER_STATE_ON;
}

bool ModemManager::ModemInterface::isValid() const
{
    Q_D(const ModemInterface);
    return d->modemIface.isValid();
}

// From org.freedesktop.ModemManager.Modem
void ModemManager::ModemInterface::enable(bool enable)
{
    Q_D(ModemInterface);
    d->modemIface.Enable(enable);
}

QStringList ModemManager::ModemInterface::listBearers()
{
    Q_D(ModemInterface);
    QStringList result;
    QList<QDBusObjectPath> objects = d->modemIface.ListBearers();
    foreach (const QDBusObjectPath & path, objects) {
        result << path.path();
    }
    return result;
}

QString ModemManager::ModemInterface::createBearer(const BearerStruct &bearer)
{
    Q_D(ModemInterface);
    QVariantMap map;
    map.insert("apn", bearer.apn);
    if (bearer.ipType != MM_BEARER_IP_FAMILY_NONE)
        map.insert("ip-type", (uint)bearer.ipType);
    if (bearer.allowedAuth != MM_BEARER_ALLOWED_AUTH_UNKNOWN)
        map.insert("allowed-auth", (uint)bearer.allowedAuth);
    if (!bearer.user.isEmpty())
        map.insert("user", bearer.user);
    if (!bearer.password.isEmpty())
        map.insert("password", bearer.password);
    map.insert("allow-roaming", bearer.allowRoaming);
    if (bearer.rmProtocol != MM_MODEM_CDMA_RM_PROTOCOL_UNKNOWN)
        map.insert("rm-protocol", (uint)bearer.rmProtocol);
    if (!bearer.number.isEmpty())
        map.insert("number", bearer.number);
    QDBusObjectPath obj = d->modemIface.CreateBearer(map);
    return obj.path();
}

void ModemManager::ModemInterface::deleteBearer(const QDBusObjectPath &bearer)
{
    Q_D(ModemInterface);
    d->modemIface.DeleteBearer(bearer);
}

void ModemManager::ModemInterface::reset()
{
    Q_D(ModemInterface);
    d->modemIface.Reset();
}

void ModemManager::ModemInterface::factoryReset(const QString &code)
{
    Q_D(ModemInterface);
    d->modemIface.FactoryReset(code);
}

void ModemManager::ModemInterface::setPowerState(MMModemPowerState state)
{
    Q_D(ModemInterface);
    d->modemIface.SetPowerState(state);
}

void ModemManager::ModemInterface::setCurrentCapabilities(Capabilities caps)
{
    Q_D(ModemInterface);
    d->modemIface.SetCurrentCapabilities((uint) caps);
}

void ModemManager::ModemInterface::setCurrentModes(const CurrentModesType &mode)
{
    Q_D(ModemInterface);
    d->modemIface.SetCurrentModes(mode);
}

void ModemManager::ModemInterface::setCurrentBands(const QList<MMModemBand> &bands)
{
    Q_D(ModemInterface);
    QList<uint> tmp;
    foreach (MMModemBand band, bands) {
        tmp.append(band);
    }
    d->modemIface.SetCurrentBands(tmp);
}

QString ModemManager::ModemInterface::command(const QString &cmd, uint timeout)
{
    Q_D(ModemInterface);
    d->modemIface.Command(cmd, timeout);
}

QString ModemManager::ModemInterface::simPath() const
{
    Q_D(const ModemInterface);
    return d->modemIface.sim().path();
}

QList<MMModemCapability> ModemManager::ModemInterface::supportedCapabilities() const
{
    Q_D(const ModemInterface);

    QList<MMModemCapability> result;
    foreach (uint cap, d->modemIface.supportedCapabilities()) {
        result.append((MMModemCapability)cap);
    }

    return result;
}

ModemManager::ModemInterface::Capabilities ModemManager::ModemInterface::currentCapabilities() const
{
    Q_D(const ModemInterface);
    return (Capabilities)d->modemIface.currentCapabilities();
}

uint ModemManager::ModemInterface::maxBearers() const
{
    Q_D(const ModemInterface);
    return d->modemIface.maxBearers();
}

uint ModemManager::ModemInterface::maxActiveBearers() const
{
    Q_D(const ModemInterface);
    return d->modemIface.maxActiveBearers();
}

QString ModemManager::ModemInterface::manufacturer() const
{
    Q_D(const ModemInterface);
    return d->modemIface.manufacturer();
}

QString ModemManager::ModemInterface::model() const
{
    Q_D(const ModemInterface);
    return d->modemIface.model();
}

QString ModemManager::ModemInterface::revision() const
{
    Q_D(const ModemInterface);
    return d->modemIface.revision();
}

QString ModemManager::ModemInterface::deviceIdentifier() const
{
    Q_D(const ModemInterface);
    return d->modemIface.deviceIdentifier();
}

QString ModemManager::ModemInterface::device() const
{
    Q_D(const ModemInterface);
    return d->modemIface.device();
}

QStringList ModemManager::ModemInterface::drivers() const
{
    Q_D(const ModemInterface);
    return d->modemIface.drivers();
}

QString ModemManager::ModemInterface::plugin() const
{
    Q_D(const ModemInterface);
    return d->modemIface.plugin();
}

QString ModemManager::ModemInterface::primaryPort() const
{
    Q_D(const ModemInterface);
    return d->modemIface.primaryPort();
}

QString ModemManager::ModemInterface::equipmentIdentifier() const
{
    Q_D(const ModemInterface);
    return d->modemIface.equipmentIdentifier();
}

MMModemLock ModemManager::ModemInterface::unlockRequired() const
{
    Q_D(const ModemInterface);
    return (MMModemLock)d->modemIface.unlockRequired();
}

UnlockRetriesMap ModemManager::ModemInterface::unlockRetries() const
{
    Q_D(const ModemInterface);
    return d->modemIface.unlockRetries();
}

MMModemState ModemManager::ModemInterface::state() const
{
    Q_D(const ModemInterface);
    return (MMModemState)d->modemIface.state();
}

MMModemStateFailedReason ModemManager::ModemInterface::stateFailedReason() const
{
    Q_D(const ModemInterface);
    return (MMModemStateFailedReason)d->modemIface.stateFailedReason();
}

ModemManager::ModemInterface::AccessTechnologies ModemManager::ModemInterface::accessTechnologies() const
{
    Q_D(const ModemInterface);
    return (AccessTechnologies)d->modemIface.accessTechnologies();
}

SignalQualityPair ModemManager::ModemInterface::signalQuality() const
{
    Q_D(const ModemInterface);
    return d->modemIface.signalQuality();
}

QStringList ModemManager::ModemInterface::ownNumbers() const
{
    Q_D(const ModemInterface);
    return d->modemIface.ownNumbers();
}

MMModemPowerState ModemManager::ModemInterface::powerState() const
{
    Q_D(const ModemInterface);
    return (MMModemPowerState)d->modemIface.powerState();
}

SupportedModesType ModemManager::ModemInterface::supportedModes() const
{
    Q_D(const ModemInterface);
    return d->modemIface.supportedModes();
}

CurrentModesType ModemManager::ModemInterface::currentModes() const
{
    Q_D(const ModemInterface);
    return d->modemIface.currentModes();
}

QList<MMModemBand> ModemManager::ModemInterface::supportedBands() const
{
    Q_D(const ModemInterface);
    QList<MMModemBand> result;
    foreach (uint band, d->modemIface.supportedBands()) {
        result.append((MMModemBand)band);
    }

    return result;
}

QList<MMModemBand> ModemManager::ModemInterface::currentBands() const
{
    Q_D(const ModemInterface);
    QList<MMModemBand> result;
    foreach (uint band, d->modemIface.currentBands()) {
        result.append((MMModemBand)band);
    }

    return result;
}

ModemManager::ModemInterface::IpBearerFamilies ModemManager::ModemInterface::supportedIpFamilies() const
{
    Q_D(const ModemInterface);
    return (IpBearerFamilies)d->modemIface.supportedIpFamilies();
}

void ModemManager::ModemInterface::onPropertiesChanged(const QString &ifaceName, const QVariantMap &changedProps, const QStringList &invalidatedProps)
{
    Q_D(ModemInterface);
    mmDebug() << ifaceName << changedProps.keys();

    if (ifaceName == QString(MM_DBUS_INTERFACE_MODEM)) {
        QLatin1String device(MM_MODEM_PROPERTY_DEVICE);
        QLatin1String drivers(MM_MODEM_PROPERTY_DRIVERS);
        QLatin1String enabled(MM_MODEM_PROPERTY_POWERSTATE);
        QLatin1String unlockRequired(MM_MODEM_PROPERTY_UNLOCKREQUIRED);
        QLatin1String signalQuality(MM_MODEM_PROPERTY_SIGNALQUALITY);
        QLatin1String tech(MM_MODEM_PROPERTY_ACCESSTECHNOLOGIES);
        QLatin1String currentModes(MM_MODEM_PROPERTY_CURRENTMODES);

        QVariantMap::const_iterator it = changedProps.constFind(device);
        if ( it != changedProps.constEnd()) {
            d->device = it->toString();
            emit deviceChanged(d->device);
        }
        it = changedProps.constFind(drivers);
        if ( it != changedProps.constEnd()) {
            d->drivers = it->toStringList();
            emit driversChanged(d->drivers);
        }
        it = changedProps.constFind(enabled);
        if ( it != changedProps.constEnd()) {
            emit enabledChanged(it->toBool());
        }
        it = changedProps.constFind(unlockRequired);
        if ( it != changedProps.constEnd()) {
            emit unlockRequiredChanged((MMModemLock)it->toUInt());
        }
        it = changedProps.constFind(tech);
        if ( it != changedProps.constEnd()) {
            emit accessTechnologyChanged(static_cast<AccessTechnologies>(it->toUInt()));
        }
        it = changedProps.constFind(currentModes);
        if ( it != changedProps.constEnd()) {
            emit currentModesChanged();
        }
        it = changedProps.constFind(signalQuality);
        if (it != changedProps.constEnd()) {
            SignalQualityPair pair = qdbus_cast<SignalQualityPair>(*it);
            if (pair.recent) {
                emit signalQualityChanged(pair.signal);
            }
        }
    }
}

void ModemManager::ModemInterface::onStateChanged(int oldState, int newState, uint reason)
{
    emit stateChanged((MMModemState) oldState, (MMModemState) newState, (MMModemStateChangeReason) reason);
}

/*** From org.freedesktop.ModemManager.Modem.Simple ***/

QDBusObjectPath ModemManager::ModemInterface::connectModem(const QVariantMap &properties)
{
    Q_D(ModemInterface);
    return d->modemSimpleIface.Connect(properties);
}

QVariantMap ModemManager::ModemInterface::status()
{
    Q_D(ModemInterface);
    return d->modemSimpleIface.GetStatus();
}

void ModemManager::ModemInterface::disconnectModem(const QDBusObjectPath &bearer)
{
    Q_D(ModemInterface);
    d->modemSimpleIface.Disconnect(bearer);
}

void ModemManager::ModemInterface::disconnectAllModems()
{
    disconnectModem(QDBusObjectPath("/"));
}
