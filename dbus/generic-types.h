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

#include <ModemManager/ModemManager.h>

#include <QMetaType>
#include <QtDBus/QtDBus>

Q_DECLARE_METATYPE(MMModemLock)

typedef QMap<QString, QVariantMap> NMVariantMapMap;
Q_DECLARE_METATYPE(NMVariantMapMap)

typedef QMap<QDBusObjectPath, NMVariantMapMap> DBUSManagerStruct;
Q_DECLARE_METATYPE(DBUSManagerStruct)

typedef QList<uint> UIntList;
Q_DECLARE_METATYPE(UIntList)

typedef QList<QList<uint> > UIntListList;
Q_DECLARE_METATYPE(UIntListList)

typedef QMap<QString, QString> NMStringMap;
Q_DECLARE_METATYPE(NMStringMap)

typedef struct {
    QString name;
    MMModemPortType type;
} Port;
Q_DECLARE_METATYPE(Port)

typedef QList<Port> PortList;
Q_DECLARE_METATYPE(PortList)

typedef struct {
    uint signal;
    bool recent;
} SignalQualityPair;
Q_DECLARE_METATYPE(SignalQualityPair)

Q_DECLARE_METATYPE(MMModemMode)

typedef struct {
public:
    MMModemMode allowed; // bitfield
    MMModemMode preferred;
} CurrentModesType;
Q_DECLARE_METATYPE(CurrentModesType)

typedef QList<CurrentModesType> SupportedModesType;
Q_DECLARE_METATYPE(SupportedModesType)

typedef struct {
public:
    MMOmaSessionType type;
    uint id;
} OmaSessionType;
Q_DECLARE_METATYPE(OmaSessionType)

typedef QList<OmaSessionType> OmaSessionTypes;
Q_DECLARE_METATYPE(OmaSessionTypes)

//Q_DECLARE_METATYPE(MMModemLock)

typedef QMap</*MMModemLock*/uint, uint> UnlockRetriesMap;
Q_DECLARE_METATYPE(UnlockRetriesMap)

typedef QList<QVariantMap> QVariantMapList;
Q_DECLARE_METATYPE(QVariantMapList)

typedef QMap<MMModemLocationSource, QVariant> LocationInformationMap;
Q_DECLARE_METATYPE(LocationInformationMap)

typedef struct {
    MMSmsValidityType validity;
    uint value;
} ValidityPair;
Q_DECLARE_METATYPE(ValidityPair)

// Port
QDBusArgument &operator << (QDBusArgument &arg, const Port &port);
const QDBusArgument &operator >> (const QDBusArgument &arg, Port &port);

// CurrentModesType
QDBusArgument &operator << (QDBusArgument &arg, const CurrentModesType &mode);
const QDBusArgument &operator >> (const QDBusArgument &arg, CurrentModesType &mode);

// SignalQualityPair
QDBusArgument &operator << (QDBusArgument &arg, const SignalQualityPair &sqp);
const QDBusArgument &operator >> (const QDBusArgument &arg, SignalQualityPair &sqp);

// ValidityPair
QDBusArgument &operator << (QDBusArgument &arg, const ValidityPair &vp);
const QDBusArgument &operator >> (const QDBusArgument &arg, ValidityPair &vp);

// OmaSessionType
QDBusArgument &operator << (QDBusArgument &arg, const OmaSessionType &sessionType);
const QDBusArgument &operator >> (const QDBusArgument &arg, OmaSessionType &sessionType);

// QVariantMapList
QDBusArgument &operator<<(QDBusArgument &argument, const QVariantMapList &variantMapList);
const QDBusArgument &operator>>(const QDBusArgument &argument, QVariantMapList &variantMapList);

void registerModemManagerTypes();

#endif // MODEMMANAGERQT_TYPES_H
