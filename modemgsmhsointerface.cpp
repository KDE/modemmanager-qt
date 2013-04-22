/*
Copyright 2008,2011 Will Stephenson <wstephenson@kde.org>
Copyright 2010 Lamarque Souza <lamarque@kde.org>

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

#include "modemgsmhsointerface.h"
#include "modemgsmhsointerface_p.h"
#include "manager.h"

#include <QDebug>

ModemGsmHsoInterfacePrivate::ModemGsmHsoInterfacePrivate(const QString &path, QObject *owner)
    : ModemInterfacePrivate(path, owner), modemGsmHsoIface(ModemManager::DBUS_SERVICE, path, QDBusConnection::systemBus())
{
}

ModemManager::ModemGsmHsoInterface::ModemGsmHsoInterface(const QString & path, QObject * parent)
    : ModemInterface(*new ModemGsmHsoInterfacePrivate(path, this), parent)
{
}

ModemManager::ModemGsmHsoInterface::~ModemGsmHsoInterface()
{
}

void ModemManager::ModemGsmHsoInterface::authenticate(const QString & username, const QString & password)
{
    Q_D(ModemGsmHsoInterface);
    d->modemGsmHsoIface.Authenticate(username, password);
}
