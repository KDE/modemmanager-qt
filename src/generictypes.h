/* This file is part of the KDE project
   Copyright 2010 Lamarque Souza <lamarque@kde.org>
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

#ifndef MODEMMANAGERQT_GENERIC_TYPES_H
#define MODEMMANAGERQT_GENERIC_TYPES_H

#include <modemmanagerqt_export.h>
#include <ModemManager/ModemManager.h>

#include <QMetaType>
#include <QtDBus/QtDBus>

typedef QMap<QString, QVariantMap> NMVariantMapMap;
typedef QMap<QDBusObjectPath, NMVariantMapMap> DBUSManagerStruct;
typedef QList<uint> UIntList;
typedef QList<QList<uint> > UIntListList;
typedef QMap<QString, QString> NMStringMap;
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
public:
    MMModemMode allowed; // bitfield
    MMModemMode preferred;
} CurrentModesType;

typedef QList<CurrentModesType> SupportedModesType;
#if MM_CHECK_VERSION(1, 2, 0)
typedef struct {
public:
    MMOmaSessionType type;
    uint id;
} OmaSessionType;
typedef QList<OmaSessionType> OmaSessionTypes;
#endif
typedef QMap</*MMModemLock*/uint, uint> UnlockRetriesMap;
typedef QList<QVariantMap> QVariantMapList;
typedef QMap<MMModemLocationSource, QVariant> LocationInformationMap;
typedef struct {
    MMSmsValidityType validity;
    uint value;
} ValidityPair;

// Port
MODEMMANAGERQT_EXPORT QDBusArgument &operator << (QDBusArgument &arg, const Port &port);
MODEMMANAGERQT_EXPORT const QDBusArgument &operator >> (const QDBusArgument &arg, Port &port);

// CurrentModesType
MODEMMANAGERQT_EXPORT QDBusArgument &operator << (QDBusArgument &arg, const CurrentModesType &mode);
MODEMMANAGERQT_EXPORT const QDBusArgument &operator >> (const QDBusArgument &arg, CurrentModesType &mode);

// SignalQualityPair
MODEMMANAGERQT_EXPORT QDBusArgument &operator << (QDBusArgument &arg, const SignalQualityPair &sqp);
MODEMMANAGERQT_EXPORT const QDBusArgument &operator >> (const QDBusArgument &arg, SignalQualityPair &sqp);

// ValidityPair
MODEMMANAGERQT_EXPORT QDBusArgument &operator << (QDBusArgument &arg, const ValidityPair &vp);
MODEMMANAGERQT_EXPORT const QDBusArgument &operator >> (const QDBusArgument &arg, ValidityPair &vp);

#if MM_CHECK_VERSION(1, 2, 0)
// OmaSessionType
MODEMMANAGERQT_EXPORT QDBusArgument &operator << (QDBusArgument &arg, const OmaSessionType &sessionType);
MODEMMANAGERQT_EXPORT const QDBusArgument &operator >> (const QDBusArgument &arg, OmaSessionType &sessionType);
#endif

// QVariantMapList
MODEMMANAGERQT_EXPORT QDBusArgument &operator<<(QDBusArgument &argument, const QVariantMapList &variantMapList);
MODEMMANAGERQT_EXPORT const QDBusArgument &operator>>(const QDBusArgument &argument, QVariantMapList &variantMapList);

MODEMMANAGERQT_EXPORT void registerModemManagerTypes();

#endif // MODEMMANAGERQT_GENERIC_TYPES_P_H
