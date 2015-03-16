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

#include <QDBusConnection>

FakeModem::FakeModem(QObject* parent)
    : QObject(parent)
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

    QDBusConnection::sessionBus().registerService(QLatin1Literal(MMQT_DBUS_SERVICE));
    QDBusConnection::sessionBus().registerObject(QLatin1Literal(MMQT_DBUS_PATH), this, QDBusConnection::ExportScriptableContents | QDBusConnection::ExportAdaptors);
}

FakeModem::~FakeModem()
{
    QDBusConnection::sessionBus().unregisterObject(QLatin1Literal(MMQT_DBUS_SERVICE));
    QDBusConnection::sessionBus().unregisterService(QLatin1Literal(MMQT_DBUS_SERVICE));
}

void FakeModem::addModem(Modem* modem)
{
    QString newModemPath = QString("/org/kde/fakemodem/Modem/") + QString::number(m_modemCounter++);
    modem->setModemPath(newModemPath);
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

void FakeModem::ScanDevices()
{

}

void FakeModem::SetLogging(const QString& level)
{
    Q_UNUSED(level);
}
