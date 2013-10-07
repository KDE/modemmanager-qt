/*
Copyright 2008,2011 Will Stephenson <wstephenson@kde.org>
Copyright 2010 Lamarque Souza <lamarque@kde.org>
Copyright 2013 Daniel Nicoletti <dantti12@gmail.com>
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

#ifndef MODEMMANAGERQT_MANAGER_H
#define MODEMMANAGERQT_MANAGER_H

#include "ModemManagerQt-export.h"

#include <QObject>
#include <QDBusObjectPath>
#include <QSharedPointer>

#include "modemdevice.h"
#include "bearer.h"
#include "sim.h"

/**
 * This namespace allows to query the underlying system to discover the available
 * modem interfaces
 * responsibility to notify when a modem interface or a modem appears or disappears.
 *
 * Note that it's implemented as a singleton and encapsulates the backend logic.
 */
namespace ModemManager
{
class Modem;

class MODEMMANAGERQT_EXPORT Notifier : public QObject
{
    Q_OBJECT
Q_SIGNALS:
    /**
     * This signal is emitted when a new modem interface is available.
     *
     * @param udi the network interface identifier
     */
    void modemAdded(const QString &udi);
    /**
     * This signal is emitted when a network interface is not available anymore.
     *
     * @param udi the network interface identifier
     */
    void modemRemoved(const QString &udi);

    void simAdded(const QString &udi);
    void simRemoved(const QString &udi);

    void bearerAdded(const QString &udi);
    void bearerRemoved(const QString &udi);
    
    /**
      * This signal is emitted when the ModemManager DBus service goes away
      */
    void serviceDisappeared();
    /**
      * This signal is emitted when the ModemManager DBus service appears
      */
    void serviceAppeared();
};

/**
  * Retrieves the list of all modem interfaces Unique Device Identifiers (UDIs)
  * in the system. This method is the equivalent of enumerateDevices described
  * in Modem Manager specification.
  *
  * Note: only HW modems are returned (Gsm or Cdma)
  *
  * @return the list of modem interfaces available in this system
  */
MODEMMANAGERQT_EXPORT ModemDevice::List modemDevices();

/**
  * Find a new ModemManagerInterface object given its UDI.
  *
  * Note: only Modem-inherited objects are returned (not SMS, SIM or Bearer objects)
  *
  * @param uni the identifier of the modem interface to find
  * @returns a valid Modem object if there's a device having the given UDI, an invalid one otherwise
  */
MODEMMANAGERQT_EXPORT ModemDevice::Ptr findModemDevice(const QString &uni);

MODEMMANAGERQT_EXPORT ModemManager::Bearer::Ptr findBearer(const QString &uni);
MODEMMANAGERQT_EXPORT ModemManager::Bearer::List bearers();
MODEMMANAGERQT_EXPORT ModemManager::Sim::Ptr findSim(const QString &uni);
MODEMMANAGERQT_EXPORT ModemManager::Sim::List sims();

/**
  * Start a new scan for connected modem devices.
  */
MODEMMANAGERQT_EXPORT void scanDevices();

MODEMMANAGERQT_EXPORT Notifier * notifier();
}

#endif
