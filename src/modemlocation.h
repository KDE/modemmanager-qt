/*
    Copyright 2008,2011 Will Stephenson <wstephenson@kde.org>
    Copyright 2010 Lamarque Souza <lamarque@kde.org>
    Copyright 2013 Lukas Tinkl <ltinkl@redhat.com>
    Copyright 2013 Jan Grulich <jgrulich@redhat.com>

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

#ifndef MODEMMANAGERQT_MODEMLOCATION_H
#define MODEMMANAGERQT_MODEMLOCATION_H

#include "ModemManagerQt-export.h"

#include <QObject>
#include <QSharedPointer>

#include "generictypes.h"
#include "interface.h"

class ModemLocationPrivate;

namespace ModemManager
{
/**
 * @brief The ModemLocation class
 *
 * The Location class allows devices to provide location information to
 * client applications. Not all devices can provide this information, or even if
 * they do, they may not be able to provide it while a data session is active.
 */
class MODEMMANAGERQT_EXPORT ModemLocation : public Interface
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(ModemLocation)
    Q_FLAGS(MMModemLocationSource)

public:
    typedef QSharedPointer<ModemLocation> Ptr;
    typedef QList<Ptr> List;

    Q_DECLARE_FLAGS(LocationSources, MMModemLocationSource)

    explicit ModemLocation(const QString &path, QObject *parent = 0);
    ~ModemLocation();

    /**
     * Configure the location sources to use when gathering location
     * information. Also enable or disable location information gathering. This
     * method may require the client to authenticate itself.
     *
     * When signals are emitted, any client application (including malicious
     * ones!) can listen for location updates unless D-Bus permissions restrict
     * these signals from certain users. If further security is desired, the
     * @p signLocation argument can be set to FALSE to disable location updates
     * via the locationChanged() signal and require applications to call authenticated APIs
     * (like GetLocation() ) to get location information.
     */
    void setup(ModemManager::ModemLocation::LocationSources sources, bool signalLocation);

    /**
     * @return current location information, if any. If the modem supports
     * multiple location types it may return more than one. See the "Location"
     * property for more information on the dictionary returned at location.
     *
     * This method may require the client to authenticate itself.
     */
    LocationInformationMap location(); // TODO process this better

    /**
     * @return QFlags of MMModemLocationSource values, specifying the supported location sources.
     */
    LocationSources capabilities() const;

    /**
     * @return QFlags specifying which of the supported MMModemLocationSource location sources is currently enabled in the device.
     */
    LocationSources enabledCapabilities() const;

    /**
     * @return whether the device has any location capabilities
     */
    bool isEnabled() const;

    /**
     * @return TRUE if location updates will be emitted via the locationChanged() signal, FALSE if location updates will not be emitted.
     *
     * See the setup() method for more information.
     */
    bool signalsLocation() const;

Q_SIGNALS:
    void capabilitiesChanged(LocationSources sources);
    void isEnabledChanged(bool enabled);
    void signalsLocationChanged(bool signalsLocation);
    /**
     * Emitted when the location has changed
     */
    void locationChanged(const LocationInformationMap &location);

private Q_SLOTS:
   void onPropertiesChanged(const QString &interface, const QVariantMap &properties, const QStringList &invalidatedProps);
};

Q_DECLARE_OPERATORS_FOR_FLAGS(ModemLocation::LocationSources)

} // namespace ModemManager

#endif
