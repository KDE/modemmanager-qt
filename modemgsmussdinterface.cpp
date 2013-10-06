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

#include "modemgsmussdinterface.h"
#include "modemgsmussdinterface_p.h"
#include "mmdebug.h"
#include "dbus/dbus.h"

Modem3gppUssdInterfacePrivate::Modem3gppUssdInterfacePrivate(const QString &path)
    : InterfacePrivate(path)
    , ussdIface(MM_DBUS_SERVICE, path, QDBusConnection::systemBus())
{
}

ModemManager::Modem3gppUssdInterface::Modem3gppUssdInterface(const QString &path, QObject *parent)
    : Interface(*new Modem3gppUssdInterfacePrivate(path), parent)
{
    Q_D(Modem3gppUssdInterface);

    QDBusConnection::systemBus().connect(MM_DBUS_SERVICE, d->uni, DBUS_INTERFACE_PROPS, "PropertiesChanged", this,
                                         SLOT(onPropertiesChanged(QString,QVariantMap,QStringList)));
}

ModemManager::Modem3gppUssdInterface::~Modem3gppUssdInterface()
{
    Q_D(Modem3gppUssdInterface);
    delete d;
}

void ModemManager::Modem3gppUssdInterface::onPropertiesChanged(const QString &interface, const QVariantMap &properties, const QStringList &invalidatedProps)
{
    mmDebug() << interface << properties.keys();

    if (interface == QString(MM_DBUS_INTERFACE_MODEM_MODEM3GPP_USSD)) {
        QLatin1String state(MM_MODEM_MODEM3GPP_USSD_PROPERTY_STATE);
        QLatin1String networkNotification(MM_MODEM_MODEM3GPP_USSD_PROPERTY_NETWORKNOTIFICATION);
        QLatin1String networkRequest(MM_MODEM_MODEM3GPP_USSD_PROPERTY_NETWORKREQUEST);

        QVariantMap::const_iterator it = properties.constFind(state);
        if ( it != properties.constEnd()) {
            emit stateChanged((MMModem3gppUssdSessionState)it->toUInt());
        }
        it = properties.constFind(networkNotification);
        if ( it != properties.constEnd()) {
            emit networkNotificationChanged(it->toString());
        }
        it = properties.constFind(networkRequest);
        if ( it != properties.constEnd()) {
            emit networkRequestChanged(it->toString());
        }
    }
}

QString ModemManager::Modem3gppUssdInterface::initiate(const QString &command)
{
    Q_D(Modem3gppUssdInterface);
    QDBusReply<QString> reply = d->ussdIface.Initiate(command);

    if (reply.isValid()) {
        return reply.value();
    }
    return QString();
}

QString ModemManager::Modem3gppUssdInterface::respond(const QString &response)
{
    Q_D(Modem3gppUssdInterface);
    QDBusReply<QString> reply = d->ussdIface.Respond(response);

    if (reply.isValid()) {
        return reply.value();
    }
    return QString();
}

void ModemManager::Modem3gppUssdInterface::cancel()
{
    Q_D(Modem3gppUssdInterface);
    d->ussdIface.Cancel();
}

MMModem3gppUssdSessionState ModemManager::Modem3gppUssdInterface::state() const
{
    Q_D(const Modem3gppUssdInterface);
    return (MMModem3gppUssdSessionState)d->ussdIface.state();
}

QString ModemManager::Modem3gppUssdInterface::networkNotification() const
{
    Q_D(const Modem3gppUssdInterface);
    return d->ussdIface.networkNotification();
}

QString ModemManager::Modem3gppUssdInterface::networkRequest() const
{
    Q_D(const Modem3gppUssdInterface);
    return d->ussdIface.networkRequest();
}
