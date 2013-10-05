/*
Copyright 2008,2011 Will Stephenson <wstephenson@kde.org>
Copyright 2010 Lamarque Souza <lamarque@kde.org>
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

#ifndef MODEMMANAGERQT_MODEMGSMUSSDINTERFACE_H
#define MODEMMANAGERQT_MODEMGSMUSSDINTERFACE_H

#include "ModemManagerQt-export.h"

#include <QObject>
#include <QSharedPointer>

#include "generic-types.h"
#include "interface.h"

class Modem3gppUssdInterfacePrivate;

namespace ModemManager
{
class MODEMMANAGERQT_EXPORT Modem3gppUssdInterface : public Interface
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(Modem3gppUssdInterface)

public:
    typedef QSharedPointer<Modem3gppUssdInterface> Ptr;
    typedef QList<Ptr> List;

    explicit Modem3gppUssdInterface(const QString & path, QObject * parent = 0);
    ~Modem3gppUssdInterface();

    // methods
    QString initiate(const QString & command);
    QString respond(const QString &response);
    void cancel();

    // properties
    MMModem3gppUssdSessionState state() const;
    QString networkNotification() const;
    QString networkRequest() const;

Q_SIGNALS:
    // properties
    void stateChanged(MMModem3gppUssdSessionState state);
    void networkNotificationChanged(const QString & networkNotification);
    void networkRequestChanged(const QString & networkRequest);

private Q_SLOTS:
    void onPropertiesChanged(const QString &interface, const QVariantMap &properties, const QStringList &invalidatedProps);
};

} // namespace ModemManager

#endif
