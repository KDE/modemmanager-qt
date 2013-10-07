/*
Copyright 2008,2011 Will Stephenson <wstephenson@kde.org>
Copyright 2010-2011 Lamarque Souza <lamarque@kde.org>
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

#ifndef MODEMMANAGER_SIM_H
#define MODEMMANAGER_SIM_H

#include "ModemManagerQt-export.h"

#include <QObject>
#include <QSharedPointer>
#include <QDBusPendingReply>

class SimPrivate;

namespace ModemManager
{
class MODEMMANAGERQT_EXPORT Sim : public QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(Sim)

public:
    typedef QSharedPointer<Sim> Ptr;
    typedef QList<Ptr> List;

    explicit Sim(const QString &path, QObject *parent = 0);
    ~Sim();

    // properties
    QString simIdentifier() const;
    QString imsi() const;
    QString operatorIdentifier() const;
    QString operatorName() const;

    // methods
    QDBusPendingReply<> sendPin(const QString &pin);
    QDBusPendingReply<> sendPuk(const QString &puk, const QString &pin);
    QDBusPendingReply<> enablePin(const QString &pin, bool enabled);
    QDBusPendingReply<> changePin(const QString &oldPin, const QString &newPin);

    QString uni() const;
private:
    SimPrivate * d_ptr;
};
} // namespace ModemManager

#endif
