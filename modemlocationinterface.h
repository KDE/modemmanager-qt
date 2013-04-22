/*
Copyright 2008,2011 Will Stephenson <wstephenson@kde.org>
Copyright 2010 Lamarque Souza <lamarque@kde.org>

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

#ifndef MODEMMANAGER_MODEMLOCATIONINTERFACE_H
#define MODEMMANAGER_MODEMLOCATIONINTERFACE_H

#include "ModemManagerQt-export.h"

#include <QObject>
#include "modeminterface.h"

class ModemLocationInterfacePrivate;

namespace ModemManager
{
class QTMODEMMANAGER_EXPORT ModemLocationInterface : public ModemInterface
{
Q_OBJECT
Q_DECLARE_PRIVATE(ModemLocationInterface)

public:
    typedef QSharedPointer<ModemLocationInterface> Ptr;
    typedef QList<Ptr> List;
    enum Capability { Unknown = 0x0, GpsNmea = 0x1, GpsLacCi = 0x2, GpsRaw = 0x4 };

    typedef QList<QMap<Capability, QVariant> > LocationInformationMap;

    ModemLocationInterface(const QString & path, QObject * parent);
    ~ModemLocationInterface();

    void enableLocation(bool enable, bool signalLocation);
    ModemManager::ModemLocationInterface::LocationInformationMap getLocation();
    ModemManager::ModemLocationInterface::Capability getCapability() const;
    bool enabled() const;
    bool signalsLocation() const;

Q_SIGNALS:
    void capabilitiesChanged(const ModemManager::ModemLocationInterface::Capability capability);
    void enabledChanged(bool enabled);
    void signalsLocationChanged(bool signalsLocation);
    void locationChanged(const ModemManager::ModemLocationInterface::LocationInformationMap & location);

private Q_SLOTS:
   void propertiesChanged(const QString & interface, const QVariantMap & properties);
};

} // namespace ModemManager

#endif

