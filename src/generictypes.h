/*
    Copyright 2010 Lamarque Souza <lamarque@kde.org>
    Copyright 2013 Lukas Tinkl <ltinkl@redhat.com>
    Copyright 2013-2015 Jan Grulich <jgrulich@redhat.com>

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

#ifndef MODEMMANAGERQT_GENERIC_TYPES_H
#define MODEMMANAGERQT_GENERIC_TYPES_H

#include <modemmanagerqt_export.h>
#include <ModemManager/ModemManager.h>

#include <QMetaType>
#include <QtDBus/QtDBus>

namespace ModemManager
{
    typedef QMap<QString, QVariantMap> MMVariantMapMap;
    typedef QMap<QDBusObjectPath, MMVariantMapMap> DBUSManagerStruct;
    typedef QList<uint> UIntList;
    typedef QList<QList<uint> > UIntListList;

    typedef struct {
        QString name;
        MMModemPortType type;
    } Port;
    typedef QList<Port> PortList;

    typedef struct {
        uint signal;
        bool recent;
    } SignalQualityPair;

    typedef struct {
        uint allowed; // bitfield
        MMModemMode preferred;
    } CurrentModesType;
    typedef QList<CurrentModesType> SupportedModesType;

#if MM_CHECK_VERSION(1, 2, 0)
    typedef struct {
        MMOmaSessionType type;
        uint id;
    } OmaSessionType;
    typedef QList<OmaSessionType> OmaSessionTypes;
#endif
    typedef QMap<MMModemLock, uint> UnlockRetriesMap;
    typedef QList<QVariantMap> QVariantMapList;
    typedef QMap<MMModemLocationSource, QVariant> LocationInformationMap;
    typedef struct {
        MMSmsValidityType validity;
        uint value;
    } ValidityPair;
}

Q_DECLARE_METATYPE(QFlags<MMModemAccessTechnology>)
Q_DECLARE_METATYPE(QFlags<MMModemCapability>)
Q_DECLARE_METATYPE(QFlags<MMModemMode>)
Q_DECLARE_METATYPE(QFlags<MMBearerIpFamily>)

Q_DECLARE_METATYPE(QList<MMModemBand>)
Q_DECLARE_METATYPE(QList<MMModemCapability>)
Q_DECLARE_METATYPE(MMModemLock)
Q_DECLARE_METATYPE(MMModemPowerState)
Q_DECLARE_METATYPE(MMModemStateFailedReason)

Q_DECLARE_METATYPE(ModemManager::DBUSManagerStruct)
Q_DECLARE_METATYPE(ModemManager::Port)
Q_DECLARE_METATYPE(ModemManager::PortList)
Q_DECLARE_METATYPE(ModemManager::SignalQualityPair)
Q_DECLARE_METATYPE(ModemManager::CurrentModesType)
Q_DECLARE_METATYPE(ModemManager::SupportedModesType)
#if MM_CHECK_VERSION(1, 2, 0)
Q_DECLARE_METATYPE(ModemManager::OmaSessionType)
Q_DECLARE_METATYPE(ModemManager::OmaSessionTypes)
#endif
Q_DECLARE_METATYPE(ModemManager::UnlockRetriesMap)
Q_DECLARE_METATYPE(ModemManager::QVariantMapList)
Q_DECLARE_METATYPE(ModemManager::LocationInformationMap)
Q_DECLARE_METATYPE(ModemManager::ValidityPair)

// Port
MODEMMANAGERQT_EXPORT QDBusArgument &operator << (QDBusArgument &arg, const ModemManager::Port &port);
MODEMMANAGERQT_EXPORT const QDBusArgument &operator >> (const QDBusArgument &arg, ModemManager::Port &port);

// CurrentModesType
MODEMMANAGERQT_EXPORT QDBusArgument &operator << (QDBusArgument &arg, const ModemManager::CurrentModesType &mode);
MODEMMANAGERQT_EXPORT const QDBusArgument &operator >> (const QDBusArgument &arg, ModemManager::CurrentModesType &mode);

// SignalQualityPair
MODEMMANAGERQT_EXPORT QDBusArgument &operator << (QDBusArgument &arg, const ModemManager::SignalQualityPair &sqp);
MODEMMANAGERQT_EXPORT const QDBusArgument &operator >> (const QDBusArgument &arg, ModemManager::SignalQualityPair &sqp);

// ValidityPair
MODEMMANAGERQT_EXPORT QDBusArgument &operator << (QDBusArgument &arg, const ModemManager::ValidityPair &vp);
MODEMMANAGERQT_EXPORT const QDBusArgument &operator >> (const QDBusArgument &arg, ModemManager::ValidityPair &vp);

#if MM_CHECK_VERSION(1, 2, 0)
// OmaSessionType
MODEMMANAGERQT_EXPORT QDBusArgument &operator << (QDBusArgument &arg, const ModemManager::OmaSessionType &sessionType);
MODEMMANAGERQT_EXPORT const QDBusArgument &operator >> (const QDBusArgument &arg, ModemManager::OmaSessionType &sessionType);
#endif

// UnlockRetriesMap
MODEMMANAGERQT_EXPORT QDBusArgument &operator<<(QDBusArgument &argument, const ModemManager::UnlockRetriesMap &unlockRetriesMap);
MODEMMANAGERQT_EXPORT const QDBusArgument &operator>>(const QDBusArgument &argument, ModemManager::UnlockRetriesMap &unlockRetriesMap);

// QVariantMapList
MODEMMANAGERQT_EXPORT QDBusArgument &operator<<(QDBusArgument &argument, const ModemManager::QVariantMapList &variantMapList);
MODEMMANAGERQT_EXPORT const QDBusArgument &operator>>(const QDBusArgument &argument, ModemManager::QVariantMapList &variantMapList);

// LocationInformationMap
MODEMMANAGERQT_EXPORT QDBusArgument &operator<<(QDBusArgument &argument, const ModemManager::LocationInformationMap &locationMap);
MODEMMANAGERQT_EXPORT const QDBusArgument &operator>>(const QDBusArgument &argument, ModemManager::LocationInformationMap &locationMap);

MODEMMANAGERQT_EXPORT void registerModemManagerTypes();

#endif // MODEMMANAGERQT_GENERIC_TYPES_P_H
