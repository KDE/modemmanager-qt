/*
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

#ifndef MODEMMANAGERQT_INTERFACE_H
#define MODEMMANAGERQT_INTERFACE_H

#include "ModemManagerQt-export.h"

#include <QObject>
#include <QVariant>
#include <QSharedPointer>
#include <QDBusObjectPath>

#include "generic-types.h"

class InterfacePrivate;

namespace ModemManager
{
class MODEMMANAGERQT_EXPORT Interface : public QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(Interface)

    Q_PROPERTY(QString uni READ uni)

public:
    typedef QSharedPointer<Interface> Ptr;
    typedef QList<Ptr> List;

    explicit Interface( const QString &path, QObject *parent = 0 );
    Interface( InterfacePrivate &dd, QObject *parent = 0);
    virtual ~Interface();

    QString uni() const;

protected:
    InterfacePrivate * d_ptr;
};
} // namespace ModemManager

#endif
