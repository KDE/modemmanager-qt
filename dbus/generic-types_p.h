/* This file is part of the KDE project
   Copyright 2010 Lamarque Souza <lamarque@kde.org>
   Copyright 2013 Lukas Tinkl <ltinkl@redhat.com>
   Copyright 2014 Lukas Tinkl <jgrulich@redhat.com>

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

#ifndef MODEMMANAGERQT_GENERIC_TYPES_P_H
#define MODEMMANAGERQT_GENERIC_TYPES_P_H

#include <QMetaType>

Q_DECLARE_METATYPE(MMModemLock)
Q_DECLARE_METATYPE(NMVariantMapMap)
Q_DECLARE_METATYPE(DBUSManagerStruct)
Q_DECLARE_METATYPE(UIntList)
Q_DECLARE_METATYPE(UIntListList)
Q_DECLARE_METATYPE(NMStringMap)
Q_DECLARE_METATYPE(Port)
Q_DECLARE_METATYPE(PortList)
Q_DECLARE_METATYPE(SignalQualityPair)
Q_DECLARE_METATYPE(MMModemMode)
Q_DECLARE_METATYPE(CurrentModesType)
Q_DECLARE_METATYPE(SupportedModesType)
Q_DECLARE_METATYPE(OmaSessionType)
Q_DECLARE_METATYPE(OmaSessionTypes)
//Q_DECLARE_METATYPE(MMModemLock)
Q_DECLARE_METATYPE(UnlockRetriesMap)
Q_DECLARE_METATYPE(QVariantMapList)
Q_DECLARE_METATYPE(LocationInformationMap)
Q_DECLARE_METATYPE(ValidityPair)

#endif // MODEMMANAGERQT_GENERIC_TYPES_P_H
