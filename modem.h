/*
    Copyright 2013 Lukas Tinkl <ltinkl@redhat.com>
    Copyright 2013 Jan Grulich <jgrulich@redhat.com>

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

#ifndef MODEMMANAGERQT_MODEM_H
#define MODEMMANAGERQT_MODEM_H

#include <ModemManager/ModemManager.h>

#include "ModemManagerQt-export.h"

#include <QObject>
#include <QVariant>
#include <QSharedPointer>
#include <QDBusObjectPath>

#include "generic-types.h"
#include "interface.h"

class ModemPrivate;

namespace ModemManager
{
class MODEMMANAGERQT_EXPORT Modem : public QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(Modem)

    Q_PROPERTY(QString uni READ uni)

public:
    typedef QSharedPointer<Modem> Ptr;
    typedef QList<Ptr> List;

    enum InterfaceType {
        ModemInterface,
        GsmInterface,
        GsmUssdInterface,
        CdmaInterface,
        MessagingInterface,
        LocationInterface,
        TimeInterface,
        FirmwareInterface
    };

    explicit Modem( const QString &path, QObject *parent = 0 );
    explicit Modem( ModemPrivate &dd, QObject *parent = 0);
    virtual ~Modem();

    QString uni() const;

    bool hasInterface(InterfaceType type) const;
    ModemManager::Interface::List interfaces() const;
    ModemManager::Interface::Ptr interface(InterfaceType type) const;

    bool isGsmModem() const;
    bool isCdmaModem() const;

private Q_SLOTS:
    void onInterfacesAdded(const QDBusObjectPath &object_path, const NMVariantMapMap &interfaces_and_properties);
    void onInterfacesRemoved(const QDBusObjectPath &object_path, const QStringList &interfaces);
private:
    void init();
    void initInterfaces();
    QString introspect() const;

protected:
    ModemPrivate * d_ptr;
};
} // namespace ModemManager

#endif
