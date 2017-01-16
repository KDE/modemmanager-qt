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

#ifndef MODEMMANAGERQT_FAKE_MODEM_MODEM_MESSAGING_H
#define MODEMMANAGERQT_FAKE_MODEM_MODEM_MESSAGING_H

#include "generictypes.h"
#include "sms.h"

#include <QObject>

#include <QDBusObjectPath>

class ModemMessaging : public QDBusAbstractAdaptor
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "org.kde.fakemodem.Modem.Messaging")
public:
    explicit ModemMessaging(QObject * parent = nullptr);
    virtual ~ModemMessaging();

    Q_PROPERTY(uint DefaultStorage READ defaultStorage)
    Q_PROPERTY(QList<QDBusObjectPath> Messages READ messages)
    Q_PROPERTY(ModemManager::UIntList SupportedStorages READ supportedStorages)

    uint defaultStorage() const;
    QList<QDBusObjectPath> messages() const;
    ModemManager::UIntList supportedStorages() const;

    /* Not part of dbus interface */
    void addMessage(Sms * sms);
    void setModemPath(const QString &path);
    void setEnableNotifications(bool enable);
    void setDefaultStorage(uint defaultStorage);
    void setSupportedStorages(const ModemManager::UIntList &supportedStorages);

    QVariantMap toMap() const;

public Q_SLOTS: // METHODS
    Q_SCRIPTABLE QDBusObjectPath Create(const QVariantMap &properties);
    Q_SCRIPTABLE void Delete(const QDBusObjectPath &path);
    Q_SCRIPTABLE QList<QDBusObjectPath> List();

Q_SIGNALS: // SIGNALS
    Q_SCRIPTABLE void Added(const QDBusObjectPath &path, bool received);
    Q_SCRIPTABLE void Deleted(const QDBusObjectPath &path);

private:
    QString m_modemPath;
    bool m_enabledNotifications;
    int m_messageCounter;
    uint m_defaultStorage;
    QMap<QDBusObjectPath, Sms*> m_messages;
    ModemManager::UIntList m_supportedStorages;
};

#endif
