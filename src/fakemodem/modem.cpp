/*
    Copyright 2015 Jan Grulich <jgrulich@redhat.com>

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

Modem::Modem(QObject* parent)
    : QObject(parent)
    , m_accessTechnologies(0)
    , m_currentCapabilities(0)
    , m_maxActiveBearers(0)
    , m_maxBearers(0)
    , m_powerState(0)
    , m_state(0)
    , m_stateFailedReason(0)
    , m_supportedBands{0}
    , m_supportedIpFamilies(0)
    , m_unlockRequired(0)
{
}

Modem::~Modem()
{
}

uint Modem::accessTechnologies() const
{
    return m_accessTechnologies;
}

QList< QDBusObjectPath > Modem::bearers() const
{
    return m_bearers;
}

UIntList Modem::currentBands() const
{
    return m_currentBands;
}

uint Modem::currentCapabilities() const
{
    return m_currentCapabilities;
}

CurrentModesType Modem::currentModes() const
{
    return m_currentModes;
}

QString Modem::device() const
{
    return m_device;
}

QString Modem::deviceIdentifier() const
{
    return m_deviceIdentifier;
}

QStringList Modem::drivers() const
{
    return m_drivers;
}

QString Modem::equipmentIdentifier() const
{
    return m_equipmentIdentifier;
}

QString Modem::manufacturer() const
{
    return m_manufacturer;
}

uint Modem::maxActiveBearers() const
{
    return m_maxActiveBearers;
}

uint Modem::maxBearers() const
{
    return m_maxBearers;
}

QString Modem::modemPath() const
{
    return m_path;
}

QString Modem::model() const
{
    return m_model;
}

QStringList Modem::ownNumbers() const
{
    return m_ownNumbers;
}

QString Modem::plugin() const
{
    return m_plugin;
}

PortList Modem::ports() const
{
    return m_ports;
}

uint Modem::powerState() const
{
    return m_powerState;
}

QString Modem::primaryPort() const
{
    return m_primaryPort;
}

QString Modem::revision() const
{
    return m_revision;
}

SignalQualityPair Modem::signalQuality() const
{
    return m_signalQuality;
}

QDBusObjectPath Modem::sim() const
{
    return m_sim;
}

int Modem::state() const
{
    return m_state;
}

uint Modem::stateFailedReason() const
{
    return m_stateFailedReason;
}

UIntList Modem::supportedBands() const
{
    return m_supportedBands;
}

UIntList Modem::supportedCapabilities() const
{
    return m_supportedCapabilities;
}

uint Modem::supportedIpFamilies() const
{
    return m_supportedIpFamilies;
}

SupportedModesType Modem::supportedModes() const
{
    return m_supportedModes;
}

uint Modem::unlockRequired() const
{
    return m_unlockRequired;
}

UnlockRetriesMap Modem::unlockRetries() const
{
    return m_unlockRetries;
}

void Modem::addBearer(const QDBusObjectPath& bearer)
{

}

void Modem::removeBearer(const QDBusObjectPath& bearer)
{

}

void Modem::setDevice(const QString& device)
{
    m_device = device;
}

void Modem::setDeviceIdentifier(const QString& deviceIdentifier)
{
    m_deviceIdentifier = deviceIdentifier;
}

void Modem::setDrivers(const QStringList& drivers)
{
    m_drivers = drivers;
}

void Modem::setEquipmentIdentifier(const QString& identifier)
{
    m_equipmentIdentifier = identifier;
}

void Modem::setManufacturer(const QString& manufacturer)
{
    m_manufacturer = manufacturer;
}

void Modem::setMaxActiveBearers(uint bearers)
{
    m_maxActiveBearers = bearers;
}

void Modem::setMaxBearers(uint bearers)
{
    m_maxBearers = bearers;
}

void Modem::setModemPath(const QString& path)
{
    m_path = path;
}

void Modem::setModel(const QString& model)
{
    m_model = model;
}

void Modem::setOwnNumbers(const QStringList& numbers)
{
    m_ownNumbers = numbers;
}

void Modem::setPlugin(const QString& plugin)
{
    m_plugin = plugin;
}

void Modem::setPorts(const PortList& ports)
{
    m_ports = ports;
}

void Modem::setPrimaryPort(const QString& port)
{
    m_primaryPort = port;
}

void Modem::setRevision(const QString& revision)
{
    m_revision = revision;
}

void Modem::setSignalQuality(const SignalQualityPair& signalQuality)
{
    m_signalQuality = signalQuality;
}

void Modem::setSim(const QDBusObjectPath& sim)
{
    m_sim = sim;
}

void Modem::setState(int state)
{
    m_state = state;
}

void Modem::setStateFailedReason(uint reason)
{
    m_stateFailedReason = reason;
}

void Modem::setSupportedBands(const UIntList& bands)
{
    m_supportedBands = bands;
}

void Modem::setSupportedCapabilities(const UIntList& capabilities)
{
    m_supportedCapabilities = capabilities;
}

void Modem::setSupportedIpFamilies(uint families)
{
    m_supportedIpFamilies = families;
}

void Modem::setSupportedModes(const SupportedModesType& modes)
{
    m_supportedModes = modes;
}

void Modem::setUnlockRequired(uint unlockRequired)
{
    m_unlockRequired = unlockRequired;
}

void Modem::setUnlockRetries(const UnlockRetriesMap& unlockRetries)
{
    m_unlockRetries = unlockRetries;
}

void Modem::setAccessTechnologies(uint technologies)
{
    m_accessTechnologies = technologies;
}

void Modem::SetCurrentBands(const QList< uint >& bands)
{
    m_currentBands = bands;
}

void Modem::SetCurrentCapabilities(uint capabilities)
{
    m_currentCapabilities = capabilities;
}

void Modem::SetCurrentModes(CurrentModesType modes)
{
    m_currentModes = modes;
}

void Modem::SetPowerState(uint state)
{
    m_powerState = state;
}

QString Modem::Command(const QString& cmd, uint timeout)
{
    // TODO
    return QString();
}

QDBusObjectPath Modem::CreateBearer(const QVariantMap& properties)
{
    // TODO
    return QDBusObjectPath();
}

void Modem::DeleteBearer(const QDBusObjectPath& bearer)
{
    // TODO
}

void Modem::Enable(bool enable)
{
    // TODO
}

void Modem::FactoryReset(const QString& code)
{
    // TODO
}

QList< QDBusObjectPath > Modem::ListBearers()
{
    // TODO
    return QList<QDBusObjectPath>();
}

void Modem::Reset()
{
    // TODO
}

QVariantMap Modem::toMap() const
{
    QVariantMap map;
    map.insert(QLatin1String(MM_MODEM_PROPERTY_SIM), QVariant::fromValue<QDBusObjectPath>(m_sim));
    map.insert(QLatin1String(MM_MODEM_PROPERTY_BEARERS), QVariant::fromValue<QList<QDBusObjectPath> >(m_bearers));
    map.insert(QLatin1String(MM_MODEM_PROPERTY_SUPPORTEDCAPABILITIES), QVariant::fromValue<UIntList>(m_supportedCapabilities));
    map.insert(QLatin1String(MM_MODEM_PROPERTY_CURRENTCAPABILITIES), m_currentCapabilities);
    map.insert(QLatin1String(MM_MODEM_PROPERTY_MAXBEARERS), m_maxBearers);
    map.insert(QLatin1String(MM_MODEM_PROPERTY_MAXACTIVEBEARERS), m_maxActiveBearers);
    map.insert(QLatin1String(MM_MODEM_PROPERTY_MANUFACTURER), m_manufacturer);
    map.insert(QLatin1String(MM_MODEM_PROPERTY_MODEL), m_model);
    map.insert(QLatin1String(MM_MODEM_PROPERTY_REVISION), m_revision);
    map.insert(QLatin1String(MM_MODEM_PROPERTY_DEVICEIDENTIFIER), m_deviceIdentifier);
    map.insert(QLatin1String(MM_MODEM_PROPERTY_DEVICE), m_device);
    map.insert(QLatin1String(MM_MODEM_PROPERTY_DRIVERS), m_drivers);
    map.insert(QLatin1String(MM_MODEM_PROPERTY_PLUGIN), m_plugin);
    map.insert(QLatin1String(MM_MODEM_PROPERTY_PRIMARYPORT), m_primaryPort);
    map.insert(QLatin1String(MM_MODEM_PROPERTY_PORTS), QVariant::fromValue<PortList>(m_ports));
    map.insert(QLatin1String(MM_MODEM_PROPERTY_EQUIPMENTIDENTIFIER), m_equipmentIdentifier);
    map.insert(QLatin1String(MM_MODEM_PROPERTY_UNLOCKREQUIRED), m_unlockRequired);
    map.insert(QLatin1String(MM_MODEM_PROPERTY_UNLOCKRETRIES), QVariant::fromValue<UnlockRetriesMap>(m_unlockRetries));
    map.insert(QLatin1String(MM_MODEM_PROPERTY_STATE), m_state);
    map.insert(QLatin1String(MM_MODEM_PROPERTY_STATEFAILEDREASON), m_stateFailedReason);
    map.insert(QLatin1String(MM_MODEM_PROPERTY_ACCESSTECHNOLOGIES), m_accessTechnologies);
    map.insert(QLatin1String(MM_MODEM_PROPERTY_SIGNALQUALITY), QVariant::fromValue<SignalQualityPair>(m_signalQuality));
    map.insert(QLatin1String(MM_MODEM_PROPERTY_OWNNUMBERS), m_ownNumbers);
    map.insert(QLatin1String(MM_MODEM_PROPERTY_POWERSTATE), m_powerState);
    map.insert(QLatin1String(MM_MODEM_PROPERTY_SUPPORTEDMODES), QVariant::fromValue<SupportedModesType>(m_supportedModes));
    map.insert(QLatin1String(MM_MODEM_PROPERTY_CURRENTMODES), QVariant::fromValue<CurrentModesType>(m_currentModes));
    map.insert(QLatin1String(MM_MODEM_PROPERTY_SUPPORTEDBANDS), QVariant::fromValue<UIntList>(m_supportedBands));
    map.insert(QLatin1String(MM_MODEM_PROPERTY_CURRENTBANDS), QVariant::fromValue<UIntList>(m_currentBands));
    map.insert(QLatin1String(MM_MODEM_PROPERTY_SUPPORTEDIPFAMILIES), m_supportedIpFamilies);
    return map;
}

