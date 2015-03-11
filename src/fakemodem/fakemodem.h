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

#ifndef MODEMMANAGERQT_FAKE_MODEM_H
#define MODEMMANAGERQT_FAKE_MODEM_H

#include "dbus/fakedbus.h"
#include "modem.h"
#include "objectmanager.h"

#include <QObject>
#include <QDBusObjectPath>
#include <QDBusPendingReply>

class FakeModem : public QObject
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", MMQT_DBUS_SERVICE)
public:
    explicit FakeModem(QObject* parent = 0);
    virtual ~FakeModem();

    /* Not part of DBus interface */
    void addModem(Modem *modem);
    void removeModem(Modem *modem);
public Q_SLOTS:
    Q_SCRIPTABLE void ScanDevices();
    Q_SCRIPTABLE void SetLogging(const QString &level);

private:
    /* Not part of DBus interface */
    int m_modemCounter;
    QMap<QDBusObjectPath, Modem *> m_modems;
    ObjectManager *m_objectManager;
};

#endif

