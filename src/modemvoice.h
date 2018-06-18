/*
    Copyright 2018 Aleksander Morgado <aleksander@aleksander.es>

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

#ifndef MODEMMANAGERQT_MODEMVOICE_H
#define MODEMMANAGERQT_MODEMVOICE_H

#include <modemmanagerqt_export.h>

#include <QObject>
#include <QSharedPointer>

#include "call.h"
#include "generictypes.h"
#include "interface.h"

namespace ModemManager
{

class ModemVoicePrivate;

/**
 * @brief The ModemVoice class
 *
 * The Voice interface handles call related actions
 */
class MODEMMANAGERQT_EXPORT ModemVoice : public Interface
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(ModemVoice)

public:
    typedef QSharedPointer<ModemVoice> Ptr;
    typedef QList<Ptr> List;

    explicit ModemVoice(const QString &path, QObject *parent = nullptr);
    ~ModemVoice();

    /**
     * Retrieve all calls.
     *
     * This method should only be used once and subsequent information retrieved
     * either by listening for the callAdded() signal, or by
     * querying the specific call object of interest using findCall()
     */
    ModemManager::Call::List calls() const;

    /**
     * Creates a new call object.
     * @param number Number property
     */
    QDBusPendingReply<QDBusObjectPath> createCall(const QString &number);

    /**
     * Creates a new call object.
     * @param call QVariantMap containing call properties
     * The 'number' property is mandatory.
     */
    QDBusPendingReply<QDBusObjectPath> createCall(const QVariantMap &call);

    /**
     * Delete a call.
     *
     * @param uni path to the Call object
     */
    QDBusPendingReply<void> deleteCall(const QString &uni);

    /**
     * @param uni path to the Call object
     * @return pointer to the found Call (may be null if not found)
     */
    ModemManager::Call::Ptr findCall(const QString &uni);

Q_SIGNALS:
    /**
     * Emitted when an new Call is detected.
     *
     * @param uni path to the Call object
     */
    void callAdded(const QString &uni);

    /**
     * Emitted when a call has been deleted.
     * @param uni path to the Call object
     */
    void callDeleted(const QString &uni);
};

} // namespace ModemManager

#endif
