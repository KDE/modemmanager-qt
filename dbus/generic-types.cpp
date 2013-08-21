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

// Marshall the ModemManager::Ip4ConfigType data into a D-BUS argument
QDBusArgument &operator << (QDBusArgument &arg,
    const ModemManager::ModemInterface::Ip4ConfigType &config)
{
    arg.beginStructure();
    arg << config.ip4Address << config.dns1 << config.dns2 << config.dns3;
    arg.endStructure();
    return arg;
}

// Retrieve the ModemManager::Ip4ConfigType data from the D-BUS argument
const QDBusArgument &operator >> (const QDBusArgument &arg,
    ModemManager::ModemInterface::Ip4ConfigType &config)
{
    arg.beginStructure();
    arg >> config.ip4Address >> config.dns1 >> config.dns2 >> config.dns3;
    arg.endStructure();

    return arg;
}

// Marshall the ModemManager::ModemManager::Modem::InfoType data into a D-BUS argument
QDBusArgument &operator << (QDBusArgument &arg,
    const ModemManager::ModemInterface::InfoType &info)
{
    arg.beginStructure();
    arg << info.manufacturer << info.model << info.version;
    arg.endStructure();
    return arg;
}

// Retrieve the ModemManager::ModemManager::Modem::InfoType data from the D-BUS argument
const QDBusArgument &operator >> (const QDBusArgument &arg,
    ModemManager::ModemInterface::InfoType &info)
{
    arg.beginStructure();
    arg >> info.manufacturer >> info.model >> info.version;
    arg.endStructure();
    return arg;
}

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

// Marshall the ModemManager::ModemGsmContactsInterface::ContactType data into a D-BUS argument
QDBusArgument &operator << (QDBusArgument &arg,
    const ModemManager::ModemGsmContactsInterface::ContactType &contact)
{
    arg.beginStructure();
    arg << contact.index << contact.name << contact.number;
    arg.endStructure();
    return arg;
}

// Retrieve the ModemManager::ModemGsmContactsInterface::ContactType data from the D-BUS argument
const QDBusArgument &operator >> (const QDBusArgument &arg,
    ModemManager::ModemGsmContactsInterface::ContactType &contact)
{
    arg.beginStructure();
    arg >> contact.index >> contact.name >> contact.number;
    arg.endStructure();
    return arg;
}

// Marshall the RegistrationInfoType data into a D-BUS argument
QDBusArgument &operator << (QDBusArgument &arg,
    const ModemManager::ModemGsmNetworkInterface::RegistrationInfoType &info)
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
    ModemManager::ModemGsmNetworkInterface::RegistrationInfoType &info)
{
    uint temp;

    arg.beginStructure();
    arg >> temp >> info.operatorCode >> info.operatorName;
    info.status = (ModemManager::ModemGsmNetworkInterface::RegistrationStatus) temp;
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
        QVariantMapList element;
        argument >> element;
        variantMapList.append( element );
    }

    argument.endArray();
    return argument;
}
