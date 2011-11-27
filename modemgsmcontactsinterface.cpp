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

#include "modemgsmcontactsinterface.h"
#include "modemgsmcontactsinterface_p.h"
#include "manager.h"

#include <QDebug>

ModemGsmContactsInterfacePrivate::ModemGsmContactsInterfacePrivate(const QString &path, QObject *owner)
    : ModemInterfacePrivate(path, owner), modemGsmContactsIface(ModemManager::DBUS_SERVICE, path, QDBusConnection::systemBus())
{
}

ModemManager::ModemGsmContactsInterface::ModemGsmContactsInterface(const QString & path, QObject * parent)
    : ModemInterface(*new ModemGsmContactsInterfacePrivate(path, this), parent)
{
}

ModemManager::ModemGsmContactsInterface::~ModemGsmContactsInterface()
{
}

int ModemManager::ModemGsmContactsInterface::addContact(const QString & name, const QString & number)
{
    Q_D(ModemGsmContactsInterface);
    QDBusReply<uint> index = d->modemGsmContactsIface.Add(name, number);

    if (index.isValid()) {
        return index.value();
    }
    return -1;
}

void ModemManager::ModemGsmContactsInterface::deleteContact(const int index)
{
    Q_D(ModemGsmContactsInterface);
    d->modemGsmContactsIface.Delete(index);
}

ModemManager::ModemGsmContactsInterface::ContactType ModemManager::ModemGsmContactsInterface::get(const int index)
{
    Q_D(ModemGsmContactsInterface);
    QDBusReply<ContactType> contact = d->modemGsmContactsIface.Get(index);

    if (contact.isValid()) {
        return contact.value();
    }

    return ContactType();
}

ModemManager::ModemGsmContactsInterface::ContactTypeList ModemManager::ModemGsmContactsInterface::list()
{
    Q_D(ModemGsmContactsInterface);
    QDBusReply<ContactTypeList> contacts = d->modemGsmContactsIface.List();

    if (contacts.isValid()) {
        return contacts.value();
    }

    return ContactTypeList();
}

ModemManager::ModemGsmContactsInterface::ContactTypeList ModemManager::ModemGsmContactsInterface::find(const QString & pattern)
{
    Q_D(ModemGsmContactsInterface);
    QDBusReply<ContactTypeList > contacts = d->modemGsmContactsIface.Find(pattern);

    if (contacts.isValid()) {
        return contacts.value();
    }

    return ContactTypeList();
}

int ModemManager::ModemGsmContactsInterface::getCount()
{
    Q_D(ModemGsmContactsInterface);
    QDBusReply<uint> count = d->modemGsmContactsIface.GetCount();

    if (count.isValid()) {
        return count.value();
    }

    return 0;
}



