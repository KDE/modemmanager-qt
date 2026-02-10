/*
    SPDX-FileCopyrightText: 2026 Devin Lin <devin@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef MODEMMANAGERQT_MODEMCELLBROADCAST_H
#define MODEMMANAGERQT_MODEMCELLBROADCAST_H

#include <modemmanagerqt_export.h>

#include <QDBusPendingReply>
#include <QObject>
#include <QSharedPointer>

#include "cbm.h"
#include "generictypes.h"
#include "interface.h"

namespace ModemManager
{
class ModemCellBroadcastPrivate;

/*!
 * \class ModemManager::ModemCellBroadcast
 *
 * \inheaderfile ModemManagerQt/ModemCellBroadcast
 *
 * \inmodule ModemManagerQt
 *
 * \brief The ModemCellBroadcast class.
 *
 * This interface allows clients to configure the details of CellBroadcast
 * service information.
 *
 * This interface will only be available once the modem is ready to be
 * registered in the cellular network.
 *
 * \since 6.24.0
 */
class MODEMMANAGERQT_EXPORT ModemCellBroadcast : public Interface
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(ModemCellBroadcast)

public:
    /*!
     * \typedef ModemManager::ModemCellBroadcast::Ptr
     */
    typedef QSharedPointer<ModemCellBroadcast> Ptr;
    /*!
     * \typedef ModemManager::ModemCellBroadcast::List
     */
    typedef QList<Ptr> List;

    /*!
     */
    explicit ModemCellBroadcast(const QString &path, QObject *parent = nullptr);
    ~ModemCellBroadcast() override;

    /*!
     * Returns the list of CBM object paths.
     */
    ModemManager::Cbm::List cellBroadcasts() const;

    /*!
     * Retrieve a specific cell broadcast message by its D-Bus object path.
     *
     * \a uni the object path of the CBM to retrieve
     */
    ModemManager::Cbm::Ptr findCellBroadcast(const QString &uni);

    /*!
     * Delete a cell broadcast message.
     *
     * \a uni the object path of the CBM to delete
     */
    QDBusPendingReply<void> deleteCellBroadcast(const QString &uni);

    /*!
     * Set the list of channels to receive Cell Broadcasts for.
     *
     * \a channels the list of channel ranges
     */
    QDBusPendingReply<void> setChannels(const ModemManager::CellBroadcastChannelRangeList &channels);

    /*!
     * Returns the list of channels that cell broadcast messages are
     * received for.
     */
    ModemManager::CellBroadcastChannelRangeList channels() const;

    /*!
     * Sets the timeout in milliseconds for all async method DBus calls.
     * -1 means the default DBus timeout (usually 25 seconds).
     */
    void setTimeout(int timeout);

    /*!
     * Returns the current value of the DBus timeout in milliseconds.
     * -1 means the default DBus timeout (usually 25 seconds).
     */
    int timeout() const;

Q_SIGNALS:
    /*!
     * Emitted when any part of a new cell broadcast message has been received.
     * Not all parts may have been received and the message may not be complete.
     * Check the State property to determine if the message is complete.
     *
     * \a uni object path of the new CBM
     */
    void cellBroadcastAdded(const QString &uni);

    /*!
     * Emitted when a message has been deleted.
     *
     * \a uni object path of the now deleted CBM
     */
    void cellBroadcastDeleted(const QString &uni);

    /*!
     */
    void channelsChanged(const ModemManager::CellBroadcastChannelRangeList &channels);
};

} // namespace ModemManager

#endif
