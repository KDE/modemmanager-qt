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

#include "modemtest.h"

#include "manager.h"
#include "modem.h"

#include "fakemodem/modem.h"

#include <QTest>
#include <QSignalSpy>
#include <QtTest/qsignalspy.h>

void ModemTest::initTestCase()
{
    fakeModem = new FakeModem();
}

void ModemTest::testModems()
{
    Modem *modem = new Modem();
    modem->setAccessTechnologies(16);
    // TODO bearers
    // modem->addBearer();
    modem->SetCurrentBands({0});
    modem->SetCurrentCapabilities(4);
    // TODO
    // modem->SetCurrentModes();
    modem->setDevice(QLatin1String("/sys/devices/pci0000:00/0000:00:1d.0/usb4/4-1/4-1.2"));
    modem->setDeviceIdentifier(QLatin1String("1c435eb6d74494b5f78d7221e2c5ae9ec526a981"));
    modem->setDrivers({QLatin1String("option1")});
    modem->setEquipmentIdentifier(QLatin1String("353475021085110"));
    modem->setManufacturer(QLatin1String("huawei"));
    modem->setMaxActiveBearers(1);
    modem->setMaxBearers(1);
    modem->setModel(QLatin1String("K2540"));
    // TODO
    // modem->setOwnNumbers();
    modem->setPlugin(QLatin1String("Huawei"));
    modem->setPorts({{QLatin1String("ttyUSB0"), MM_MODEM_PORT_TYPE_AT}, {QLatin1String("ttyUSB1"), MM_MODEM_PORT_TYPE_QCDM}, {QLatin1String("ttyUSB2"), MM_MODEM_PORT_TYPE_AT}});
    modem->SetPowerState(3);
    modem->setPrimaryPort(QLatin1String("ttyUSB2"));
    modem->setRevision(QLatin1String("11.001.05.00.11"));
    modem->setSignalQuality({93, true});
    modem->setSim(QDBusObjectPath("/org/kde/fakemodem/SIM/1"));
    modem->setState(8);
    modem->setStateFailedReason(0);
    modem->setSupportedBands({0});
    modem->setSupportedCapabilities({4});
    modem->setSupportedIpFamilies(3);
    // TODO
    // modem->setSupportedModes();
    modem->setUnlockRequired(1);
    modem->setUnlockRetries({{MM_MODEM_LOCK_SIM_PIN, 3}, {MM_MODEM_LOCK_SIM_PIN2, 3}, {MM_MODEM_LOCK_SIM_PUK, 10}, {MM_MODEM_LOCK_SIM_PUK2, 10}});

    QSignalSpy addModemSpy(ModemManager::notifier(), SIGNAL(modemAdded(QString)));
    connect(ModemManager::notifier(), &ModemManager::Notifier::modemAdded, this, &ModemTest::testModemAdded);
    fakeModem->addModem(modem);
    QVERIFY(addModemSpy.wait());
    QCOMPARE(ModemManager::modemDevices().count(), 1);
    QCOMPARE(ModemManager::modemDevices().first()->uni(), addModemSpy.at(0).at(0).toString());
    const QString addedModemPath = ModemManager::modemDevices().first()->uni();

    Bearer * bearer = new Bearer();
    // We need to set some default values
    bearer->setConnected(false);
    bearer->setInterface(QLatin1String("ttyUSB0"));
    bearer->setIp4Config({{QLatin1String("method"), MM_BEARER_IP_METHOD_PPP}});
    bearer->setIp6Config({{QLatin1String("method"), MM_BEARER_IP_METHOD_UNKNOWN}});
    bearer->setIpTimeout(20);
    bearer->setProperties({{QLatin1String("apn"),QLatin1String("internet")},{QLatin1Literal("ip-type"), 1}, {QLatin1String("number"), QLatin1String("*99#")}});
    bearer->setSuspended(false);

    ModemManager::Modem::Ptr modemDevice = ModemManager::modemDevices().first()->modemInterface();
    QCOMPARE(modemDevice->listBearers().count(), 0);
    QSignalSpy bearerAddedSpy(modemDevice.data(), SIGNAL(bearerAdded(QString)));
    fakeModem->addBearer(bearer);
    QVERIFY(bearerAddedSpy.wait());
    QCOMPARE(modemDevice->listBearers().count(), 1);

    QSignalSpy bearerRemovedSpy(modemDevice.data(), SIGNAL(bearerRemoved(QString)));
    fakeModem->removeBearer(bearer);
    QVERIFY(bearerRemovedSpy.wait());
    QCOMPARE(modemDevice->listBearers().count(), 0);

    QSignalSpy removeModemSpy(ModemManager::notifier(), SIGNAL(modemRemoved(QString)));
    fakeModem->removeModem(modem);
    QVERIFY(removeModemSpy.wait());
    QVERIFY(ModemManager::modemDevices().isEmpty());
    QCOMPARE(removeModemSpy.at(0).at(0).toString(), addedModemPath);

    delete modem;
}

void ModemTest::testModemAdded(const QString &dev)
{
    ModemManager::ModemDevice::Ptr modemDevice = ModemManager::findModemDevice(dev);
    ModemManager::Modem::Ptr modem = modemDevice->modemInterface();

    QCOMPARE(modem->accessTechnologies(), 16);
    // TODO bearers
    QList<MMModemBand> bands = {MM_MODEM_BAND_UNKNOWN};
//     bands << MM_MODEM_BAND_UNKNOWN;
    QCOMPARE(modem->currentBands(), bands);
    QCOMPARE(modem->currentCapabilities(), 4);
    QCOMPARE(modem->device(), QLatin1String("/sys/devices/pci0000:00/0000:00:1d.0/usb4/4-1/4-1.2"));
    QCOMPARE(modem->deviceIdentifier(), QLatin1String("1c435eb6d74494b5f78d7221e2c5ae9ec526a981"));
    QCOMPARE(modem->drivers(), QStringList(QLatin1String("option1")));
    QCOMPARE(modem->equipmentIdentifier(), QLatin1String("353475021085110"));
    QCOMPARE(modem->manufacturer(), QLatin1String("huawei"));
    QCOMPARE(modem->maxActiveBearers(), (uint)1);
    QCOMPARE(modem->maxBearers(), (uint)1);
    QCOMPARE(modem->model(), QLatin1String("K2540"));
    // TODO ownNumbers
    QCOMPARE(modem->plugin(), QLatin1String("Huawei"));
    ModemManager::PortList ports = {{QLatin1String("ttyUSB0"), MM_MODEM_PORT_TYPE_AT}, {QLatin1String("ttyUSB1"), MM_MODEM_PORT_TYPE_QCDM}, {QLatin1String("ttyUSB2"), MM_MODEM_PORT_TYPE_AT}};
    ModemManager::PortList modemPorts = modem->ports();
    QCOMPARE(ports.count(), modemPorts.count());
    Q_FOREACH (const ModemManager::Port port, ports) {
        bool found = false;
        Q_FOREACH(const ModemManager::Port modemPort, modemPorts) {
            if (port.name == modemPort.name) {
                found = true;
                QCOMPARE(port.type, modemPort.type);
            }
        }
        QVERIFY(found);
        found = false;
    }
    QCOMPARE(modem->powerState(), MM_MODEM_POWER_STATE_ON);
    QCOMPARE(modem->primaryPort(), QLatin1String("ttyUSB2"));
    QCOMPARE(modem->revision(), QLatin1String("11.001.05.00.11"));
    ModemManager::SignalQualityPair signalQuality = {93, true};
    ModemManager::SignalQualityPair modemSignalQuality = modem->signalQuality();
    QCOMPARE(modemSignalQuality.recent, signalQuality.recent);
    QCOMPARE(modemSignalQuality.signal, signalQuality.signal);
    QCOMPARE(modem->simPath(), QLatin1String("/org/kde/fakemodem/SIM/1"));
    QCOMPARE(modem->state(), MM_MODEM_STATE_REGISTERED);
    QCOMPARE(modem->stateFailedReason(), MM_MODEM_STATE_FAILED_REASON_NONE);
    QList<MMModemBand> supportedBands = {MM_MODEM_BAND_UNKNOWN};
    QCOMPARE(modem->supportedBands(), supportedBands);
    QList<MMModemCapability> supportedCapabilities = {(MMModemCapability)4};
    QCOMPARE(modem->supportedCapabilities(), supportedCapabilities);
    QCOMPARE(modem->supportedIpFamilies(), 3);
    QCOMPARE(modem->unlockRequired(), MM_MODEM_LOCK_NONE);
    ModemManager::UnlockRetriesMap unlockRetries = {{MM_MODEM_LOCK_SIM_PIN, 3}, {MM_MODEM_LOCK_SIM_PIN2, 3}, {MM_MODEM_LOCK_SIM_PUK, 10}, {MM_MODEM_LOCK_SIM_PUK2, 10}};
    QCOMPARE(modem->unlockRetries(), unlockRetries);
}

QTEST_MAIN(ModemTest)
