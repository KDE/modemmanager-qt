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

#ifndef MODEMMANAGER_GENERIC_TYPES_H
#define MODEMMANAGER_GENERIC_TYPES_H

#include <ModemManager/ModemManager.h>

#include <QMetaType>
#include <QtDBus/QtDBus>

typedef QMap<QString,QVariantMap> QVariantMapMap;
Q_DECLARE_METATYPE(QVariantMapMap)

typedef QMap<QDBusObjectPath, QVariantMapMap> DBUSManagerStruct;
Q_DECLARE_METATYPE(DBUSManagerStruct)

typedef QList<uint> UIntList;
Q_DECLARE_METATYPE(UIntList)

typedef struct {
    uint signal;
    bool recent;
} SignalQualityPair;
Q_DECLARE_METATYPE(SignalQualityPair)

Q_DECLARE_BUILTIN_METATYPE(MMModemMode, UInt)
typedef struct {
public:
    MMModemMode allowed; // bitfield
    MMModemMode prefered;
} CurrentModesType;
Q_DECLARE_METATYPE(CurrentModesType)

typedef QList<CurrentModesType> SupportedModesType;
Q_DECLARE_METATYPE(SupportedModesType)

Q_DECLARE_BUILTIN_METATYPE(MMModemLock, UInt)
typedef QMap</*MMModemLock*/uint, uint> UnlockRetriesMap;
Q_DECLARE_METATYPE(UnlockRetriesMap)

typedef QList<QVariantMap> ScanResultsType;
Q_DECLARE_METATYPE(ScanResultsType)

Q_DECLARE_BUILTIN_METATYPE(MMModemLocationSource, UInt)
typedef QMap<MMModemLocationSource, QVariant> LocationInformationMap;
Q_DECLARE_METATYPE(LocationInformationMap)

Q_DECLARE_BUILTIN_METATYPE(MMSmsValidityType, UInt)
typedef struct {
    MMSmsValidityType validity;
    uint value;
} ValidityPair;
Q_DECLARE_METATYPE(ValidityPair)

// CurrentModesType
QDBusArgument &operator << (QDBusArgument &arg, const CurrentModesType &mode);
const QDBusArgument &operator >> (const QDBusArgument &arg, CurrentModesType &mode);

// SignalQualityPair
QDBusArgument &operator << (QDBusArgument &arg, const SignalQualityPair &sqp);
const QDBusArgument &operator >> (const QDBusArgument &arg, SignalQualityPair &sqp);

// ValidityPair
QDBusArgument &operator << (QDBusArgument &arg, const ValidityPair &vp);
const QDBusArgument &operator >> (const QDBusArgument &arg, ValidityPair &vp);

inline void registerModemManagerTypes() {
    qDBusRegisterMetaType<CurrentModesType>();
    qDBusRegisterMetaType<SignalQualityPair>();
    qDBusRegisterMetaType<SupportedModesType>();
    qDBusRegisterMetaType<UnlockRetriesMap>();
    qDBusRegisterMetaType<ScanResultsType>();
    //qDBusRegisterMetaType<LocationInformationMap>();
    qDBusRegisterMetaType<ValidityPair>();
}

#endif // MODEMMANAGER_TYPES_H
