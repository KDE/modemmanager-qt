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

#ifndef MODEMMANAGERQT_FAKE_MODEM_BEARER_H
#define MODEMMANAGERQT_FAKE_MODEM_BEARER_H

#include "generictypes.h"

#include <QObject>

#include <QDBusObjectPath>

class Bearer : public QObject
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "org.kde.fakemodem.Bearer")
public:
    explicit Bearer(QObject * parent = 0);
    virtual ~Bearer();

    Q_PROPERTY(bool Connected READ connected)
    Q_PROPERTY(QString Interface READ interface)
    Q_PROPERTY(QVariantMap Ip4Config READ ip4Config)
    Q_PROPERTY(QVariantMap Ip6Config READ ip6Config)
    Q_PROPERTY(uint IpTimeout READ ipTimeout)
    Q_PROPERTY(QVariantMap Properties READ properties)
    Q_PROPERTY(bool Suspended READ suspended)

    bool connected() const;
    QString interface() const;
    QVariantMap ip4Config() const;
    QVariantMap ip6Config() const;
    uint ipTimeout() const;
    QVariantMap properties() const;
    bool suspended() const;

    /* Not part of dbus interface */
    QString bearerPath() const;
    void setEnableNotifications(bool enable);
    void setBearerPath(const QString &path);
    void setConnected(bool connected);
    void setInterface(const QString &interface);
    void setIp4Config(const QVariantMap &config);
    void setIp6Config(const QVariantMap &config);
    void setIpTimeout(uint timeout);
    void setProperties(const QVariantMap &properties);
    void setSuspended(bool suspended);

public Q_SLOTS:
    Q_SCRIPTABLE void Connect();
    Q_SCRIPTABLE void Disconnect();

private:
    QString m_bearerPath;
    bool m_connected;
    bool m_enabledNotifications;
    QString m_interface;
    QVariantMap m_ip4Config;
    QVariantMap m_ip6Config;
    uint m_ipTimeout;
    QVariantMap m_properties;
    bool m_suspended;
};

#endif
