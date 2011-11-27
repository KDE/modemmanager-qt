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

#include "modemgsmsmsinterface.h"
#include "modemgsmsmsinterface_p.h"
#include "manager.h"

#include <QDebug>

ModemGsmSmsInterfacePrivate::ModemGsmSmsInterfacePrivate(const QString &path, QObject *owner)
    : ModemInterfacePrivate(path, owner), modemGsmSmsIface(ModemManager::DBUS_SERVICE, path, QDBusConnection::systemBus())
{
}

ModemManager::ModemGsmSmsInterface::ModemGsmSmsInterface(const QString & path, QObject * parent)
    : ModemInterface(*new ModemGsmSmsInterfacePrivate(path, this), parent)
{
    Q_D(const ModemGsmSmsInterface);
    connect(&d->modemGsmSmsIface, SIGNAL(smsReceived(uint,bool)),
                this, SIGNAL(smsReceived(int,bool)));
    connect(&d->modemGsmSmsIface, SIGNAL(completed(uint,bool)),
                this, SIGNAL(completed(int,bool)));
}

ModemManager::ModemGsmSmsInterface::~ModemGsmSmsInterface()
{
}

void ModemManager::ModemGsmSmsInterface::deleteSms(const int index)
{
    Q_D(ModemGsmSmsInterface);
    d->modemGsmSmsIface.Delete(index);
}

QVariantMap ModemManager::ModemGsmSmsInterface::get(const int index)
{
    Q_D(ModemGsmSmsInterface);
    QDBusReply<QVariantMap> sms = d->modemGsmSmsIface.Get(index);

    if (sms.isValid()) {
        return sms.value();
    }

    return QVariantMap();
}

int ModemManager::ModemGsmSmsInterface::getFormat()
{
    Q_D(ModemGsmSmsInterface);
    QDBusReply<uint> format = d->modemGsmSmsIface.GetFormat();

    if (format.isValid()) {
        return format.value();
    }

    return 0;
}

void ModemManager::ModemGsmSmsInterface::setFormat(const int format)
{
    Q_D(ModemGsmSmsInterface);
    d->modemGsmSmsIface.SetFormat(format);
}

QString ModemManager::ModemGsmSmsInterface::getSmsc()
{
    Q_D(ModemGsmSmsInterface);
    QDBusReply<QString> smsc = d->modemGsmSmsIface.GetSmsc();

    if (smsc.isValid()) {
        return smsc.value();
    }

    return QString();
}

QList<QVariantMap> ModemManager::ModemGsmSmsInterface::list()
{
    Q_D(ModemGsmSmsInterface);
    QDBusReply<QList<QVariantMap> > sms = d->modemGsmSmsIface.List();

    if (sms.isValid()) {
        return sms.value();
    }

    return QList<QVariantMap>();
}

void ModemManager::ModemGsmSmsInterface::save(const QVariantMap & properties)
{
    Q_D(ModemGsmSmsInterface);
    d->modemGsmSmsIface.Save(properties);
}

void ModemManager::ModemGsmSmsInterface::send(const QVariantMap & properties)
{
    Q_D(ModemGsmSmsInterface);
    d->modemGsmSmsIface.Save(properties);
}

void ModemManager::ModemGsmSmsInterface::sendFromStorage(const int index)
{
    Q_D(ModemGsmSmsInterface);
    d->modemGsmSmsIface.SendFromStorage(index);
}

void ModemManager::ModemGsmSmsInterface::setIndication(const int mode, const int mt, const int bm, const int ds, const int brf)
{
    Q_D(ModemGsmSmsInterface);
    d->modemGsmSmsIface.SetIndication(mode, mt, bm, ds, brf);
}



