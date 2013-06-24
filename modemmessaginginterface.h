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

#ifndef MODEMMANAGER_MODEMMESSAGINGINTERFACE_H
#define MODEMMANAGER_MODEMMESSAGINGINTERFACE_H

#include "ModemManagerQt-export.h"

#include "modeminterface.h"

class ModemMessagingInterfacePrivate;

namespace ModemManager
{
class MODEMMANAGERQT_EXPORT ModemMessagingInterface : public ModemInterface
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(ModemMessagingInterface)

public:
    typedef QSharedPointer<ModemMessagingInterface> Ptr;
    typedef QList<Ptr> List;

    ModemMessagingInterface(const QString & path, QObject * parent);
    ~ModemMessagingInterface();

    // properties
    QList<MMSmsStorage> supportedStorages() const;
    MMSmsStorage defaultStorage() const;

    //methods
    QList<QDBusObjectPath> listMessages();
    void createMessage(const QVariantMap & properties);
    void deleteMessage(const QDBusObjectPath & path);

Q_SIGNALS:
    void messageAdded(const QDBusObjectPath & path, bool received);
    void messageDeleted(const QDBusObjectPath & path);
};

} // namespace ModemManager

#endif
