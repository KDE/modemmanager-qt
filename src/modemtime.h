/*
    Copyright 2013 Lukas Tinkl <ltinkl@redhat.com>
    Copyright 2013-2015 Jan Grulich <jgrulich@redhat.com>

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

#ifndef MODEMMANAGERQT_MODEMTIME_H
#define MODEMMANAGERQT_MODEMTIME_H

#include <modemmanagerqt_export.h>
#include "interface.h"

#include <QObject>
#include <QDateTime>
#include <QSharedPointer>

namespace ModemManager
{

class ModemTimePrivate;

/**
 * This class represents the timezone data provided by the network
 */
class MODEMMANAGERQT_EXPORT NetworkTimezone
{
public:
    /**
     * Constructs an empty timezone data object
     */
    NetworkTimezone();

    /**
     * Destroys this NetworkTimezone object.
     */
    ~NetworkTimezone();

    /**
     * Constructs an NetworkTimezone object that is a copy of the object @p other.
     */
    NetworkTimezone(const NetworkTimezone &other);

    /**
     * Returns offset of the timezone from UTC, in minutes (including DST, if applicable)
     */
    int offset() const;

    /**
     * Sets offset of the timezone from UTC
     */
    void setOffset(int offset);

    /**
     * Returns amount of offset that is due to DST (daylight saving time)
     */
    int dstOffset() const;

    /**
     * Sets amount of offset that is due to DST
     */
    void setDstOffset(int dstOffset);

    /**
     * Returns number of leap seconds included in the network time
     */
    int leapSecond() const;

    /**
     * Sets number of leap seconds included in the network timezone
     */
    void setLeapSecond(int leapSecond);
    /**
     * Makes a copy of the NetworkTimezone object @p other.
     */
    NetworkTimezone &operator=(const NetworkTimezone &other);

private:
    class Private;
    Private *const d;
};

/**
 * @brief The ModemTime class
 *
 * This class allows clients to receive network time and timezone updates broadcast by mobile networks.
 */
class MODEMMANAGERQT_EXPORT ModemTime : public Interface
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(ModemTime)

public:
    typedef QSharedPointer<ModemTime> Ptr;
    typedef QList<Ptr> List;

    explicit ModemTime(const QString &path, QObject *parent = 0);
    ~ModemTime();

    /**
     * @return the current network time in local time.
     *
     * This method will only work if the modem tracks, or can request, the
     * current network time; it will not attempt to use previously-received
     * network time updates on the host to guess the current network time.
     */
    QDBusPendingReply<QString> networkTime();

    /**
     * @return the timezone data provided by the network.
     * @see NetworkTimezone
     */
    ModemManager::NetworkTimezone networkTimezone() const;

Q_SIGNALS:
    /**
     * Sent when the network time is updated.
     * @param dateTime the new date and time
     */
    void networkTimeChanged(const QDateTime &dateTime);
    void networkTimezoneChanged(const ModemManager::NetworkTimezone &timeZone);
};

} // namespace ModemManager

Q_DECLARE_METATYPE(ModemManager::NetworkTimezone)

#endif
