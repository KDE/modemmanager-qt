/*
Copyright 2008,2011 Will Stephenson <wstephenson@kde.org>
Copyright 2010 Lamarque Souza <lamarque@kde.org>

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 2 of
the License or (at your option) version 3 or any later version
accepted by the membership of KDE e.V. (or its successor approved
by the membership of KDE e.V.), which shall act as a proxy
defined in Section 14 of version 3 of the license.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "manager.h"
#include "modemgsmussdinterface.h"
#include "modemgsmussdinterface_p.h"

#include "mmdebug.h"

ModemGsmUssdInterfacePrivate::ModemGsmUssdInterfacePrivate(const QString &path, QObject *owner)
    : ModemInterfacePrivate(path, owner), modemGsmUssdIface(ModemManager::DBUS_SERVICE, path, QDBusConnection::systemBus())
{
}

ModemManager::ModemGsmUssdInterface::ModemGsmUssdInterface(const QString & path, QObject * parent)
    : ModemInterface(*new ModemGsmUssdInterfacePrivate(path, this), parent)
{
    Q_D(ModemGsmUssdInterface);

    d->modemGsmUssdIface.connection().connect(ModemManager::DBUS_SERVICE,
        path, QLatin1String("org.freedesktop.DBus.Properties"),
        QLatin1String("MmPropertiesChanged"), QLatin1String("sa{sv}"),
        this, SLOT(propertiesChanged(const QString &,const QVariantMap &)));
}

ModemManager::ModemGsmUssdInterface::~ModemGsmUssdInterface()
{

}

void ModemManager::ModemGsmUssdInterface::propertiesChanged(const QString & interface, const QVariantMap & properties)
{
    mmDebug() << interface << properties.keys();

    if (interface == QString("org.freedesktop.ModemManager.Modem.Gsm.Ussd")) {
        QLatin1String state("State");
        QLatin1String networkNotification("NetworkNotification");
        QLatin1String networkRequest("NetworkRequest");

        QVariantMap::const_iterator it = properties.find(state);
        if ( it != properties.end()) {
            emit stateChanged(it->toString());
        }
        it = properties.find(networkNotification);
        if ( it != properties.end()) {
            emit networkNotificationChanged(it->toString());
        }
        it = properties.find(networkRequest);
        if ( it != properties.end()) {
            emit networkRequestChanged(it->toString());
        }
    }
}

QString ModemManager::ModemGsmUssdInterface::initiate(const QString & command)
{
    Q_D(ModemGsmUssdInterface);
    QDBusReply<QString> reply = d->modemGsmUssdIface.Initiate(command);

    if (reply.isValid()) {
        return reply.value();
    }
    return QString();
}

void ModemManager::ModemGsmUssdInterface::respond(const QString response)
{
    Q_D(ModemGsmUssdInterface);
    d->modemGsmUssdIface.Respond(response);
}

void ModemManager::ModemGsmUssdInterface::cancel()
{
    Q_D(ModemGsmUssdInterface);
    d->modemGsmUssdIface.Cancel();
}

QString ModemManager::ModemGsmUssdInterface::getState()
{
    Q_D(const ModemGsmUssdInterface);
    return d->modemGsmUssdIface.state();
}

QString ModemManager::ModemGsmUssdInterface::getNetworkNotification()
{
    Q_D(const ModemGsmUssdInterface);
    return d->modemGsmUssdIface.networkNotification();
}

QString ModemManager::ModemGsmUssdInterface::getNetworkRequest()
{
    Q_D(const ModemGsmUssdInterface);
    return d->modemGsmUssdIface.networkRequest();
}



