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

#ifndef MODEMMANAGER_MODEM3GPP_H
#define MODEMMANAGER_MODEM3GPP_H

#include "ModemManagerQt-export.h"

#include <QObject>
#include <QSharedPointer>

#include "generic-types.h"
#include "interface.h"

class Modem3gppPrivate;

namespace ModemManager
{
class MODEMMANAGERQT_EXPORT Modem3gpp : public Interface
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(Modem3gpp)
    Q_FLAGS(MMModem3gppFacility)

public:
    typedef QSharedPointer<Modem3gpp> Ptr;
    typedef QList<Ptr> List;

    Q_DECLARE_FLAGS(FacilityLocks, MMModem3gppFacility)

    explicit Modem3gpp(const QString &path, QObject *parent = 0);
    ~Modem3gpp();

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
      * @param networkId The operator ID (ie, "MCCMNC", like "310260") to register. An empty string can be used to register to the home network.
    */
    void registerToNetwork(const QString &networkId = QString());

    QDBusPendingReply<ScanResultsType> scan();

Q_SIGNALS:
    void registrationStateChanged(MMModem3gppRegistrationState registrationState);
    void enabledFacilityLocksChanged(FacilityLocks locks);

private Q_SLOTS:
    void onPropertiesChanged(const QString &interface, const QVariantMap &properties, const QStringList &invalidatedProps);
};

Q_DECLARE_OPERATORS_FOR_FLAGS(Modem3gpp::FacilityLocks)

} // namespace ModemManager

#endif
