/*
    Copyright 2015 Jan Grulich <jgrulich@redhat.com>

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

#include "fakemodem.h"
#include "dbus/fakedbus.h"

#include <QDBusConnection>

FakeModem::FakeModem(QObject* parent)
    : QObject(parent)
    , m_bearerCounter(0)
    , m_modemCounter(0)
    , m_objectManager(new ObjectManager(this))
{
    qDBusRegisterMetaType<ModemManager::MMVariantMapMap> ();
    qDBusRegisterMetaType<ModemManager::UIntList> ();
    qDBusRegisterMetaType<ModemManager::CurrentModesType> ();
    qDBusRegisterMetaType<ModemManager::SupportedModesType> ();
    qDBusRegisterMetaType<ModemManager::SignalQualityPair> ();
    qDBusRegisterMetaType<ModemManager::PortList> ();
    qDBusRegisterMetaType<ModemManager::UnlockRetriesMap> ();
    qDBusRegisterMetaType<ModemManager::Port> ();
    qDBusRegisterMetaType<ModemManager::LocationInformationMap>();

    QDBusConnection::sessionBus().registerService(QLatin1String(MMQT_DBUS_SERVICE));
    QDBusConnection::sessionBus().registerObject(QLatin1String(MMQT_DBUS_PATH), this, QDBusConnection::ExportScriptableContents | QDBusConnection::ExportAdaptors);
}

FakeModem::~FakeModem()
{
    QDBusConnection::sessionBus().unregisterObject(QLatin1String(MMQT_DBUS_SERVICE));
    QDBusConnection::sessionBus().unregisterService(QLatin1String(MMQT_DBUS_SERVICE));
}

void FakeModem::addModem(Modem* modem)
{
    QString newModemPath = QStringLiteral("/org/kde/fakemodem/Modem/") + QString::number(m_modemCounter++);
    modem->setModemPath(newModemPath);
    // Start monitoring property changes
    modem->setEnableNotifications(true);
    m_modems.insert(QDBusObjectPath(newModemPath), modem);
    QDBusConnection::sessionBus().registerObject(newModemPath, modem, QDBusConnection::ExportScriptableContents | QDBusConnection::ExportAdaptors);

    ModemManager::MMVariantMapMap interfaces;
    interfaces.insert(QLatin1String(MMQT_DBUS_INTERFACE_MODEM), modem->toMap());

    m_objectManager->addInterfaces(QDBusObjectPath(newModemPath), interfaces);
}

void FakeModem::removeModem(Modem* modem)
{
    m_modems.remove(QDBusObjectPath(modem->modemPath()));
    QDBusConnection::sessionBus().unregisterObject(modem->modemPath());

    m_objectManager->removeInterfaces(QDBusObjectPath(modem->modemPath()), {QLatin1String(MMQT_DBUS_INTERFACE_MODEM)});
}

void FakeModem::addBearer(Bearer* bearer)
{
    QString newBearerPath = QStringLiteral("/org/kde/fakemodem/Bearer/") + QString::number(m_bearerCounter++);
    bearer->setBearerPath(newBearerPath);
    bearer->setEnableNotifications(true);
    m_bearers.insert(QDBusObjectPath(newBearerPath), bearer);
    QDBusConnection::sessionBus().registerObject(newBearerPath, bearer, QDBusConnection::ExportScriptableContents);
    Q_FOREACH (Modem * modem, m_modems.values()) {
        modem->addBearer(QDBusObjectPath(newBearerPath));
        QVariantMap map;
        map.insert(QLatin1String("Bearers"), QVariant::fromValue<QList<QDBusObjectPath> >(modem->bearers()));
        QDBusMessage message = QDBusMessage::createSignal(modem->modemPath(), QLatin1String("org.freedesktop.DBus.Properties"), QLatin1String("PropertiesChanged"));
        message << QLatin1String("org.kde.fakemodem.Modem") << map << QStringList();
        QDBusConnection::sessionBus().send(message);
    }
}

void FakeModem::removeBearer(Bearer* bearer)
{
    m_bearers.remove(QDBusObjectPath(bearer->bearerPath()));
    QDBusConnection::sessionBus().unregisterObject(bearer->bearerPath());
    Q_FOREACH (Modem * modem, m_modems.values()) {
        modem->removeBearer(QDBusObjectPath(bearer->bearerPath()));
        QVariantMap map;
        map.insert(QLatin1String("Bearers"), QVariant::fromValue<QList<QDBusObjectPath> >(modem->bearers()));
        QDBusMessage message = QDBusMessage::createSignal(modem->modemPath(), QLatin1String("org.freedesktop.DBus.Properties"), QLatin1String("PropertiesChanged"));
        message << QLatin1String("org.kde.fakemodem.Modem") << map << QStringList();
        QDBusConnection::sessionBus().send(message);
    }
}

void FakeModem::addInterfaces(const QDBusObjectPath& object_path, const ModemManager::MMVariantMapMap& interfaces_and_properties)
{
    m_objectManager->addInterfaces(object_path, interfaces_and_properties);
}

void FakeModem::removeInterfaces(const QDBusObjectPath& object_path, const QStringList& interfaces)
{
    m_objectManager->removeInterfaces(object_path, interfaces);
}


void FakeModem::ScanDevices()
{
}

void FakeModem::SetLogging(const QString& level)
{
    Q_UNUSED(level);
}
