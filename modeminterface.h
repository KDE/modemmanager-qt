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

#ifndef MODEMMANAGER_MODEMINTERFACE_H
#define MODEMMANAGER_MODEMINTERFACE_H

#include <ModemManager/ModemManager.h>

#include "ModemManagerQt-export.h"

#include <QObject>
#include <QVariant>
#include <QSharedPointer>
#include <QDBusObjectPath>

#include "../dbus/generic-types.h"

class ModemInterfacePrivate;

namespace ModemManager
{
class MODEMMANAGERQT_EXPORT ModemInterface : public QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(ModemInterface)

    Q_PROPERTY(QString udi READ udi)

    Q_FLAGS(MMModemCapability)
    Q_FLAGS(MMModemAccessTechnology)
    Q_FLAGS(MMModemMode)
    Q_FLAGS(MMBearerIpFamily)

public:
    Q_DECLARE_FLAGS(Capabilities, MMModemCapability)
    Q_DECLARE_FLAGS(AccessTechnologies, MMModemAccessTechnology)
    Q_DECLARE_FLAGS(ModemModes, MMModemMode)
    Q_DECLARE_FLAGS(IpBearerFamilies, MMBearerIpFamily)

    typedef QSharedPointer<ModemInterface> Ptr;
    typedef QList<Ptr> List;

    enum InterfaceType {
        Gsm,
        GsmUssd,
        Cdma,
        SimCard,
        Messaging,
        Sms,
        Location,
        Bearer,
        Time
        //, Firmware  // TODO ?
    };

    explicit ModemInterface( const QString & path, QObject * parent = 0 );
    explicit ModemInterface( ModemInterfacePrivate &dd, QObject * parent = 0);
    virtual ~ModemInterface();

    QString udi() const;
    bool isEnabled() const;
    bool isValid() const;

    bool hasInterface(const QString & name) const;
    QStringList interfaces() const;

    bool isGsmModem() const;
    bool isCdmaModem() const;

    // From org.freedesktop.ModemManager.Modem
    // methods
    void enable(bool enable);

    QList<QDBusObjectPath> listBearers();
    QDBusObjectPath createBearer(const QVariantMap & properties);
    void deleteBearer(const QDBusObjectPath & bearer);

    void reset();
    void factoryReset(const QString & code);
    void setPowerState(MMModemPowerState state);
    void setCurrentCapabilities(Capabilities caps);
    void setCurrentModes(const CurrentModesType & mode);
    void setCurrentBands(const QList<MMModemBand> & bands);
    QString command(const QString & cmd, uint timeout);

    // properties
    QDBusObjectPath simPath() const;
    QList<MMModemCapability> supportedCapabilities() const;
    Capabilities currentCapabilities() const;
    uint maxBearers() const;
    uint maxActiveBearers() const;
    QString manufacturer() const;
    QString model() const;
    QString revision() const;
    QString deviceIdentifier() const;
    QString device() const;
    QStringList drivers() const;
    QString plugin() const;
    QString primaryPort() const;
    QString equipmentIdentifier() const;
    MMModemLock unlockRequired() const;
    UnlockRetriesMap unlockRetries() const;
    MMModemState state() const;
    MMModemStateFailedReason stateFailedReason() const;
    AccessTechnologies accessTechnologies() const;
    SignalQualityPair signalQuality() const;
    QStringList ownNumbers() const;
    MMModemPowerState powerState() const;
    SupportedModesType supportedModes() const;
    CurrentModesType currentModes() const;
    QList<MMModemBand> supportedBands() const;
    QList<MMModemBand> currentBands() const;
    IpBearerFamilies supportedIpFamilies() const;


    // From org.freedesktop.ModemManager.Modem.Simple

    //    Dictionary of properties needed to get the modem connected.
    //    Each implementation is free to add its own specific key-value pairs. The predefined
    //    common ones are:

    //      'pin'          : string
    //      'network_id'   : string (GSM/HSPA only)
    //      'band'         : uint
    //      'network_mode' : uint
    //      'apn'          : string (GSM/HSPA only)
    //      'number'       : string
    //      'rm-protocol'  : uint (CDMA/EVDO only) (1 - Relay, 2 - Network PPP)
    //      'allowed_auth' : uint (GSM/HSPA only) (MM_MODEM_GSM_ALLOWED_AUTH bitfield)
    void connectModem(const QVariantMap & properties);

    //    Dictionary of properties.
    //     Each implementation is free to add it's own specific key-value pairs. The predefined
    //     common ones are:
    //
    //    'state'          : uint (always)
    //    'signal_quality' : uint  (state >= registered)
    //    'operator_code'  : string (state >= registered)
    //    'operator_name'  : string (state >= registered)
    //    'band'           : uint (state >= registered)
    //    'network_mode'   : uint (state >= registered)
    //
    QVariantMap status();

    void disconnectModem(const QDBusObjectPath &bearer);
    void disconnectAllModems();

Q_SIGNALS:
    void deviceChanged(const QString & device);
    void driversChanged(const QStringList & drivers);
    void enabledChanged(bool enabled);
    void unlockRequiredChanged(MMModemLock lock);
    void stateChanged(MMModemState oldState, MMModemState newState, MMModemStateChangeReason reason);

private Q_SLOTS:
    void onInterfacesAdded(const QDBusObjectPath &object_path, const QVariantMapMap &interfaces_and_properties);
    void onInterfacesRemoved(const QDBusObjectPath &object_path, const QStringList &interfaces);
    void onPropertiesChanged(const QString &ifaceName, const QVariantMap &changedProps, const QStringList &invalidatedProps);
    void onStateChanged(int oldState, int newState, uint reason);

private:
    void init();
    void initInterfaces();
    QString introspect() const;

protected:
    ModemInterfacePrivate * d_ptr;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(ModemInterface::Capabilities)
Q_DECLARE_OPERATORS_FOR_FLAGS(ModemInterface::AccessTechnologies)
Q_DECLARE_OPERATORS_FOR_FLAGS(ModemInterface::ModemModes)
Q_DECLARE_OPERATORS_FOR_FLAGS(ModemInterface::IpBearerFamilies)

} // namespace ModemManager

#endif
