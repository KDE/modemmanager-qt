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

#ifndef MODEMMANAGERQT_FAKE_MODEM_MODEM_LOCATION_H
#define MODEMMANAGERQT_FAKE_MODEM_MODEM_LOCATION_H

#include "generictypes.h"

#include <QObject>

#include <QDBusObjectPath>

class ModemLocation : public QDBusAbstractAdaptor
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "org.kde.fakemodem.Modem.Location")
public:
    explicit ModemLocation(QObject * parent = nullptr);
    virtual ~ModemLocation();

    Q_PROPERTY(uint Capabilities READ capabilities)
    Q_PROPERTY(uint Enabled READ enabled)
    Q_PROPERTY(ModemManager::LocationInformationMap Location READ location)
    Q_PROPERTY(bool SignalsLocation READ signalsLocation)

    uint capabilities() const;
    uint enabled() const;
    ModemManager::LocationInformationMap location() const;
    bool signalsLocation() const;

    /* Not part of dbus interface */
    void setModemPath(const QString &path);
    void setEnableNotifications(bool enable);
    void setCapabilities(uint capabilities);
    void setEnabled(uint enabled);
    void setLocation(const ModemManager::LocationInformationMap &location);
    void setSignalsLocation(bool signalsLocation);

    QVariantMap toMap() const;

public Q_SLOTS:
    Q_SCRIPTABLE ModemManager::LocationInformationMap GetLocation();
    Q_SCRIPTABLE void Setup(uint sources, bool signal_location);

private:
    QString m_modemPath;
    bool m_enabledNotifications;
    uint m_capabilities;
    uint m_enabled;
    ModemManager::LocationInformationMap m_location;
    bool m_signalsLocation;
};

#endif
