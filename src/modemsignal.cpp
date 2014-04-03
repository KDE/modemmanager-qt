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

#include "modemsignal.h"
#include "modemsignal_p.h"

ModemSignalPrivate::ModemSignalPrivate(const QString &path)
    : InterfacePrivate(path)
    , modemSignalIface(MM_DBUS_SERVICE, path, QDBusConnection::systemBus())
{
}

ModemManager::ModemSignal::ModemSignal(const QString &path, QObject *parent)
    : Interface(*new ModemSignalPrivate(path), parent)
{
    Q_D(ModemSignal);
}

ModemManager::ModemSignal::~ModemSignal()
{
}

uint ModemManager::ModemSignal::rate() const
{
    Q_D(const ModemSignal);
    return d->modemSignalIface.rate();
}

QVariantMap ModemManager::ModemSignal::cdma() const
{
    Q_D(const ModemSignal);
    return d->modemSignalIface.cdma();
}

QVariantMap ModemManager::ModemSignal::evdo() const
{
    Q_D(const ModemSignal);
    return d->modemSignalIface.evdo();
}

QVariantMap ModemManager::ModemSignal::gsm() const
{
    Q_D(const ModemSignal);
    return d->modemSignalIface.gsm();
}

QVariantMap ModemManager::ModemSignal::lte() const
{
    Q_D(const ModemSignal);
    return d->modemSignalIface.lte();
}

QVariantMap ModemManager::ModemSignal::umts() const
{
    Q_D(const ModemSignal);
    return d->modemSignalIface.umts();
}

void ModemManager::ModemSignal::setup(uint rate)
{
    Q_D(ModemSignal);
    d->modemSignalIface.Setup(rate);
}
