/*
    SPDX-FileCopyrightText: 2010 Lamarque Souza <lamarque@kde.org>
    SPDX-FileCopyrightText: 2013 Lukas Tinkl <ltinkl@redhat.com>
    SPDX-FileCopyrightText: 2013-2015 Jan Grulich <jgrulich@redhat.com>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "generictypes.h"
#include "generictypes_p.h"

#include <QDBusMetaType>

QDBusArgument &operator<<(QDBusArgument &arg, const ModemManager::Port &port)
{
    arg.beginStructure();
    arg << port.name << port.type;
    arg.endStructure();
    return arg;
}

const QDBusArgument &operator>>(const QDBusArgument &arg, ModemManager::Port &port)
{
    QString temp1;
    uint temp2;
    arg.beginStructure();
    arg >> temp1 >> temp2;
    port.name = temp1;
    port.type = (MMModemPortType)temp2;
    arg.endStructure();

    return arg;
}

// Marshall the CurrentModesType data into a D-BUS argument
QDBusArgument &operator<<(QDBusArgument &arg, const ModemManager::CurrentModesType &mode)
{
    arg.beginStructure();
    arg << mode.allowed << mode.preferred;
    arg.endStructure();
    return arg;
}

// Retrieve the CurrentModesType data from the D-BUS argument
const QDBusArgument &operator>>(const QDBusArgument &arg, ModemManager::CurrentModesType &mode)
{
    uint temp1;
    uint temp2;
    arg.beginStructure();
    arg >> temp1 >> temp2;
    mode.allowed = (MMModemMode)temp1;
    mode.preferred = (MMModemMode)temp2;
    arg.endStructure();

    return arg;
}

// Marshall the ConnectionError data into a D-BUS argument
QDBusArgument &operator<<(QDBusArgument &arg, const ModemManager::ConnectionError &error)
{
    arg.beginStructure();
    arg << error.name << error.message;
    arg.endStructure();
    return arg;
}

// Retrieve the ConnectionError data from the D-BUS argument
const QDBusArgument &operator>>(const QDBusArgument &arg, ModemManager::ConnectionError &error)
{
    QString name;
    QString message;
    arg.beginStructure();
    arg >> name >> message;
    error.name = name;
    error.message = message;
    arg.endStructure();

    return arg;
}

// Marshall the FacilityLock data into a D-BUS argument
QDBusArgument &operator<<(QDBusArgument &arg, const ModemManager::FacilityLock &lock)
{
    arg.beginStructure();
    arg << lock.facility << lock.controlKey;
    arg.endStructure();
    return arg;
}

// Retrieve the FacilityLock data from the D-BUS argument
const QDBusArgument &operator>>(const QDBusArgument &arg, ModemManager::FacilityLock &lock)
{
    uint facility;
    QString controlKey;
    arg.beginStructure();
    arg >> facility >> controlKey;
    lock.facility = facility;
    lock.controlKey = controlKey;
    arg.endStructure();

    return arg;
}

// Marshall the PcoInfo data into a D-BUS argument
QDBusArgument &operator<<(QDBusArgument &arg, const ModemManager::PcoInfo &info)
{
    arg.beginStructure();
    arg << info.sessionId << info.isComplete << info.data;
    arg.endStructure();
    return arg;
}

// Retrieve the PcoInfo data from the D-BUS argument
const QDBusArgument &operator>>(const QDBusArgument &arg, ModemManager::PcoInfo &info)
{
    uint sessionId;
    bool isComplete;
    QByteArray data;
    arg.beginStructure();
    arg >> sessionId >> isComplete >> data;
    info.sessionId = sessionId;
    info.isComplete = isComplete;
    info.data = data;
    arg.endStructure();

    return arg;
}

// Marshall the FirmwareUpdateSettings data into a D-BUS argument
QDBusArgument &operator<<(QDBusArgument &arg, const ModemManager::FirmwareUpdateSettings &settings)
{
    arg.beginStructure();
    arg << settings.methods << settings.settings;
    arg.endStructure();
    return arg;
}

// Retrieve the FirmwareUpdateSettings data from the D-BUS argument
const QDBusArgument &operator>>(const QDBusArgument &arg, ModemManager::FirmwareUpdateSettings &settings)
{
    uint methods;
    QVariantMap map;
    arg.beginStructure();
    arg >> methods >> map;
    settings.methods = methods;
    settings.settings = map;
    arg.endStructure();

    return arg;
}

#if MM_CHECK_VERSION(1, 24, 0)
// Marshall the CellBroadcastChannelRange data into a D-BUS argument
QDBusArgument &operator<<(QDBusArgument &arg, const ModemManager::CellBroadcastChannelRange &range)
{
    arg.beginStructure();
    arg << range.from << range.to;
    arg.endStructure();
    return arg;
}

// Retrieve the CellBroadcastChannelRange data from the D-BUS argument
const QDBusArgument &operator>>(const QDBusArgument &arg, ModemManager::CellBroadcastChannelRange &range)
{
    uint from;
    uint to;
    arg.beginStructure();
    arg >> from >> to;
    range.from = from;
    range.to = to;
    arg.endStructure();

    return arg;
}
#endif

#if MM_CHECK_VERSION(1, 2, 0)
// Marshall the OmaSessionType data into a D-BUS argument
QDBusArgument &operator<<(QDBusArgument &arg, const ModemManager::OmaSessionType &sessionType)
{
    arg.beginStructure();
    arg << sessionType.type << sessionType.id;
    arg.endStructure();
    return arg;
}

// Retrieve the OmaSessionType data from the D-BUS argument
const QDBusArgument &operator>>(const QDBusArgument &arg, ModemManager::OmaSessionType &sessionType)
{
    uint type;
    uint id;
    arg.beginStructure();
    arg >> type >> id;
    sessionType.type = (MMOmaSessionType)type;
    sessionType.id = id;
    arg.endStructure();

    return arg;
}
#endif

// Marshall the SignalQualityPair data into a D-BUS argument
QDBusArgument &operator<<(QDBusArgument &arg, const ModemManager::SignalQualityPair &sqp)
{
    arg.beginStructure();
    arg << sqp.signal << sqp.recent;
    arg.endStructure();
    return arg;
}

// Retrieve the SignalQualityPair data from the D-BUS argument
const QDBusArgument &operator>>(const QDBusArgument &arg, ModemManager::SignalQualityPair &sqp)
{
    arg.beginStructure();
    arg >> sqp.signal >> sqp.recent;
    arg.endStructure();
    return arg;
}

// Marshall the ValidityPair data into a D-BUS argument
QDBusArgument &operator<<(QDBusArgument &arg, const ModemManager::ValidityPair &vp)
{
    arg.beginStructure();
    arg << vp.validity << vp.value;
    arg.endStructure();
    return arg;
}

// Retrieve the ValidityPair data from the D-BUS argument
const QDBusArgument &operator>>(const QDBusArgument &arg, ModemManager::ValidityPair &vp)
{
    uint temp1;
    uint temp2;
    arg.beginStructure();
    arg >> temp1 >> temp2;
    vp.validity = (MMSmsValidityType)temp1;
    vp.value = temp2;
    arg.endStructure();

    return arg;
}

// Marshall the UnlockRetriesMap data into a D-BUS argument
QDBusArgument &operator<<(QDBusArgument &argument, const ModemManager::UnlockRetriesMap &unlockRetriesMap)
{
    argument.beginMap(QMetaType::Int, QMetaType::UInt);

    QMapIterator<MMModemLock, uint> i(unlockRetriesMap);
    while (i.hasNext()) {
        i.next();
        argument.beginMapEntry();
        argument << i.key() << i.value();
        argument.endMapEntry();
    }
    argument.endMap();
    return argument;
}

// Retrieve the UnlockRetriesMap data from the D-BUS argument
const QDBusArgument &operator>>(const QDBusArgument &argument, ModemManager::UnlockRetriesMap &unlockRetriesMap)
{
    argument.beginMap();
    unlockRetriesMap.clear();

    while (!argument.atEnd()) {
        int key;
        uint value;
        argument.beginMapEntry();
        argument >> key;
        argument >> value;
        argument.endMapEntry();
        unlockRetriesMap.insert((MMModemLock)key, value);
    }

    argument.endMap();
    return argument;
}

// Marshal QList<QVariantMap> into a D-BUS argument
QDBusArgument &operator<<(QDBusArgument &argument, const ModemManager::QVariantMapList &variantMapList)
{
    argument.beginArray(qMetaTypeId<QVariantMap>());
    for (int i = 0; i < variantMapList.length(); ++i) {
        argument << variantMapList[i];
    }
    argument.endArray();
    return argument;
}

// Retrieve QList<QVariantMap> from a D-BUS argument
const QDBusArgument &operator>>(const QDBusArgument &argument, ModemManager::QVariantMapList &variantMapList)
{
    argument.beginArray();
    variantMapList.clear();

    while (!argument.atEnd()) {
        QVariantMap element;
        argument >> element;
        variantMapList.append(element);
    }

    argument.endArray();
    return argument;
}

// Marshal LocationInformationMap into a D-BUS argument
QDBusArgument &operator<<(QDBusArgument &argument, const ModemManager::LocationInformationMap &locationMap)
{
    argument.beginMap(qMetaTypeId<uint>(), qMetaTypeId<QDBusVariant>());

    QMapIterator<MMModemLocationSource, QVariant> i(locationMap);
    while (i.hasNext()) {
        i.next();
        argument.beginMapEntry();
        argument << (uint)i.key() << QDBusVariant(i.value());
        argument.endMapEntry();
    }
    argument.endMap();
    return argument;
}

// Retrieve LocationInformationMap from a D-BUS argument
const QDBusArgument &operator>>(const QDBusArgument &argument, ModemManager::LocationInformationMap &locationMap)
{
    argument.beginMap();
    locationMap.clear();

    while (!argument.atEnd()) {
        uint key;
        QDBusVariant value;
        argument.beginMapEntry();
        argument >> key;
        argument >> value;
        argument.endMapEntry();
        locationMap.insert((MMModemLocationSource)key, value.variant());
    }

    argument.endMap();
    return argument;
}

void registerModemManagerTypes()
{
    qDBusRegisterMetaType<ModemManager::MMVariantMapMap>();
    qDBusRegisterMetaType<ModemManager::DBUSManagerStruct>();
    qDBusRegisterMetaType<ModemManager::UIntList>();
    qDBusRegisterMetaType<ModemManager::UIntListList>();
    qDBusRegisterMetaType<ModemManager::ConnectionError>();
    qDBusRegisterMetaType<ModemManager::FacilityLock>();
    qDBusRegisterMetaType<ModemManager::PcoInfo>();
    qDBusRegisterMetaType<ModemManager::PcoInfoList>();
    qDBusRegisterMetaType<ModemManager::FirmwareUpdateSettings>();
    qDBusRegisterMetaType<ModemManager::Port>();
    qDBusRegisterMetaType<ModemManager::PortList>();
    qDBusRegisterMetaType<ModemManager::CurrentModesType>();
    qDBusRegisterMetaType<ModemManager::SignalQualityPair>();
    qDBusRegisterMetaType<ModemManager::SupportedModesType>();
#if MM_CHECK_VERSION(1, 24, 0)
    qDBusRegisterMetaType<ModemManager::CellBroadcastChannelRange>();
    qDBusRegisterMetaType<ModemManager::CellBroadcastChannelRangeList>();
#endif
    qDBusRegisterMetaType<ModemManager::UnlockRetriesMap>();
    qDBusRegisterMetaType<ModemManager::QVariantMapList>();
#if MM_CHECK_VERSION(1, 2, 0)
    qDBusRegisterMetaType<ModemManager::OmaSessionType>();
    qDBusRegisterMetaType<ModemManager::OmaSessionTypes>();
#endif
    qDBusRegisterMetaType<ModemManager::LocationInformationMap>();
    qDBusRegisterMetaType<ModemManager::ValidityPair>();
    qDBusRegisterMetaType<ModemManager::PortList>();
    qRegisterMetaType<MMModemMode>("MMModemMode");
    qRegisterMetaType<MMModemLock>("MMModemLock");
    qRegisterMetaType<MMModem3gppUssdSessionState>("MMModem3gppUssdSessionState");
    qRegisterMetaType<MMModemLocationSource>("MMModemLocationSource");
}
