/*
Copyright 2008,2011 Will Stephenson <wstephenson@kde.org>
Copyright 2010 Lamarque Souza <lamarque@kde.org>

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License as
published by the Free Software Foundation; either version 2 of
the License or (at your option) version 3 or any later version
accepted by the membership of KDE e.V. (or its successor approved
by the membership of KDE e.V.), which shall act as a proxy
defined in Section 14 of version 3 of the license.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef MODEMMANAGER_MODEMGSMNETWORKINTERFACE_H
#define MODEMMANAGER_MODEMGSMNETWORKINTERFACE_H

#include "mmqt-export.h"

#include <QObject>
#include "modeminterface.h"

class ModemGsmNetworkInterfacePrivate;

namespace ModemManager
{
class MMQT_EXPORT ModemGsmNetworkInterface : public ModemInterface
{
Q_OBJECT
Q_DECLARE_PRIVATE(ModemGsmNetworkInterface)

public:
    typedef QList< QMap<QString,QString> > ScanResultsType;

    /* GSM registration code as defined in 3GPP TS 27.007 section 10.1.19. */
    enum RegistrationStatus {
        StatusIdle = 0x0, /* Not registered, not searching for new operator to register. */
        StatusHome, /* Registered on home network. */
        StatusSearching, /* Not registered, searching for new operator to register with. */
        StatusDenied, /* Registration denied. */
        StatusUnknown, /* Unknown registration status. */
        StatusRoaming /* Registered on a roaming network. */
    };

    class RegistrationInfoType
    {
    public:
        RegistrationStatus status; /* Mobile registration status as defined in 3GPP TS 27.007 section 10.1.19. */
        QString operatorCode, /* Current operator code of the operator to which the mobile is currently registered.
                                 Returned in the format "MCCMNC", where MCC is the three-digit ITU E.212 Mobile Country Code
                                 and MNC is the two- or three-digit GSM Mobile Network Code. If the MCC and MNC are not known
                                 or the mobile is not registered to a mobile network, this value should be a zero-length (blank)
                                 string. e.g. "31026" or "310260". */
        operatorName /* Current operator name of the operator to which the mobile is currently registered.
                                If the operator name is not knowon or the mobile is not registered to a mobile network,
                                this value should be a zero-length (blank) string. */;
    };


    ModemGsmNetworkInterface(const QString & path, QObject * parent);
    ~ModemGsmNetworkInterface();

    /**
     * Register the device to network.
     *
     * @param networkId the network ID to register. An empty string can be used to register to the home network.
     */
    void registerToNetwork(const QString & networkId);
    /*
     * Each item list may include one or more of the following keys:
     *
     * "status": a number representing network availability status as defined in 3GPP TS 27.007
     *           section 7.3. e.g. "0" (unknown), "1" (available), "2" (current), or "3" (forbidden).
     *           This key will always be present.
     * "operator-long": long-format name of operator. If the name is unknown, this field should not be present.
     * "operator-short": short-format name of operator. If the name is unknown, this field should not be present.
     * "operator-num": mobile code of the operator. Returned in the format "MCCMNC",
     *                 where MCC is the three-digit ITU E.212 Mobile Country Code and MNC is the two- or
     *                 three-digit GSM Mobile Network Code. e.g. "31026" or "310260".
     * "access-tech": a number representing the access technology used by this mobile network as described
     *                in 3GPP TS 27.007 section 7.3. e.g. "0" (GSM), "1" (GSM Compact), "2" (UTRAN/UMTS), "3" (EDGE), etc.
     */
    ScanResultsType scan();
    /**
     * Sets the Access Point Name (APN).
     *
     * @param apn the APN to set to.
     */
    void setApn(const QString & apn);
    /**
     * Retrieves the current signal quality of the gsm connection.
     *
     * @return the signal quality as a percentage
     */
    uint getSignalQuality();
    /**
     * Sets the band the device is allowed to use when connecting to a mobile network.
     *
     * @param band the desired band. Only one band may be specified, and may not be UNKNOWN.
     */
    void setBand(const ModemManager::ModemInterface::Band band);
    ModemManager::ModemInterface::Band getBand();
    RegistrationInfoType getRegistrationInfo();
    void setAllowedMode(const ModemManager::ModemInterface::AllowedMode mode);
    // properties
    ModemManager::ModemInterface::AllowedMode getAllowedMode() const;
    ModemManager::ModemInterface::AccessTechnology getAccessTechnology() const;
public Q_SLOTS:
    void slotRegistrationInfoChanged(uint status, const QString & operatorCode, const QString &operatorName);
    void propertiesChanged(const QString & interface, const QVariantMap & properties);
Q_SIGNALS:
    void signalQualityChanged(uint signalQuality);
    void registrationInfoChanged(const ModemManager::ModemGsmNetworkInterface::RegistrationInfoType &registrationInfo);
    // properties
    void allowedModeChanged(const ModemManager::ModemInterface::AllowedMode mode);
    void accessTechnologyChanged(const ModemManager::ModemInterface::AccessTechnology tech);
};

} // namespace ModemManager

#endif

