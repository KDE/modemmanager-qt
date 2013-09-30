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

#include "generic-types.h"


// Marshall the CurrentModesType data into a D-BUS argument
QDBusArgument &operator << (QDBusArgument &arg, const CurrentModesType &mode)
{
    arg.beginStructure();
    arg << mode.allowed << mode.prefered;
    arg.endStructure();
    return arg;
}

// Retrieve the CurrentModesType data from the D-BUS argument
const QDBusArgument &operator >> (const QDBusArgument &arg, CurrentModesType &mode)
{
    uint temp1, temp2;
    arg.beginStructure();
    arg >> temp1 >> temp2;
    mode.allowed = (MMModemMode)temp1;
    mode.prefered = (MMModemMode)temp2;
    arg.endStructure();

    return arg;
}


// Marshall the SignalQualityPair data into a D-BUS argument
QDBusArgument &operator << (QDBusArgument &arg, const SignalQualityPair &sqp)
{
    arg.beginStructure();
    arg << sqp.signal << sqp.recent;
    arg.endStructure();
    return arg;
}

// Retrieve the SignalQualityPair data from the D-BUS argument
const QDBusArgument &operator >> (const QDBusArgument &arg, SignalQualityPair &sqp)
{
    arg.beginStructure();
    arg >> sqp.signal >> sqp.recent;
    arg.endStructure();
    return arg;
}

// Marshall the ValidityPair data into a D-BUS argument
QDBusArgument &operator <<(QDBusArgument &arg, const ValidityPair &vp)
{
    arg.beginStructure();
    arg << vp.validity << vp.value;
    arg.endStructure();
    return arg;
}

// Retrieve the ValidityPair data from the D-BUS argument
const QDBusArgument &operator >>(const QDBusArgument &arg, ValidityPair &vp)
{
    uint temp1, temp2;
    arg.beginStructure();
    arg >> temp1 >> temp2;
    vp.validity = (MMSmsValidityType)temp1;
    vp.value = temp2;
    arg.endStructure();

    return arg;
}

// Marshal QList<QVariantMap> into a D-BUS argument
QDBusArgument &operator<<(QDBusArgument &argument, const QList<QVariantMap> &variantMapList)
{
    argument.beginArray(qMetaTypeId<QVariantMap>());
    for (int i = 0; i < variantMapList.length(); ++i)
        argument << variantMapList[i];
    argument.endArray();
    return argument;
}

// Retrieve QList<QVariantMap> from a D-BUS argument
const QDBusArgument &operator>>(const QDBusArgument &argument, QList<QVariantMap> &variantMapList)
{
    argument.beginArray();
    variantMapList.clear();

    while (!argument.atEnd()) {
        QList<QVariantMap> element;
        argument >> element;
        variantMapList.append( element );
    }

    argument.endArray();
    return argument;
}
