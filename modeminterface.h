/*
Copyright 2008,2011 Will Stephenson <wstephenson@kde.org>
Copyright 2010 Lamarque Souza <lamarque@kde.org>

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License as
published by the Free Software Foundation; either version 2 of
the License or (at your option) version 3 or any later version
accepted by the membership of KDE e.V. (or its successor approved
by the membership of KDE e.V.), which shall act as a prox
defined in Section 14 of version 3 of the license.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef MODEMMANAGER_MODEMINTERFACE_H
#define MODEMMANAGER_MODEMINTERFACE_H

#include "mmqt-export.h"

#include <QObject>
#include <QVariant>

class ModemInterfacePrivate;

namespace ModemManager
{
class MMQT_EXPORT ModemInterface : public QObject
{
Q_OBJECT
Q_DECLARE_PRIVATE(ModemInterface)
Q_PROPERTY(QString udi READ udi)

public:
    enum Type {
        UnknownType = 0, /* MM specification does not include this type though. */
        GsmType = 1,
        CdmaType = 2
    };

    enum Method {
        Ppp = 0, /* Use PPP to get the address. */
        Static = 1, /* Static configuration, the modem will provide IP information. */
        Dhcp = 2, /*Use DHCP */
        UnknownMethod = 0xff
    };

    /** Describes the device's current access mode preference;
     * ie the specific technology preferences the device is allowed to use
     * when connecting to a mobile network. */
    enum AllowedMode {
         AnyModeAllowed = 0x0, /* Any mode can be used */
         Prefer2g, /* Prefer 2G (GPRS or EDGE) */
         Prefer3g, /* Prefer 3G (UMTS or HSxPA) */
         UseOnly2g, /* Use only 2G (GPRS or EDGE) */
         UseOnly3g /* Use only 3G (UMTS or HSxPA) */
    };

    /** Describes various access technologies that a device uses when connected to a mobile network. */
    enum AccessTechnology {
        UnknownTechnology = 0x0, /* The access technology used is unknown */
        Gsm, /* GSM */
        GsmCompact, /* Compact GSM */
        Gprs, /* GPRS */
        Edge, /* EDGE (ETSI 27.007: "GSM w/EGPRS") */
        Umts, /* UMTS (ETSI 27.007: "UTRAN") */
        Hsdpa, /* HSDPA (ETSI 27.007: "UTRAN w/HSDPA") */
        Hsupa, /* HSUPA (ETSI 27.007: "UTRAN w/HSUPA") */
        Hspa /* HSPA (ETSI 27.007: "UTRAN w/HSDPA and HSUPA") */
    };

    /** A bitfield describing the specifc access modes and technologies supported by a device
     * and the access technology in-use when connected to a mobile network. */
    enum Mode {
        UnknownMode = 0x0, /* Unknown or invalid mode. */
        AnyMode = 0x1, /* For certain operations, allow the modem to pick any available mode. */
        GprsMode = 0x2, /* GPRS */
        EdgeMode = 0x4, /* EDGE */
        UmtsMode = 0x8, /* UMTS (3G) */
        HsdpaMode = 0x10, /* HSDPA (3G) */
        Prefer2gMode = 0x20, /* Prefer 2G (GPRS or EDGE) */
        Prefer3gMode = 0x40, /* Prefer 3G (UMTS/HSDPA/HSUPA/HSPA) */
        UseOnly2gMode = 0x80, /* Use only 2G (GPRS or EDGE) */
        UseOnly3gMode = 0x100, /* Use only 3G (UMTS/HSDPA/HSUPA/HSPA) */
        HsupaMode = 0x200, /* HSUPA (3G) */
        HspaMode = 0x400, /* HSPA (3G) */
        GsmMode = 0x800, /* GSM */
        GsmCompactMode = 0x1000 /* GSM Compact */
    };

    /** A bitfield describing the specific radio bands supported by the device
     * and the radio bands the device is allowed to use when connecting to a mobile network. */
    enum Band {
        UnknownBand = 0x0, /* Unknown or invalid band */
        AnyBand = 0x1, /* For certain operations allow the modem to select a band automatically. */
        Egsm = 0x2, /* GSM/GPRS/EDGE 900 MHz */
        Dcs = 0x4, /* GSM/GPRS/EDGE 1800 MHz */
        Pcs = 0x8, /* GSM/GPRS/EDGE 1900 MHz */
        G850 = 0x10, /* GSM/GPRS/EDGE 850 MHz */
        U2100 = 0x20, /* WCDMA 2100 MHz (Class I) */
        U1800 = 0x40, /* WCDMA 3GPP 1800 MHz (Class III) */
        U17IV = 0x80, /* WCDMA 3GPP AWS 1700/2100 MHz (Class IV) */
        U800 = 0x100, /* WCDMA 3GPP UMTS 800 MHz (Class VI) */
        U850 = 0x200, /* WCDMA 3GPP UMTS 850 MHz (Class V) */
        U900 = 0x400, /* WCDMA 3GPP UMTS 900 MHz (Class VIII) */
        U17IX = 0x800, /* WCDMA 3GPP UMTS 1700 MHz (Class IX) */
        U19IX = 0x1000 /* WCDMA 3GPP UMTS 1900 MHz (Class II) */
    };

    enum GsmInterfaceType {
        NotGsm,
        GsmCard,
        GsmContacts,
        GsmNetwork,
        GsmSms,
        GsmHso,
        GsmUssd
    };

    class Ip4ConfigType
    {
    public:
        QString ip4Address,
                dns1,
                dns2,
                dns3;
    };

    class InfoType
    {
    public:
        QString manufacturer,
                model,
                version;
    };

    ModemInterface( const QString & path, QObject * parent = 0 );
    ModemInterface( ModemInterfacePrivate &dd, QObject * parent = 0);
    virtual ~ModemInterface();

    QString udi() const;

    // From org.freedesktop.ModemManager.Modem
    void enable(const bool enable);
    void connectModem(const QString & number);
    void disconnectModem();
    ModemManager::ModemInterface::Ip4ConfigType getIp4Config();
    ModemManager::ModemInterface::InfoType getInfo();

    QString device() const;
    QString masterDevice() const;
    QString driver() const;
    ModemManager::ModemInterface::Type type() const;
    bool enabled() const;
    QString unlockRequired() const;
    ModemManager::ModemInterface::Method ipMethod() const;

    // From org.freedesktop.ModemManager.Modem.Simple
    void connectModem(const QVariantMap & properties);
    QVariantMap getStatus();
public Q_SLOTS:
    void propertiesChanged(const QString & interface, const QVariantMap & properties);

Q_SIGNALS:
    void deviceChanged(const QString & device);
    void masterDeviceChanged(const QString & masterDevice);
    void driverChanged(const QString & driver);
    void typeChanged(const ModemManager::ModemInterface::Type type);
    void enabledChanged(const bool enabled);
    void unlockRequiredChanged(const QString & codeRequired);
    void ipMethodChanged(const ModemManager::ModemInterface::Method ipMethod);

private:
    void init();

protected:
    ModemInterfacePrivate * d_ptr;
};

} // namespace ModemManager

#endif

