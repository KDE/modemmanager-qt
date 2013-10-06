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

#include "modemcdmainterface.h"
#include "modemcdmainterface_p.h"
#include "mmdebug.h"

ModemCdmaInterfacePrivate::ModemCdmaInterfacePrivate(const QString &path)
    : InterfacePrivate(path)
    , modemCdmaIface(MM_DBUS_SERVICE, path, QDBusConnection::systemBus())
{
}

ModemManager::ModemCdmaInterface::ModemCdmaInterface(const QString &path, QObject *parent)
    : Interface(*new ModemCdmaInterfacePrivate(path), parent)
{
    Q_D(ModemCdmaInterface);

    connect(&d->modemCdmaIface, SIGNAL(ActivationStateChanged(uint,uint,QVariantMap)),
            SLOT(onActivationStateChanged(uint,uint,QVariantMap)));
}

ModemManager::ModemCdmaInterface::~ModemCdmaInterface()
{
}

void ModemManager::ModemCdmaInterface::activate(const QString &carrierCode)
{
    Q_D(ModemCdmaInterface);
    d->modemCdmaIface.Activate(carrierCode);
}

void ModemManager::ModemCdmaInterface::activateManual(const QVariantMap &properties)
{
    Q_D(ModemCdmaInterface);
    d->modemCdmaIface.ActivateManual(properties);
}

MMModemCdmaActivationState ModemManager::ModemCdmaInterface::activationState() const
{
    Q_D(const ModemCdmaInterface);
    return (MMModemCdmaActivationState)d->modemCdmaIface.activationState();
}

QString ModemManager::ModemCdmaInterface::meid() const
{
    Q_D(const ModemCdmaInterface);
    return d->modemCdmaIface.meid();
}

QString ModemManager::ModemCdmaInterface::esn() const
{
    Q_D(const ModemCdmaInterface);
    return d->modemCdmaIface.esn();
}

uint ModemManager::ModemCdmaInterface::sid() const
{
    Q_D(const ModemCdmaInterface);
    return d->modemCdmaIface.sid();
}

uint ModemManager::ModemCdmaInterface::nid() const
{
    Q_D(const ModemCdmaInterface);
    return d->modemCdmaIface.nid();
}

MMModemCdmaRegistrationState ModemManager::ModemCdmaInterface::cdma1xRegistrationState() const
{
    Q_D(const ModemCdmaInterface);
    return (MMModemCdmaRegistrationState)d->modemCdmaIface.cdma1xRegistrationState();
}

MMModemCdmaRegistrationState ModemManager::ModemCdmaInterface::evdoRegistrationState() const
{
    Q_D(const ModemCdmaInterface);
    return (MMModemCdmaRegistrationState)d->modemCdmaIface.evdoRegistrationState();
}

void ModemManager::ModemCdmaInterface::onActivationStateChanged(uint activation_state, uint activation_error, const QVariantMap &status_changes)
{
    emit activationStateChanged((MMModemCdmaActivationState)activation_state, (MMCdmaActivationError)activation_error, status_changes);
}
