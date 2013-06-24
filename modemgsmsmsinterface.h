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

#ifndef MODEMMANAGER_MODEMSMSINTERFACE_H
#define MODEMMANAGER_MODEMSMSINTERFACE_H

#include "ModemManagerQt-export.h"
#include "modeminterface.h"

#include <QObject>
#include <QSharedPointer>
#include <QDateTime>
#include <QPair>

class ModemSmsInterfacePrivate;

namespace ModemManager
{
class MODEMMANAGERQT_EXPORT ModemSmsInterface : public ModemInterface
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(ModemSmsInterface)

public:
    typedef QSharedPointer<ModemSmsInterface> Ptr;
    typedef QList<Ptr> List;

    ModemSmsInterface(const QString & path, QObject * parent);
    ~ModemSmsInterface();

    // properties
    MMSmsState state() const;
    MMSmsPduType pduType() const;
    QString number() const;
    QString text() const;
    QByteArray data() const;
    QString smsc() const;
    ValidityPair validity() const;
    int smsClass() const;
    bool deliveryReportRequest() const;
    uint messageReference() const;
    QDateTime timestamp() const;
    QDateTime dischargeTimestamp() const;
    MMSmsDeliveryState deliveryState() const;
    MMSmsStorage storage() const;

    // methods
    void send();
    void store(MMSmsStorage storage);
};

} // namespace ModemManager

#endif
