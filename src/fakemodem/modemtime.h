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

#ifndef MODEMMANAGERQT_FAKE_MODEM_MODEM_TIME_H
#define MODEMMANAGERQT_FAKE_MODEM_MODEM_TIME_H

#include "generictypes.h"

#include <QObject>

#include <QDBusObjectPath>

class ModemTime : public QDBusAbstractAdaptor
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "org.kde.fakemodem.Modem.Time")
public:
    explicit ModemTime(QObject * parent = 0);
    virtual ~ModemTime();

    Q_PROPERTY(QVariantMap NetworkTimezone READ networkTimezone)

    QVariantMap networkTimezone() const;

    /* Not part of dbus interface */
    void setModemPath(const QString &path);
    void setEnableNotifications(bool enable);
    void setNetworkTimezone(const QVariantMap &timezone);

    QVariantMap toMap() const;

public Q_SLOTS:
    Q_SCRIPTABLE QString GetNetworkTime();

Q_SIGNALS: // SIGNALS
    Q_SCRIPTABLE void NetworkTimeChanged(const QString &time);

private:
    QString m_modemPath;
    bool m_enabledNotifications;
    QVariantMap m_timezone;
};

#endif
