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

#ifndef MODEMMANAGERQT_MODEMINTERFACE_H
#define MODEMMANAGERQT_MODEMINTERFACE_H

#include <ModemManager/ModemManager.h>

#include "ModemManagerQt-export.h"

#include <QObject>
#include <QVariant>
#include <QSharedPointer>
#include <QDBusObjectPath>

#include "generic-types.h"
#include "interface.h"

class ModemInterfacePrivate;

namespace ModemManager
{
class MODEMMANAGERQT_EXPORT ModemInterface : public Interface
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(ModemInterface)

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

    struct BearerStruct {
        QString apn; // required for 3GPP
        MMBearerIpFamily ipType;
        MMBearerAllowedAuth allowedAuth;
        QString user;
        QString password;
        bool allowRoaming;
        MMModemCdmaRmProtocol rmProtocol;
        QString number; // required for POTS
    };

    explicit ModemInterface( const QString &path, QObject *parent = 0 );
    ~ModemInterface();

    QString uni() const;
    bool isEnabled() const;
    bool isValid() const;

    // From org.freedesktop.ModemManager.Modem
    // methods
    void enable(bool enable);

    QStringList listBearers();
    QString createBearer(const BearerStruct &bearer);
    void deleteBearer(const QDBusObjectPath &bearer);

    void reset();
    void factoryReset(const QString &code);
    void setPowerState(MMModemPowerState state);
    void setCurrentCapabilities(Capabilities caps);
    void setCurrentModes(const CurrentModesType &mode);
    void setCurrentBands(const QList<MMModemBand> &bands);
    QString command(const QString &cmd, uint timeout);

    // properties
    QString simPath() const;
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

    //  Dictionary of properties needed to get the modem connected.
    //  Each implementation is free to add its own specific key-value pairs. The predefined
    //  common ones are:
    //
    //  "pin": SIM-PIN unlock code, given as a string value (signature "s").
    //  "operator-id": ETSI MCC-MNC of a network to force registration with, given as a string value (signature "s").
    //  "apn": For GSM/UMTS and LTE devices the APN to use, given as a string value (signature "s").
    //  "ip-type": For GSM/UMTS and LTE devices the IP addressing type to use, given as a MMBearerIpFamily value (signature "u").
    //  "allowed-auth": The authentication method to use, given as a MMBearerAllowedAuth value (signature "u"). Optional in 3GPP.
    //  "user": User name (if any) required by the network, given as a string value (signature "s"). Optional in 3GPP.
    //  "password": Password (if any) required by the network, given as a string value (signature "s"). Optional in 3GPP.
    //  "number": For POTS devices the number to dial,, given as a string value (signature "s").
    //  "allow-roaming": FALSE to allow only connections to home networks, given as a boolean value (signature "b").
    //  "rm-protocol": For CDMA devices, the protocol of the Rm interface, given as a MMModemCdmaRmProtocol value (signature "u").
    QDBusObjectPath connectModem(const QVariantMap &properties);

    //  Dictionary of properties.
    //  Each implementation is free to add it's own specific key-value pairs. The predefined
    //  common ones are:
    //
    //  "state": A MMModemState value specifying the overall state of the modem, given as an unsigned integer value (signature "u").
    //  "signal-quality": Signal quality value, given only when registered, as an unsigned integer value (signature "u").
    //  "current-bands": List of MMModemBand values, given only when registered, as a list of unsigned integer values (signature "au").
    //  "access-technology": A MMModemAccessTechnology value, given only when registered, as an unsigned integer value (signature "u").
    //  "m3gpp-registration-state":A MMModem3gppRegistrationState value specifying the state of the registration, given only when registered in a 3GPP network, as an unsigned integer value (signature "u").
    //  "m3gpp-operator-code":Operator MCC-MNC, given only when registered in a 3GPP network, as a string value (signature "s").
    //  "m3gpp-operator-name":Operator name, given only when registered in a 3GPP network, as a string value (signature "s").
    //  "cdma-cdma1x-registration-state":A MMModemCdmaRegistrationState value specifying the state of the registration, given only when registered in a CDMA1x network, as an unsigned integer value (signature "u").
    //  "cdma-evdo-registration-state":A MMModemCdmaRegistrationState value specifying the state of the registration, given only when registered in a EV-DO network, as an unsigned integer value (signature "u").
    //  "cdma-sid":The System Identifier of the serving network, if registered in a CDMA1x network and if known. Given as an unsigned integer value (signature "u").
    //  "cdma-nid":The Network Identifier of the serving network, if registered in a CDMA1x network and if known. Given as an unsigned integer value (signature "u").
    QVariantMap status();

    void disconnectModem(const QDBusObjectPath &bearer);
    void disconnectAllModems();

Q_SIGNALS:
    void deviceChanged(const QString &device);
    void driversChanged(const QStringList &drivers);
    void enabledChanged(bool enabled);
    void unlockRequiredChanged(MMModemLock lock);
    void stateChanged(MMModemState oldState, MMModemState newState, MMModemStateChangeReason reason);
    void signalQualityChanged(uint percentStrength);
    void accessTechnologyChanged(ModemManager::ModemInterface::AccessTechnologies tech);
    void currentModesChanged();

private Q_SLOTS:
    void onPropertiesChanged(const QString &ifaceName, const QVariantMap &changedProps, const QStringList &invalidatedProps);
    void onStateChanged(int oldState, int newState, uint reason);

private:
    void init();
};

Q_DECLARE_OPERATORS_FOR_FLAGS(ModemInterface::Capabilities)
Q_DECLARE_OPERATORS_FOR_FLAGS(ModemInterface::AccessTechnologies)
Q_DECLARE_OPERATORS_FOR_FLAGS(ModemInterface::ModemModes)
Q_DECLARE_OPERATORS_FOR_FLAGS(ModemInterface::IpBearerFamilies)

} // namespace ModemManager

#endif
