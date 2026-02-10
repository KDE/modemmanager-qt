/*
    SPDX-FileCopyrightText: 2008, 2011 Will Stephenson <wstephenson@kde.org>
    SPDX-FileCopyrightText: 2010 Lamarque Souza <lamarque@kde.org>
    SPDX-FileCopyrightText: 2013 Daniel Nicoletti <dantti12@gmail.com>
    SPDX-FileCopyrightText: 2013 Lukas Tinkl <ltinkl@redhat.com>
    SPDX-FileCopyrightText: 2013 Jan Grulich <jgrulich@redhat.com>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef MODEMMANAGERQT_MANAGER_H
#define MODEMMANAGERQT_MANAGER_H

#include <modemmanagerqt_export.h>

#include <QDBusPendingReply>
#include <QObject>
#include <QSharedPointer>
#include <QVariantMap>

#include "modemdevice.h"

/*!
 * \namespace ModemManager
 * This namespace allows to query the underlying system to discover the available
 * modem interfaces
 * responsibility to notify when a modem interface or a modem appears or disappears.
 *
 * Note that it's implemented as a singleton and encapsulates the backend logic.
 */
namespace ModemManager
{
class Modem;

/*!
 * \class ModemManager::Notifier
 *
 * \inheaderfile ModemManagerQt/Manager
 *
 * \inmodule ModemManagerQt
 */
class MODEMMANAGERQT_EXPORT Notifier : public QObject
{
    Q_OBJECT
Q_SIGNALS:
    /*!
     * This signal is emitted when a new modem interface is available.
     *
     * \a udi the network interface identifier
     */
    void modemAdded(const QString &udi);
    /*!
     * This signal is emitted when a network interface is not available anymore.
     *
     * \a udi the network interface identifier
     */
    void modemRemoved(const QString &udi);

    /*!
     * This signal is emitted when the ModemManager DBus service goes away
     */
    void serviceDisappeared();
    /*!
     * This signal is emitted when the ModemManager DBus service appears
     */
    void serviceAppeared();
};

/*!
 * Retrieves the list of all modem interfaces Unique Device Identifiers (UDIs)
 * in the system. This method is the equivalent of enumerateDevices described
 * in Modem Manager specification.
 *
 * \note only HW modems are returned (Gsm or Cdma)
 *
 * Returns the list of modem interfaces available in this system
 */
MODEMMANAGERQT_EXPORT ModemDevice::List modemDevices();

/*!
 * Find a new ModemManagerInterface object given its UDI.
 *
 * \note only Modem-inherited objects are returned (not SMS, SIM or Bearer objects)
 *
 * \a uni the identifier of the modem interface to find
 * Returns a valid Modem object if there's a device having the given UDI, an invalid one otherwise
 */
MODEMMANAGERQT_EXPORT ModemDevice::Ptr findModemDevice(const QString &uni);

/*!
 * Start a new scan for connected modem devices.
 */
MODEMMANAGERQT_EXPORT void scanDevices();

/*!
 * Set logging verbosity.
 *
 * \a level one of "ERR", "WARN", "MSG" (since ModemManager 1.22), "INFO", "DEBUG".
 *
 * \since 6.24.0
 */
MODEMMANAGERQT_EXPORT QDBusPendingReply<> setLogging(const QString &level);

/*!
 * Reports a kernel event to ModemManager.
 *
 * This method is only available if udev is not being used to report kernel events.
 *
 * \a properties event properties dictionary. Mandatory keys are:
 *   "action" (string: "add" or "remove"), "name" (string: device name),
 *   "subsystem" (string: device subsystem). Optional key: "uid" (string:
 *   unique ID of the physical device; if not given, the sysfs path is used).
 *
 * \since 6.24.0
 */
MODEMMANAGERQT_EXPORT QDBusPendingReply<> reportKernelEvent(const QVariantMap &properties);

/*!
 * Inhibit or uninhibit the device.
 *
 * When the modem is inhibited, ModemManager will close all its ports and
 * unexport it from the bus, so that users of the interface are no longer
 * able to operate with it.
 *
 * The inhibition request is bound to the caller's existence on the DBus bus.
 * If the caller disappears, the inhibition is automatically removed.
 *
 * \a uid the unique ID of the physical device, as given in the
 *   org.freedesktop.ModemManager1.Modem Device property.
 * \a inhibit \c true to inhibit the modem, \c false to uninhibit it.
 *
 * \since 6.24.0
 */
MODEMMANAGERQT_EXPORT QDBusPendingReply<> inhibitDevice(const QString &uid, bool inhibit);

/*!
 * Returns the runtime version of the ModemManager daemon.
 *
 * \since 6.24.0
 */
MODEMMANAGERQT_EXPORT QString version();

/*!
 */
MODEMMANAGERQT_EXPORT Notifier *notifier();
}

#endif
