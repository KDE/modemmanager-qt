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

#ifndef MODEMMANAGER_MODEM3GPPINTERFACE_H
#define MODEMMANAGER_MODEM3GPPINTERFACE_H

#include "ModemManagerQt-export.h"

#include "modeminterface.h"

class Modem3gppInterfacePrivate;

namespace ModemManager
{
class MODEMMANAGERQT_EXPORT Modem3gppInterface : public ModemInterface
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(Modem3gppInterface)
    Q_FLAGS(MMModem3gppFacility)

public:
    typedef QSharedPointer<Modem3gppInterface> Ptr;
    typedef QList<Ptr> List;

    Q_DECLARE_FLAGS(FacilityLocks, MMModem3gppFacility)

    Modem3gppInterface(const QString & path, QObject * parent);
    ~Modem3gppInterface();

    // properties
    QString imei() const;
    MMModem3gppRegistrationState registrationState() const;
    QString operatorCode() const;
    QString operatorName() const;
    FacilityLocks enabledFacilityLocks() const;

    // methods
    /**
      * Register the device to network.
      *
      * @param networkId the network ID to register. An empty string can be used to register to the home network.
    */
    void registerToNetwork(const QString & networkId = QString());

    QDBusPendingReply<ScanResultsType> scan();

Q_SIGNALS:
    void registrationStateChanged(MMModem3gppRegistrationState registrationState);
    void enabledFacilityLocksChanged(FacilityLocks locks);

private Q_SLOTS:
    void onPropertiesChanged(const QString &interface, const QVariantMap &properties, const QStringList &invalidatedProps);
};

Q_DECLARE_OPERATORS_FOR_FLAGS(Modem3gppInterface::FacilityLocks)

} // namespace ModemManager

#endif
