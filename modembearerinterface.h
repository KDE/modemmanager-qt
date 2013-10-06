/*
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

#ifndef MODEMMANAGER_MODEMBEARERINTERFACE_H
#define MODEMMANAGER_MODEMBEARERINTERFACE_H

#include "ModemManagerQt-export.h"

#include <QObject>
#include <QSharedPointer>

#include "generic-types.h"

class ModemBearerInterfacePrivate;

namespace ModemManager
{
class MODEMMANAGERQT_EXPORT ModemBearerInterface: public QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(ModemBearerInterface)

public:
    typedef QSharedPointer<ModemBearerInterface> Ptr;
    typedef QList<Ptr> List;

    struct IpConfig {
        MMBearerIpMethod method;
        QString address;
        uint prefix;
        QString dns1;
        QString dns2;
        QString dns3;
        QString gateway;
    };

    ModemBearerInterface(const QString &path, QObject *parent = 0);
    ~ModemBearerInterface();

    // properties
    QString interface() const;
    bool isConnected() const;
    bool isSuspended() const;
    IpConfig ip4Config() const;
    IpConfig ip6Config() const;
    uint ipTimeout() const;
    QVariantMap properties() const;

    // methods
    void connectBearer();
    void disconnectBearer();

Q_SIGNALS:
    void interfaceChanged(const QString &iface);
    void connectedChanged(bool connected);
    void suspendedChanged(bool suspended);
    void ip4ConfigChanged();
    void ip6ConfigChanged();

private Q_SLOTS:
   void onPropertiesChanged(const QString &interface, const QVariantMap &properties, const QStringList &invalidatedProps);

protected:
    ModemBearerInterface * d_ptr;
};

} // namespace ModemManager

#endif
