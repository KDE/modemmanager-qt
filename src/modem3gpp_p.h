/*
    SPDX-FileCopyrightText: 2008 Will Stephenson <wstephenson@kde.org>
    SPDX-FileCopyrightText: 2010 Lamarque Souza <lamarque@kde.org>
    SPDX-FileCopyrightText: 2013-2015 Jan Grulich <jgrulich@redhat.com>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef MODEMMANAGER_MODEM3GPP_P_H
#define MODEMMANAGER_MODEM3GPP_P_H

#include "dbus/modem3gppinterface.h"
#include "interface_p.h"
#include "modem3gpp.h"

namespace ModemManager
{
class Modem3gppPrivate : public InterfacePrivate
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
    void onPropertiesChanged(const QString &interface, const QVariantMap &properties, const QStringList &invalidatedProps) override;
};

} // namespace ModemManager
#endif
