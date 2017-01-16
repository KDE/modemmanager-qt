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

#ifndef MODEMMANAGERQT_FAKE_MODEM_MODEMCDMA_H
#define MODEMMANAGERQT_FAKE_MODEM_MODEMCDMA_H

#include "generictypes.h"

#include <QObject>

#include <QDBusObjectPath>

class ModemCdma : public QDBusAbstractAdaptor
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "org.kde.fakemodem.Modem.ModemCdma")
public:
    explicit ModemCdma(QObject * parent = nullptr);
    virtual ~ModemCdma();

    Q_PROPERTY(uint ActivationState READ activationState)
    Q_PROPERTY(uint Cdma1xRegistrationState READ cdma1xRegistrationState)
    Q_PROPERTY(QString Esn READ esn)
    Q_PROPERTY(uint EvdoRegistrationState READ evdoRegistrationState)
    Q_PROPERTY(QString Meid READ meid)
    Q_PROPERTY(uint Nid READ nid)
    Q_PROPERTY(uint Sid READ sid)

    uint activationState() const;
    uint cdma1xRegistrationState() const;
    QString esn() const;
    uint evdoRegistrationState() const;
    QString meid() const;
    uint nid() const;
    uint sid() const;

    /* Not part of dbus interface */
    void setModemPath(const QString &path);
    void setEnableNotifications(bool enable);
    void setActivationState(uint state);
    void setCdma1xRegistrationState(uint state);
    void setEsn(const QString &esn);
    void setEvdoRegistrationState(uint state);
    void setMeid(const QString &meid);
    void setNid(uint nid);
    void setSid(uint sid);

    QVariantMap toMap() const;

public Q_SLOTS:
    void Activate(const QString &carrier_code);
    void ActivateManual(const QVariantMap &properties);

Q_SIGNALS: // SIGNALS
    void ActivationStateChanged(uint activation_state, uint activation_error, const QVariantMap &status_changes);

private:
    QString m_modemPath;
    bool m_enabledNotifications;
    uint m_activationState;
    uint m_cdma1xRegistrationState;
    QString m_esn;
    uint m_evdoRegistrationState;
    QString m_meid;
    uint m_nid;
    uint m_sid;
};

#endif
