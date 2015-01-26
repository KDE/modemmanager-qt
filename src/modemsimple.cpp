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

#include "modemsimple_p.h"

#include "dbus/dbus.h"
#include "generictypes.h"
#include "mmdebug.h"

ModemManager::ModemSimplePrivate::ModemSimplePrivate(const QString &path, ModemSimple *q)
    : InterfacePrivate(path, q)
    , modemSimpleIface(MM_DBUS_SERVICE, path, QDBusConnection::systemBus())
    , q_ptr(q)
{
}

ModemManager::ModemSimple::ModemSimple(const QString &path, QObject *parent)
    : Interface(*new ModemSimplePrivate(path, this), parent)
{
}

ModemManager::ModemSimple::~ModemSimple()
{
}

QString ModemManager::ModemSimple::uni() const
{
    Q_D(const ModemSimple);
    return d->uni;
}

QDBusPendingReply<QDBusObjectPath> ModemManager::ModemSimple::connectModem(ConnectPropertiesStruct properties)
{
    Q_D(ModemSimple);

    QVariantMap map;
    map.insert(QLatin1String("pin"), properties.pin);
    map.insert(QLatin1String("operator-id"), properties.operatorId);
    map.insert(QLatin1String("apn"), properties.apn);
    map.insert(QLatin1String("ip-type"), properties.ipType);
    map.insert(QLatin1String("allowed-auth"), properties.allowedAuth);
    map.insert(QLatin1String("user"), properties.user);
    map.insert(QLatin1String("password"), properties.password);
    map.insert(QLatin1String("number"), properties.number);
    map.insert(QLatin1String("allow-roaming"), properties.allowRoaming);
    map.insert(QLatin1String("rm-protocol"), properties.rmProtocol);

    return d->modemSimpleIface.Connect(map);
}

QDBusPendingReply<QVariantMap> ModemManager::ModemSimple::getStatus()
{
    Q_D(ModemSimple);
    return d->modemSimpleIface.GetStatus();
}

QDBusPendingReply<void> ModemManager::ModemSimple::disconnectModem(const QString &bearer)
{
    Q_D(ModemSimple);
    return d->modemSimpleIface.Disconnect(QDBusObjectPath(bearer));
}

QDBusPendingReply<void> ModemManager::ModemSimple::disconnectAllModems()
{
    return disconnectModem(QStringLiteral("/"));
}

ModemManager::ModemSimple::ModemStatusStruct ModemManager::ModemSimple::mapToModemStatusStruct(const QVariantMap& map)
{
    ModemStatusStruct modemStatus;

    if (map.contains("state")) {
        modemStatus.state = (MMModemState)map.value("state").toUInt();
    }
    if (map.contains("signal-quality")) {
        modemStatus.signalQuality = map.value("signal-quality").toUInt();
    }
    if (map.contains("current-bands")) {
        modemStatus.currentBands = (MMModemBand)map.value("current-bands").toUInt();
    }
    if (map.contains("access-technology")) {
        modemStatus.accessTechnology = (MMModemAccessTechnology)map.value("access-technology").toUInt();
    }
    if (map.contains("m3gpp-registration-state")) {
        modemStatus.m3gppRegistrationState = (MMModem3gppRegistrationState)map.value("m3gpp-registration-state").toUInt();
    }
    if (map.contains("m3gpp-operator-code")) {
        modemStatus.m3gppOperatorCode = map.value("m3gpp-operator-code").toString();
    }
    if (map.contains("m3gpp-operator-name")) {
        modemStatus.m3gppOperatorName = map.value("m3gpp-operator-name").toString();
    }
    if (map.contains("cdma-cdma1x-registration-state")) {
        modemStatus.cdma1xRegistrationState = (MMModemCdmaRegistrationState)map.value("cdma-cdma1x-registration-state").toUInt();
    }
    if (map.contains("cdma-evdo-registration-state")) {
        modemStatus.evdoRegistrationState = (MMModemCdmaRegistrationState)map.value("cdma-evdo-registration-state").toUInt();
    }
    if (map.contains("cdma-sid")) {
        modemStatus.cdmaSid = map.value("cdma-sid").toUInt();
    }
    if (map.contains("cdma-nid")) {
        modemStatus.cdmaNid = map.value("cdma-nid").toUInt();
    }

    return modemStatus;
}
