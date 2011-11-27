/* This file is part of the KDE project
   Copyright 2010 Lamarque Souza <lamarque@kde.org>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License as
   published by the Free Software Foundation; either version 2 of
   the License or (at your option) version 3 or any later version
   accepted by the membership of KDE e.V. (or its successor approved
   by the membership of KDE e.V.), which shall act as a proxy
   defined in Section 14 of version 3 of the license.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef MODEMMANAGER_GENERIC_TYPES_H
#define MODEMMANAGER_GENERIC_TYPES_H

#include <QMetaType>
#include <QDBusArgument>
#include <QtDBus/QtDBus>

#include "manager.h"
#include "modemlocationinterface.h"
#include "modemcdmainterface.h"
#include "modemgsmcontactsinterface.h"
#include "modemgsmnetworkinterface.h"
//#include "networkipv4config.h"

typedef QList<QVariantMap> QVariantMapList;

Q_DECLARE_METATYPE(ModemManager::ModemInterface::Ip4ConfigType)
Q_DECLARE_METATYPE(ModemManager::ModemInterface::InfoType)
Q_DECLARE_METATYPE(ModemManager::ModemLocationInterface::LocationInformationMap)
Q_DECLARE_METATYPE(ModemManager::ModemCdmaInterface::ServingSystemType)
Q_DECLARE_METATYPE(ModemManager::ModemCdmaInterface::RegistrationStateResult)
Q_DECLARE_METATYPE(ModemManager::ModemGsmContactsInterface::ContactType)
Q_DECLARE_METATYPE(ModemManager::ModemGsmContactsInterface::ContactTypeList)
Q_DECLARE_METATYPE(ModemManager::ModemGsmNetworkInterface::ScanResultsType)
Q_DECLARE_METATYPE(ModemManager::ModemGsmNetworkInterface::RegistrationInfoType)
Q_DECLARE_METATYPE(QList<QVariantMap>)

// ModemManager::ModemManager::Modem::Ip4Configtype
QDBusArgument &operator << (QDBusArgument &arg,
    const ModemManager::ModemInterface::Ip4ConfigType &config);

const QDBusArgument &operator >> (const QDBusArgument &arg,
    ModemManager::ModemInterface::Ip4ConfigType &config);

// ModemManager::ModemManager::Modem::InfoType
QDBusArgument &operator << (QDBusArgument &arg,
    const ModemManager::ModemInterface::InfoType &info);

const QDBusArgument &operator >> (const QDBusArgument &arg,
    ModemManager::ModemInterface::InfoType &info);

// ModemManager::ModemCdmaInterface::ServingSystemType
QDBusArgument &operator << (QDBusArgument &arg,
    const ModemManager::ModemCdmaInterface::ServingSystemType &servingSystem);

const QDBusArgument &operator >> (const QDBusArgument &arg,
    ModemManager::ModemCdmaInterface::ServingSystemType &servingSystem);

// ModemManager::ModemGsmContactsInterface::ContactType
QDBusArgument &operator << (QDBusArgument &arg,
    const ModemManager::ModemGsmContactsInterface::ContactType &contact);

const QDBusArgument &operator >> (const QDBusArgument &arg,
    ModemManager::ModemGsmContactsInterface::ContactType &contact);

// ModemManager::ModemManager::Modem::Gsm::Network::RegistrationInfoType
QDBusArgument &operator << (QDBusArgument &arg,
    const ModemManager::ModemGsmNetworkInterface::RegistrationInfoType &info);

const QDBusArgument &operator >> (const QDBusArgument &arg,
    ModemManager::ModemGsmNetworkInterface::RegistrationInfoType &info);

inline void registerModemManagerTypes() {
        qDBusRegisterMetaType<ModemManager::ModemInterface::Ip4ConfigType>();
        qDBusRegisterMetaType<ModemManager::ModemInterface::InfoType>();
        qDBusRegisterMetaType<ModemManager::ModemCdmaInterface::ServingSystemType>();
        qDBusRegisterMetaType<ModemManager::ModemGsmContactsInterface::ContactType>();
        qDBusRegisterMetaType<ModemManager::ModemGsmNetworkInterface::RegistrationInfoType>();
}

#endif // MODEMMANAGER_TYPES_H
