/* This file is part of the KDE project
   Copyright 2010 Lamarque Souza <lamarque@kde.org>

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

#if 0
// Marshall the ModemManager::ModemCdmaInterface::ServingSystemType data into a D-BUS argument
QDBusArgument &operator << (QDBusArgument &arg,
    const ModemManager::ModemCdmaInterface::ServingSystemType &servingSystem)
{
    arg.beginStructure();
    arg << servingSystem.bandClass << servingSystem.band << servingSystem.systemId;
    arg.endStructure();
    return arg;
}

// Retrieve the ModemManager::ModemCdmaInterface::ServingSystemType data from the D-BUS argument
const QDBusArgument &operator >> (const QDBusArgument &arg,
    ModemManager::ModemCdmaInterface::ServingSystemType &servingSystem)
{
    uint temp;
    arg.beginStructure();
    arg >> temp >> servingSystem.band >> servingSystem.systemId;
    servingSystem.bandClass = (ModemManager::ModemCdmaInterface::BandClass) temp;
    arg.endStructure();
    return arg;
}

// Marshall the RegistrationInfoType data into a D-BUS argument
QDBusArgument &operator << (QDBusArgument &arg,
    const ModemManager::Modem3gppInterface::RegistrationInfoType &info)
{
    uint temp;

    temp = (uint) info.status;
    arg.beginStructure();
    arg << temp << info.operatorCode << info.operatorName;
    arg.endStructure();
    return arg;
}

// Retrieve the RegistrationInfoType data from the D-BUS argument
const QDBusArgument &operator >> (const QDBusArgument &arg,
    ModemManager::Modem3gppInterface::RegistrationInfoType &info)
{
    uint temp;

    arg.beginStructure();
    arg >> temp >> info.operatorCode >> info.operatorName;
    info.status = (ModemManager::Modem3gppInterface::RegistrationStatus) temp;
    arg.endStructure();
    return arg;
}
#endif
