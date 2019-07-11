/*
    Copyright 2008,2011 Will Stephenson <wstephenson@kde.org>
    Copyright 2010 Lamarque Souza <lamarque@kde.org>
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

#include "modem3gppussd.h"
#include "modem3gppussd_p.h"
#include "mmdebug_p.h"
#ifdef MMQT_STATIC
#include "dbus/fakedbus.h"
#else
#include "dbus/dbus.h"
#endif

ModemManager::Modem3gppUssdPrivate::Modem3gppUssdPrivate(const QString &path, Modem3gppUssd *q)
    : InterfacePrivate(path, q)
#ifdef MMQT_STATIC
    , ussdIface(QLatin1String(MMQT_DBUS_SERVICE), path, QDBusConnection::sessionBus())
#else
    , ussdIface(QLatin1String(MMQT_DBUS_SERVICE), path, QDBusConnection::systemBus())
#endif
    , q_ptr(q)
{
    if (ussdIface.isValid()) {
        state = (MMModem3gppUssdSessionState)ussdIface.state();
        networkNotification = ussdIface.networkNotification();
        networkRequest = ussdIface.networkRequest();
    }
}

ModemManager::Modem3gppUssd::Modem3gppUssd(const QString &path, QObject *parent)
    : Interface(*new Modem3gppUssdPrivate(path, this), parent)
{
    Q_D(Modem3gppUssd);

    qRegisterMetaType<MMModem3gppUssdSessionState>();

#ifdef MMQT_STATIC
    QDBusConnection::sessionBus().connect(QLatin1String(MMQT_DBUS_SERVICE), d->uni, QLatin1String(DBUS_INTERFACE_PROPS), QStringLiteral("PropertiesChanged"), d,
                                         SLOT(onPropertiesChanged(QString,QVariantMap,QStringList)));
#else
    QDBusConnection::systemBus().connect(QLatin1String(MMQT_DBUS_SERVICE), d->uni, QLatin1String(DBUS_INTERFACE_PROPS), QStringLiteral("PropertiesChanged"), d,
                                         SLOT(onPropertiesChanged(QString,QVariantMap,QStringList)));
#endif
}

ModemManager::Modem3gppUssd::~Modem3gppUssd()
{
}

void ModemManager::Modem3gppUssdPrivate::onPropertiesChanged(const QString &interface, const QVariantMap &properties, const QStringList &invalidatedProps)
{
    Q_Q(Modem3gppUssd);
    Q_UNUSED(invalidatedProps);
    qCDebug(MMQT) << interface << properties.keys();

    if (interface == QLatin1String(MMQT_DBUS_INTERFACE_MODEM_MODEM3GPP_USSD)) {
        QVariantMap::const_iterator it = properties.constFind(QLatin1String(MM_MODEM_MODEM3GPP_USSD_PROPERTY_STATE));
        if ( it != properties.constEnd()) {
            state = (MMModem3gppUssdSessionState)it->toUInt();
            Q_EMIT q->stateChanged(state);
        }
        it = properties.constFind(QLatin1String(MM_MODEM_MODEM3GPP_USSD_PROPERTY_NETWORKNOTIFICATION));
        if ( it != properties.constEnd()) {
            networkNotification = it->toString();
            Q_EMIT q->networkNotificationChanged(networkNotification);
        }
        it = properties.constFind(QLatin1String(MM_MODEM_MODEM3GPP_USSD_PROPERTY_NETWORKREQUEST));
        if ( it != properties.constEnd()) {
            networkRequest = it->toString();
            Q_EMIT q->networkRequestChanged(networkRequest);
        }
    }
}

QDBusPendingReply<QString> ModemManager::Modem3gppUssd::initiate(const QString &command)
{
    Q_D(Modem3gppUssd);
    return d->ussdIface.Initiate(command);
}

QDBusPendingReply<QString> ModemManager::Modem3gppUssd::respond(const QString &response)
{
    Q_D(Modem3gppUssd);
    return d->ussdIface.Respond(response);
}

void ModemManager::Modem3gppUssd::cancel()
{
    Q_D(Modem3gppUssd);
    d->ussdIface.Cancel();
}

MMModem3gppUssdSessionState ModemManager::Modem3gppUssd::state() const
{
    Q_D(const Modem3gppUssd);
    return d->state;
}

QString ModemManager::Modem3gppUssd::networkNotification() const
{
    Q_D(const Modem3gppUssd);
    return d->networkNotification;
}

QString ModemManager::Modem3gppUssd::networkRequest() const
{
    Q_D(const Modem3gppUssd);
    return d->networkRequest;
}

void ModemManager::Modem3gppUssd::setTimeout(int timeout)
{
    Q_D(Modem3gppUssd);
    d->ussdIface.setTimeout(timeout);
}

int ModemManager::Modem3gppUssd::timeout() const
{
    Q_D(const Modem3gppUssd);
    return d->ussdIface.timeout();
}
