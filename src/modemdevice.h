/*
    SPDX-FileCopyrightText: 2013, 2014 Lukas Tinkl <ltinkl@redhat.com>
    SPDX-FileCopyrightText: 2013-2015 Jan Grulich <jgrulich@redhat.com>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef MODEMMANAGERQT_MODEMDEVICE_H
#define MODEMMANAGERQT_MODEMDEVICE_H

#include <ModemManager/ModemManager.h>

#include <modemmanagerqt_export.h>

#include <QObject>
#include <QSharedPointer>

#include "bearer.h"
#include "generictypes.h"
#include "interface.h"
#include "modem.h"
#if MM_CHECK_VERSION(1, 24, 0)
#include "modemcellbroadcast.h"
#endif
#include "modemmessaging.h"
#include "sim.h"

namespace ModemManager
{
class ModemDevicePrivate;

/*!
 * \class ModemManager::ModemDevice
 *
 * \inheaderfile ModemManager::ModemDevice
 *
 * \inmodule ModemManagerQt
 */
class MODEMMANAGERQT_EXPORT ModemDevice : public QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(ModemDevice)

    Q_PROPERTY(QString uni READ uni)
public:
    /*!
     * \typedef ModemManager::ModemDevice::Ptr
     */
    typedef QSharedPointer<ModemDevice> Ptr;
    /*!
     * \typedef ModemManager::ModemDevice::List
     */
    typedef QList<Ptr> List;

    /*!
     * \enum InterfaceType
     *
     * \value ModemInterface
     * \value SimpleInterface
     * \value GsmInterface
     * \value GsmUssdInterface
     * \value CdmaInterface
     * \value MessagingInterface
     * \value CellBroadcastInterface
     * \value LocationInterface
     * \value TimeInterface
     * \value FirmwareInterface
     * \value OmaInterface
     * \value SignalInterface
     * \value VoiceInterface
     */
    enum InterfaceType {
        ModemInterface,
        SimpleInterface,
        GsmInterface,
        GsmUssdInterface,
        CdmaInterface,
        MessagingInterface,
#if MM_CHECK_VERSION(1, 24, 0)
        CellBroadcastInterface,
#endif
        LocationInterface,
        TimeInterface,
        FirmwareInterface,
        OmaInterface,
        SignalInterface,
        VoiceInterface,
    };

    /*!
     */
    explicit ModemDevice(const QString &path, QObject *parent = nullptr);
    ~ModemDevice() override;

    /*!
     */
    QString uni() const;

    /*!
     */
    bool hasInterface(InterfaceType type) const;
    /*!
     */
    ModemManager::Interface::List interfaces() const;
    /*!
     */
    ModemManager::Interface::Ptr interface(InterfaceType type) const;

    /*!
     */
    ModemManager::ModemMessaging::Ptr messagingInterface();
#if MM_CHECK_VERSION(1, 24, 0)
    /*!
     * \since 6.24.0
     */
    ModemManager::ModemCellBroadcast::Ptr cellBroadcastInterface();
#endif
    /*!
     */
    ModemManager::Modem::Ptr modemInterface();

    /*!
     */
    ModemManager::Bearer::Ptr findBearer(const QString &uni);
    /*!
     */
    ModemManager::Bearer::List bearers() const;
    /*!
     */
    ModemManager::Sim::Ptr sim() const;

    /*!
     */
    bool isGsmModem() const;
    /*!
     */
    bool isCdmaModem() const;

Q_SIGNALS:
    /*!
     */
    void simAdded(const QString &udi);
    /*!
     */
    void simRemoved(const QString &udi);

    /*!
     */
    void bearerAdded(const QString &udi); // TODO emit these signals in the next version
    /*!
     */
    void bearerRemoved(const QString &udi);

    /*!
     */
    void interfaceAdded(InterfaceType type);
    /*!
     */
    void interfaceRemoved(InterfaceType type);

protected:
    /*!
     */
    MODEMMANAGERQT_NO_EXPORT explicit ModemDevice(ModemDevicePrivate &dd, QObject *parent = nullptr);

private:
    ModemDevicePrivate *const d_ptr;
};
} // namespace ModemManager

#endif
