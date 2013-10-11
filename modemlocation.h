/*
Copyright 2008,2011 Will Stephenson <wstephenson@kde.org>
Copyright 2010 Lamarque Souza <lamarque@kde.org>
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

#ifndef MODEMMANAGERQT_MODEMLOCATION_H
#define MODEMMANAGERQT_MODEMLOCATION_H

#include "ModemManagerQt-export.h"

#include <QObject>
#include <QSharedPointer>

#include "generic-types.h"
#include "interface.h"

class ModemLocationPrivate;

namespace ModemManager
{
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

    // methods
    void setup(ModemManager::ModemLocation::LocationSources sources, bool signalLocation);
    LocationInformationMap location(); // TODO process this better

    // properties
    LocationSources capabilities() const;
    LocationSources enabledCapabilities() const;
    bool isEnabled() const;
    bool signalsLocation() const;

Q_SIGNALS:
    void capabilitiesChanged(LocationSources sources);
    void isEnabledChanged(bool enabled);
    void signalsLocationChanged(bool signalsLocation);
    void locationChanged(const LocationInformationMap &location);

private Q_SLOTS:
   void onPropertiesChanged(const QString &interface, const QVariantMap &properties, const QStringList &invalidatedProps);
};

Q_DECLARE_OPERATORS_FOR_FLAGS(ModemLocation::LocationSources)

} // namespace ModemManager

#endif
