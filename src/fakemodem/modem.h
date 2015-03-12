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

#ifndef MODEMMANAGERQT_FAKE_MODEM_MODEM_H
#define MODEMMANAGERQT_FAKE_MODEM_MODEM_H

#include "dbus/fakedbus.h"
#include "generictypes.h"
#include "generictypes_p.h"

#include <QObject>

#include <QCoreApplication>
#include <QDBusObjectPath>
#include <QDBusPendingReply>

class Modem : public QObject
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "org.kde.fakemodem.Modem")
public:
    explicit Modem(QObject* parent = 0);
    virtual ~Modem();

    Q_PROPERTY(uint AccessTechnologies READ accessTechnologies)
    Q_PROPERTY(QList<QDBusObjectPath> Bearers READ bearers)
    Q_PROPERTY(UIntList CurrentBands READ currentBands)
    Q_PROPERTY(uint CurrentCapabilities READ currentCapabilities)
    Q_PROPERTY(CurrentModesType CurrentModes READ currentModes)
    Q_PROPERTY(QString Device READ device)
    Q_PROPERTY(QString DeviceIdentifier READ deviceIdentifier)
    Q_PROPERTY(QStringList Drivers READ drivers)
    Q_PROPERTY(QString EquipmentIdentifier READ equipmentIdentifier)
    Q_PROPERTY(QString Manufacturer READ manufacturer)
    Q_PROPERTY(uint MaxActiveBearers READ maxActiveBearers)
    Q_PROPERTY(uint MaxBearers READ maxBearers)
    Q_PROPERTY(QString Model READ model)
    Q_PROPERTY(QStringList OwnNumbers READ ownNumbers)
    Q_PROPERTY(QString Plugin READ plugin)
    Q_PROPERTY(PortList Ports READ ports)
    Q_PROPERTY(uint PowerState READ powerState)
    Q_PROPERTY(QString PrimaryPort READ primaryPort)
    Q_PROPERTY(QString Revision READ revision)
    Q_PROPERTY(SignalQualityPair SignalQuality READ signalQuality)
    Q_PROPERTY(QDBusObjectPath Sim READ sim)
    Q_PROPERTY(int State READ state)
    Q_PROPERTY(uint StateFailedReason READ stateFailedReason)
    Q_PROPERTY(UIntList SupportedBands READ supportedBands)
    Q_PROPERTY(UIntList SupportedCapabilities READ supportedCapabilities)
    Q_PROPERTY(uint SupportedIpFamilies READ supportedIpFamilies)
    Q_PROPERTY(SupportedModesType SupportedModes READ supportedModes)
    Q_PROPERTY(uint UnlockRequired READ unlockRequired)
    Q_PROPERTY(UnlockRetriesMap UnlockRetries READ unlockRetries)

    uint accessTechnologies() const;
    QList<QDBusObjectPath> bearers() const;
    UIntList currentBands() const;
    uint currentCapabilities() const;
    CurrentModesType currentModes() const;
    QString device() const;
    QString deviceIdentifier() const;
    QStringList drivers() const;
    QString equipmentIdentifier() const;
    QString manufacturer() const;
    uint maxActiveBearers() const;
    uint maxBearers() const;
    QString model() const;
    QStringList ownNumbers() const;
    QString plugin() const;
    PortList ports() const;
    uint powerState() const;
    QString primaryPort() const;
    QString revision() const;
    SignalQualityPair signalQuality() const;
    QDBusObjectPath sim() const;
    int state() const;
    uint stateFailedReason() const;
    UIntList supportedBands() const;
    UIntList supportedCapabilities() const;
    uint supportedIpFamilies() const;
    SupportedModesType supportedModes() const;
    uint unlockRequired() const;
    UnlockRetriesMap unlockRetries() const;

    /* Not part of DBus interface */
    void addBearer(const QDBusObjectPath &bearer);
    void removeBearer(const QDBusObjectPath &bearer);
    QString modemPath() const;
    void setAccessTechnologies(uint technologies);
    void setDevice(const QString &device);
    void setDeviceIdentifier(const QString &deviceIdentifier);
    void setDrivers(const QStringList &drivers);
    void setEquipmentIdentifier(const QString &identifier);
    void setManufacturer(const QString &manufacturer);
    void setMaxActiveBearers(uint bearers);
    void setMaxBearers(uint bearers);
    void setModemPath(const QString &path);
    void setModel(const QString &model);
    void setOwnNumbers(const QStringList &numbers);
    void setPlugin(const QString &plugin);
    void setPorts(const PortList &ports);
    void setPrimaryPort(const QString &port);
    void setRevision(const QString &revision);
    void setSignalQuality(const SignalQualityPair &signalQuality);
    void setSim(const QDBusObjectPath &sim);
    void setState(int state);
    void setStateFailedReason(uint reason);
    void setSupportedBands(const UIntList &bands);
    void setSupportedCapabilities(const UIntList &capabilities);
    void setSupportedIpFamilies(uint families);
    void setSupportedModes(const SupportedModesType &modes);
    void setUnlockRequired(uint unlockRequired);
    void setUnlockRetries(const UnlockRetriesMap &unlockRetries);

    QVariantMap toMap() const;

public Q_SLOTS:
    Q_SCRIPTABLE QString Command(const QString &cmd, uint timeout);
    Q_SCRIPTABLE QDBusObjectPath CreateBearer(const QVariantMap &properties);
    Q_SCRIPTABLE void DeleteBearer(const QDBusObjectPath &bearer);
    Q_SCRIPTABLE void Enable(bool enable);
    Q_SCRIPTABLE void FactoryReset(const QString &code);
    Q_SCRIPTABLE QList<QDBusObjectPath> ListBearers();
    Q_SCRIPTABLE void Reset();
    Q_SCRIPTABLE void SetCurrentBands(const QList<uint> &bands);
    Q_SCRIPTABLE void SetCurrentCapabilities(uint capabilities);
    Q_SCRIPTABLE void SetCurrentModes(CurrentModesType modes);
    Q_SCRIPTABLE void SetPowerState(uint state);

Q_SIGNALS:
    Q_SCRIPTABLE void StateChanged(int oldState, int newState, uint reason);

private:
    uint m_accessTechnologies;
    QList<QDBusObjectPath> m_bearers;
    UIntList m_currentBands;
    uint m_currentCapabilities;
    CurrentModesType m_currentModes;
    QString m_device;
    QString m_deviceIdentifier;
    QStringList m_drivers;
    QString m_equipmentIdentifier;
    QString m_manufacturer;
    uint m_maxActiveBearers;
    uint m_maxBearers;
    QString m_path;
    QString m_model;
    QStringList m_ownNumbers;
    QString m_plugin;
    PortList m_ports;
    uint m_powerState;
    QString m_primaryPort;
    QString m_revision;
    SignalQualityPair m_signalQuality;
    QDBusObjectPath m_sim;
    int m_state;
    uint m_stateFailedReason;
    UIntList m_supportedBands;
    UIntList m_supportedCapabilities;
    uint m_supportedIpFamilies;
    SupportedModesType m_supportedModes;
    uint m_unlockRequired;
    UnlockRetriesMap m_unlockRetries;
};

#endif

