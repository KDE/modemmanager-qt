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

#ifndef MODEMMANAGERQT_FAKE_MODEM_MODEM_3GPP_USSD_H
#define MODEMMANAGERQT_FAKE_MODEM_MODEM_3GPP_USSD_H

#include "generictypes.h"

#include <QObject>

#include <QDBusObjectPath>

class Modem3gppUssd : public QDBusAbstractAdaptor
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "org.kde.fakemodem.Modem.Modem3gpp.Ussd")
public:
    explicit Modem3gppUssd(QObject * parent = nullptr);
    virtual ~Modem3gppUssd();

    Q_PROPERTY(QString NetworkNotification READ networkNotification)
    Q_PROPERTY(QString NetworkRequest READ networkRequest)
    Q_PROPERTY(uint State READ state)

    QString networkNotification() const;
    QString networkRequest() const;
    uint state() const;

    /* Not part of dbus interface */
    void setModemPath(const QString &path);
    void setEnableNotifications(bool enable);
    void setNetworkNotification(const QString &networkNotification);
    void setNetworkRequest(const QString &networkRequest);
    void setState(uint state);

    QVariantMap toMap() const;

public Q_SLOTS:
    Q_SCRIPTABLE void Cancel();
    Q_SCRIPTABLE QString Initiate(const QString &command);
    Q_SCRIPTABLE QString Respond(const QString &response);

private:
    QString m_modemPath;
    bool m_enabledNotifications;
    QString m_networkNotification;
    QString m_networkRequest;
    uint m_state;
};

#endif
