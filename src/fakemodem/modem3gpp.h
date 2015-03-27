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

#ifndef MODEMMANAGERQT_FAKE_MODEM_MODEM_3GPP_H
#define MODEMMANAGERQT_FAKE_MODEM_MODEM_3GPP_H

#include "generictypes.h"

#include <QObject>

#include <QDBusObjectPath>

class Modem3gpp : public QDBusAbstractAdaptor
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "org.kde.fakemodem.Modem.Modem3gpp")
public:
    explicit Modem3gpp(QObject * parent = 0);
    virtual ~Modem3gpp();

    Q_PROPERTY(uint EnabledFacilityLocks READ enabledFacilityLocks)
    Q_PROPERTY(QString Imei READ imei)
    Q_PROPERTY(QString OperatorCode READ operatorCode)
    Q_PROPERTY(QString OperatorName READ operatorName)
    Q_PROPERTY(uint RegistrationState READ registrationState)
    Q_PROPERTY(uint SubscriptionState READ subscriptionState)

    uint enabledFacilityLocks() const;
    QString imei() const;
    QString operatorCode() const;
    QString operatorName() const;
    uint registrationState() const;
    uint subscriptionState() const;

    /* Not part of dbus interface */
    void setModemPath(const QString &path);
    void setEnableNotifications(bool enable);
    void setEnabledFacilityLocks(uint enabledFacilitiesLocks);
    void setImei(const QString &imei);
    void setOperatorCode(const QString &operatorCode);
    void setOperatorName(const QString &operatorName);
    void setRegistrationState(uint registrationState);
    void setSubscriptionState(uint subscriptionState);

    QVariantMap toMap() const;

public Q_SLOTS:
    Q_SCRIPTABLE void Register(const QString &operator_id);
    Q_SCRIPTABLE ModemManager::QVariantMapList Scan();

private:
    QString m_modemPath;
    bool m_enabledNotifications;
    uint m_enabledFacilityLocks;
    QString m_imei;
    QString m_operatorCode;
    QString m_operatorName;
    uint m_registrationState;
    uint m_subscriptionState;
};

#endif
