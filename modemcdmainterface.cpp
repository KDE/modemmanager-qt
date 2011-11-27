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

#include "modemcdmainterface.h"
#include "modemcdmainterface_p.h"
#include "manager.h"
#include "mmdebug.h"

ModemCdmaInterfacePrivate::ModemCdmaInterfacePrivate(const QString &path, QObject *owner)
    : ModemInterfacePrivate(path, owner), modemCdmaIface(ModemManager::DBUS_SERVICE, path, QDBusConnection::systemBus())
{
}

ModemManager::ModemCdmaInterface::ModemCdmaInterface(const QString & path, QObject * parent)
    : ModemInterface(*new ModemCdmaInterfacePrivate(path, this), parent)
{
    Q_D(ModemCdmaInterface);

    connect( &d->modemCdmaIface, SIGNAL(RegistrationStateChanged(ModemManager::ModemCdmaInterface::RegistrationState,
                                                                 ModemManager::ModemCdmaInterface::RegistrationState)),
                this, SIGNAL(registrationStateChanged(ModemManager::ModemCdmaInterface::RegistrationState,
                                                      ModemManager::ModemCdmaInterface::RegistrationState)));
    connect( &d->modemCdmaIface, SIGNAL(SignalQuality(uint)),
                this, SIGNAL(signalQualityChanged(uint)));
}

ModemManager::ModemCdmaInterface::~ModemCdmaInterface()
{

}

uint ModemManager::ModemCdmaInterface::getSignalQuality()
{
    Q_D(ModemCdmaInterface);
    QDBusReply< uint > signalQuality = d->modemCdmaIface.GetSignalQuality();

    if (signalQuality.isValid())
        return signalQuality.value();

    mmDebug() << "Error getting signal quality: " << signalQuality.error().name() << ": " << signalQuality.error().message();
    return 0;
}

QString ModemManager::ModemCdmaInterface::getEsn()
{
    Q_D(ModemCdmaInterface);
    QDBusReply<QString> esn = d->modemCdmaIface.GetEsn();

    if (esn.isValid())
        return esn.value();

    mmDebug() << "Error getting ESN: " << esn.error().name() << ": " << esn.error().message();
    return QString();
}

ModemManager::ModemCdmaInterface::ServingSystemType ModemManager::ModemCdmaInterface::getServingSystem()
{
    Q_D(ModemCdmaInterface);
    QDBusReply<ServingSystemType> servingSystem = d->modemCdmaIface.GetServingSystem();

    if (servingSystem.isValid())
        return servingSystem.value();

    mmDebug() << "Error getting serving system info: " << servingSystem.error().name() << ": " << servingSystem.error().message();
    return ServingSystemType();
}

ModemManager::ModemCdmaInterface::RegistrationStateResult ModemManager::ModemCdmaInterface::getRegistrationState()
{
    Q_D(ModemCdmaInterface);
    QDBusReply<RegistrationStateResult> registrationState = d->modemCdmaIface.GetRegistrationState();

    if (registrationState.isValid())
        return registrationState.value();

    mmDebug() << "Error getting registration state: " << registrationState.error().name() << ": " << registrationState.error().message();
    return RegistrationStateResult();
}


