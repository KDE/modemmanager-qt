/*
    Copyright 2014 Lukas Tinkl <ltinkl@redhat.com>

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

#include "modemfirmware.h"
#include "modemfirmware_p.h"
#ifdef MMQT_STATIC
#include "dbus/fakedbus.h"
#else
#include "dbus/dbus.h"
#endif

ModemManager::ModemFirmwarePrivate::ModemFirmwarePrivate(const QString &path, ModemFirmware *q)
    : InterfacePrivate(path, q)
#ifdef MMQT_STATIC
    , modemFirmwareIface(MMQT_DBUS_SERVICE, path, QDBusConnection::sessionBus())
#else
    , modemFirmwareIface(MMQT_DBUS_SERVICE, path, QDBusConnection::systemBus())
#endif
    , q_ptr(q)
{
}

ModemManager::ModemFirmware::ModemFirmware(const QString &path, QObject *parent)
    : Interface(*new ModemFirmwarePrivate(path, this), parent)
{
}

ModemManager::ModemFirmware::~ModemFirmware()
{
}

QDBusPendingReply<QString, QVariantMapList> ModemManager::ModemFirmware::listImages()
{
    Q_D(ModemFirmware);
    return d->modemFirmwareIface.List();
}

void ModemManager::ModemFirmware::selectImage(const QString &uniqueid)
{
    Q_D(ModemFirmware);
    d->modemFirmwareIface.Select(uniqueid);
}
