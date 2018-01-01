/*
    Copyright 2008 Will Stephenson <wstephenson@kde.org>
    Copyright 2010 Lamarque Souza <lamarque@kde.org>
    Copyright 2013-2015 Jan Grulich <jgrulich@redhat.com>

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

#ifndef MODEMMANAGER_MODEM3GPP_P_H
#define MODEMMANAGER_MODEM3GPP_P_H

#include "dbus/modem3gppinterface.h"
#include "interface_p.h"
#include "modem3gpp.h"

namespace ModemManager
{

class Modem3gppPrivate: public InterfacePrivate
{
public:
    explicit Modem3gppPrivate(const QString &path, Modem3gpp *q);
    OrgFreedesktopModemManager1ModemModem3gppInterface modem3gppIface;

    QString imei;
    MMModem3gppRegistrationState registrationState;
    QString operatorCode;
    QString operatorName;
    QFlags<MMModem3gppFacility> enabledFacilityLocks;
#if MM_CHECK_VERSION(1, 2, 0)
    MMModem3gppSubscriptionState subscriptionState;
#endif

    Q_DECLARE_PUBLIC(Modem3gpp)
    Modem3gpp *q_ptr;
private Q_SLOTS:
    void onPropertiesChanged(const QString &interface, const QVariantMap &properties, const QStringList &invalidatedProps) Q_DECL_OVERRIDE;
};

} // namespace ModemManager
#endif
