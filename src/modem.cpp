/*
    Copyright 2008,2011 Will Stephenson <wstephenson@kde.org>
    Copyright 2010 Lamarque Souza <lamarque@kde.org>
    Copyright 2013 Daniel Nicoletti <dantti12@gmail.com>
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

#include "modem.h"
#include "modem_p.h"

#include "dbus/dbus.h"
#include "generictypes.h"
#include "mmdebug.h"

#include <QDomDocument>


ModemPrivate::ModemPrivate(const QString &path)
    : InterfacePrivate(path)
    , modemIface(MM_DBUS_SERVICE, path, QDBusConnection::systemBus())
    , modemSimpleIface(MM_DBUS_SERVICE, path, QDBusConnection::systemBus())
{
    device = modemIface.device();
    drivers = modemIface.drivers();
    simPath = modemIface.sim().path();
}

ModemManager::Modem::Modem(const QString &path, QObject *parent)
    : Interface(*new ModemPrivate(path), parent)
{
    Q_D(Modem);
    if (d->modemIface.isValid()) {
        QDBusConnection::systemBus().connect(MM_DBUS_SERVICE, d->uni, DBUS_INTERFACE_PROPS, "PropertiesChanged", this,
                                             SLOT(onPropertiesChanged(QString,QVariantMap,QStringList)));
    }

    connect(&d->modemIface, &OrgFreedesktopModemManager1ModemInterface::StateChanged, this, &Modem::onStateChanged);
}

ModemManager::Modem::~Modem()
{
}

QString ModemManager::Modem::uni() const
{
    Q_D(const Modem);
    return d->uni;
}

bool ModemManager::Modem::isEnabled() const
{
    Q_D(const Modem);
    return (MMModemPowerState)d->modemIface.powerState() == MM_MODEM_POWER_STATE_ON;
}

bool ModemManager::Modem::isValid() const
{
    Q_D(const Modem);
    return d->modemIface.isValid();
}

void ModemManager::Modem::setEnabled(bool enable)
{
    Q_D(Modem);
    d->modemIface.Enable(enable);
}

QString ModemManager::Modem::createBearer(const BearerStruct &bearer)
{
    Q_D(Modem);
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

void ModemManager::Modem::deleteBearer(const QString &bearer)
{
    Q_D(Modem);
    d->modemIface.DeleteBearer(QDBusObjectPath(bearer));
}

QStringList ModemManager::Modem::listBearers()
{
    Q_D(const Modem);
    QStringList result;

#if MM_CHECK_VERSION(1, 2, 0)
    QList<QDBusObjectPath> bearers = d->modemIface.bearers();
#else
    QList<QDBusObjectPath> bearers = const_cast<ModemPrivate *>(d)->modemIface.ListBearers();
#endif

    foreach(const QDBusObjectPath &path, bearers) {
        result.append(path.path());
    }

    return result;
}

void ModemManager::Modem::reset()
{
    Q_D(Modem);
    d->modemIface.Reset();
}

void ModemManager::Modem::factoryReset(const QString &code)
{
    Q_D(Modem);
    d->modemIface.FactoryReset(code);
}

void ModemManager::Modem::setPowerState(MMModemPowerState state)
{
    Q_D(Modem);
    d->modemIface.SetPowerState(state);
}

void ModemManager::Modem::setCurrentCapabilities(Capabilities caps)
{
    Q_D(Modem);
    d->modemIface.SetCurrentCapabilities((uint) caps);
}

void ModemManager::Modem::setCurrentModes(const CurrentModesType &mode)
{
    Q_D(Modem);
    d->modemIface.SetCurrentModes(mode);
}

void ModemManager::Modem::setCurrentBands(const QList<MMModemBand> &bands)
{
    Q_D(Modem);
    QList<uint> tmp;
    foreach (const MMModemBand band, bands) {
        tmp.append(band);
    }
    d->modemIface.SetCurrentBands(tmp);
}

QDBusPendingReply<QString> ModemManager::Modem::command(const QString &cmd, uint timeout)
{
    Q_D(Modem);
    return d->modemIface.Command(cmd, timeout);
}

QString ModemManager::Modem::simPath() const
{
    Q_D(const Modem);
    return d->simPath;
}

QList<MMModemCapability> ModemManager::Modem::supportedCapabilities() const
{
    Q_D(const Modem);

    QList<MMModemCapability> result;
    foreach (uint cap, d->modemIface.supportedCapabilities()) {
        result.append((MMModemCapability)cap);
    }

    return result;
}

ModemManager::Modem::Capabilities ModemManager::Modem::currentCapabilities() const
{
    Q_D(const Modem);
    return (Capabilities)d->modemIface.currentCapabilities();
}

uint ModemManager::Modem::maxBearers() const
{
    Q_D(const Modem);
    return d->modemIface.maxBearers();
}

uint ModemManager::Modem::maxActiveBearers() const
{
    Q_D(const Modem);
    return d->modemIface.maxActiveBearers();
}

QString ModemManager::Modem::manufacturer() const
{
    Q_D(const Modem);
    return d->modemIface.manufacturer();
}

QString ModemManager::Modem::model() const
{
    Q_D(const Modem);
    return d->modemIface.model();
}

QString ModemManager::Modem::revision() const
{
    Q_D(const Modem);
    return d->modemIface.revision();
}

QString ModemManager::Modem::deviceIdentifier() const
{
    Q_D(const Modem);
    return d->modemIface.deviceIdentifier();
}

QString ModemManager::Modem::device() const
{
    Q_D(const Modem);
    return d->modemIface.device();
}

QStringList ModemManager::Modem::drivers() const
{
    Q_D(const Modem);
    return d->modemIface.drivers();
}

QString ModemManager::Modem::plugin() const
{
    Q_D(const Modem);
    return d->modemIface.plugin();
}

QString ModemManager::Modem::primaryPort() const
{
    Q_D(const Modem);
    return d->modemIface.primaryPort();
}

PortList ModemManager::Modem::ports() const
{
    Q_D(const Modem);
    return d->modemIface.ports();
}

QString ModemManager::Modem::equipmentIdentifier() const
{
    Q_D(const Modem);
    return d->modemIface.equipmentIdentifier();
}

MMModemLock ModemManager::Modem::unlockRequired() const
{
    Q_D(const Modem);
    return (MMModemLock)d->modemIface.unlockRequired();
}

UnlockRetriesMap ModemManager::Modem::unlockRetries() const
{
    Q_D(const Modem);
    return d->modemIface.unlockRetries();
}

MMModemState ModemManager::Modem::state() const
{
    Q_D(const Modem);
    return (MMModemState)d->modemIface.state();
}

MMModemStateFailedReason ModemManager::Modem::stateFailedReason() const
{
    Q_D(const Modem);
    return (MMModemStateFailedReason)d->modemIface.stateFailedReason();
}

ModemManager::Modem::AccessTechnologies ModemManager::Modem::accessTechnologies() const
{
    Q_D(const Modem);
    return (AccessTechnologies)d->modemIface.accessTechnologies();
}

SignalQualityPair ModemManager::Modem::signalQuality() const
{
    Q_D(const Modem);
    return d->modemIface.signalQuality();
}

QStringList ModemManager::Modem::ownNumbers() const
{
    Q_D(const Modem);
    return d->modemIface.ownNumbers();
}

MMModemPowerState ModemManager::Modem::powerState() const
{
    Q_D(const Modem);
    return (MMModemPowerState)d->modemIface.powerState();
}

SupportedModesType ModemManager::Modem::supportedModes() const
{
    Q_D(const Modem);
    return d->modemIface.supportedModes();
}

CurrentModesType ModemManager::Modem::currentModes() const
{
    Q_D(const Modem);
    return d->modemIface.currentModes();
}

QList<MMModemBand> ModemManager::Modem::supportedBands() const
{
    Q_D(const Modem);
    QList<MMModemBand> result;
    foreach (uint band, d->modemIface.supportedBands()) {
        result.append((MMModemBand)band);
    }

    return result;
}

QList<MMModemBand> ModemManager::Modem::currentBands() const
{
    Q_D(const Modem);
    QList<MMModemBand> result;
    foreach (uint band, d->modemIface.currentBands()) {
        result.append((MMModemBand)band);
    }

    return result;
}

ModemManager::Modem::IpBearerFamilies ModemManager::Modem::supportedIpFamilies() const
{
    Q_D(const Modem);
    return (IpBearerFamilies)d->modemIface.supportedIpFamilies();
}

void ModemManager::Modem::onPropertiesChanged(const QString &ifaceName, const QVariantMap &changedProps, const QStringList &invalidatedProps)
{
    Q_UNUSED(invalidatedProps);
    Q_D(Modem);
    qCDebug(MMQT) << ifaceName << changedProps.keys();

    if (ifaceName == QString(MM_DBUS_INTERFACE_MODEM)) {
        QLatin1String device(MM_MODEM_PROPERTY_DEVICE);
        QLatin1String drivers(MM_MODEM_PROPERTY_DRIVERS);
        QLatin1String enabled(MM_MODEM_PROPERTY_POWERSTATE);
        QLatin1String unlockRequired(MM_MODEM_PROPERTY_UNLOCKREQUIRED);
        QLatin1String signalQuality(MM_MODEM_PROPERTY_SIGNALQUALITY);
        QLatin1String tech(MM_MODEM_PROPERTY_ACCESSTECHNOLOGIES);
        QLatin1String currentModes(MM_MODEM_PROPERTY_CURRENTMODES);
        QLatin1String simPath(MM_MODEM_PROPERTY_SIM);
        QLatin1String powerState(MM_MODEM_PROPERTY_POWERSTATE);
#if MM_CHECK_VERSION(1,1,900)
        QLatin1String bearers(MM_MODEM_PROPERTY_BEARERS);
#endif

        QVariantMap::const_iterator it = changedProps.constFind(device);
        if (it != changedProps.constEnd()) {
            d->device = it->toString();
            emit deviceChanged(d->device);
        }
        it = changedProps.constFind(drivers);
        if (it != changedProps.constEnd()) {
            d->drivers = it->toStringList();
            emit driversChanged(d->drivers);
        }
        it = changedProps.constFind(enabled);
        if (it != changedProps.constEnd()) {
            emit enabledChanged(it->toBool());
        }
        it = changedProps.constFind(unlockRequired);
        if (it != changedProps.constEnd()) {
            emit unlockRequiredChanged((MMModemLock)it->toUInt());
        }
        it = changedProps.constFind(tech);
        if (it != changedProps.constEnd()) {
            emit accessTechnologyChanged(static_cast<AccessTechnologies>(it->toUInt()));
        }
        it = changedProps.constFind(currentModes);
        if (it != changedProps.constEnd()) {
            emit currentModesChanged();
        }
        it = changedProps.constFind(signalQuality);
        if (it != changedProps.constEnd()) {
            SignalQualityPair pair = qdbus_cast<SignalQualityPair>(*it);
            if (pair.recent) {
                emit signalQualityChanged(pair.signal);
            }
        }
        it = changedProps.constFind(simPath);
        if (it != changedProps.constEnd()) {
            const QString path = qdbus_cast<QDBusObjectPath>(*it).path();
            emit simPathChanged(d->simPath, path);
            d->simPath = path;
        }
        it = changedProps.constFind(powerState);
        if (it != changedProps.constEnd()) {
            emit powerStateChanged((MMModemPowerState)it->toUInt());
        }
#if MM_CHECK_VERSION(1, 1, 900)
        it = changedProps.constFind(bearers);
        if (it != changedProps.constEnd()) {
            emit bearersChanged();
        }
#endif
    }
}

void ModemManager::Modem::onStateChanged(int oldState, int newState, uint reason)
{
    emit stateChanged((MMModemState) oldState, (MMModemState) newState, (MMModemStateChangeReason) reason);
}

/*** From org.freedesktop.ModemManager.Modem.Simple ***/

QDBusObjectPath ModemManager::Modem::connectModem(const QVariantMap &properties)
{
    Q_D(Modem);
    return d->modemSimpleIface.Connect(properties);
}

QVariantMap ModemManager::Modem::status()
{
    Q_D(Modem);
    return d->modemSimpleIface.GetStatus();
}

void ModemManager::Modem::disconnectModem(const QString &bearer)
{
    Q_D(Modem);
    d->modemSimpleIface.Disconnect(QDBusObjectPath(bearer));
}

void ModemManager::Modem::disconnectAllModems()
{
    disconnectModem(QStringLiteral("/"));
}
