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

#ifndef MODEMMANAGERQT_BEARER_H
#define MODEMMANAGERQT_BEARER_H

#include "ModemManagerQt-export.h"

#include <QObject>
#include <QSharedPointer>

#include "generic-types.h"

class BearerPrivate;

namespace ModemManager
{
/**
 * @brief The Bearer class
 *
 * This class provides access to specific actions that may be performed on available bearers.
 */
class MODEMMANAGERQT_EXPORT Bearer: public QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(Bearer)

public:
    typedef QSharedPointer<Bearer> Ptr;
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

    explicit Bearer(const QString &path, QObject *parent = 0);
    ~Bearer();

    /**
     * @return the operating system name for the network data interface that
     * provides packet data using this bearer.
     *
     * Connection managers must configure this interface depending on the IP
     * "method" given by the ip4Config() or ip6Config() properties set by bearer
     * activation.
     *
     * If MM_BEARER_IP_METHOD_STATIC or MM_BEARER_IP_METHOD_DHCP methods are
     * given, the interface will be an ethernet-style interface suitable for DHCP
     * or setting static IP configuration on, while if the
     * MM_BEARER_IP_METHOD_PPP method is given, the interface will be a serial
     * TTY which must then have PPP run over it.
     *
     */
    QString interface() const;

    /**
     * @return whether or not the bearer is connected and thus whether packet
     * data communication using this bearer is possible.
     */
    bool isConnected() const;

    /**
     * In some devices, packet data service will be suspended while the device
     * is handling other communication, like a voice call. If packet data
     * service is suspended (but not deactivated) this property will be @p true
     */
    bool isSuspended() const;

    /**
     * If the bearer was configured for IPv4 addressing, upon activation
     * this property contains the addressing details for assignment to the data
     * interface.
     */
    IpConfig ip4Config() const;

    /**
     * If the bearer was configured for IPv6 addressing, upon activation this
     * property contains the addressing details for assignment to the data
     * interface.
     */
    IpConfig ip6Config() const;

    /**
     * @return maximum time to wait for a successful IP establishment, when PPP is used.
     */
    uint ipTimeout() const;

    /**
     * @return map of properties used when creating the bearer
     * @see IpConfig
     */
    QVariantMap properties() const;

    /**
     * Requests activation of a packet data connection with the network using
     * this bearer's properties. Upon successful activation, the modem can send
     * and receive packet data and, depending on the addressing capability of
     * the modem, a connection manager may need to start PPP, perform DHCP, or
     * assign the IP address returned by the modem to the data interface. Upon
     * successful return, the ip4Config() and/or ip6Config() properties become
     * valid and may contain IP configuration information for the data interface
     * associated with this bearer.
     */
    void connectBearer();

    /**
     * Disconnect and deactivate this packet data connection.
     *
     * Any ongoing data session will be terminated and IP addresses become invalid when this method is called.
     */
    void disconnectBearer();

    /**
     * @return the DBUS path (uni) to the object
     */
    QString uni() const;

Q_SIGNALS:
    void interfaceChanged(const QString &iface);
    void connectedChanged(bool connected);
    void suspendedChanged(bool suspended);
    void ip4ConfigChanged();
    void ip6ConfigChanged();

private Q_SLOTS:
   void onPropertiesChanged(const QString &interface, const QVariantMap &properties, const QStringList &invalidatedProps);

private:
    BearerPrivate *const d_ptr;
};

} // namespace ModemManager

#endif
