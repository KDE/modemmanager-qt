/*
Copyright 2008,2011 Will Stephenson <wstephenson@kde.org>
Copyright 2010-2011 Lamarque Souza <lamarque@kde.org>
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

#include "modemgsmcardinterface.h"
#include "modemgsmcardinterface_p.h"
#include "manager.h"
#include "mmdebug.h"

ModemSimCardInterfacePrivate::ModemSimCardInterfacePrivate(const QString &path, QObject *owner)
    : ModemInterfacePrivate(path, owner),
      modemSimCardIface(MM_DBUS_SERVICE, path, QDBusConnection::systemBus(), this)
{
}

ModemManager::ModemSimCardInterface::ModemSimCardInterface(const QString & path, QObject * parent)
    : ModemInterface(*new ModemSimCardInterfacePrivate(path, this), parent)
{
    Q_D(ModemSimCardInterface);

#if 0
    d->modemSimCardIface.connection().connect(ModemManager::DBUS_SERVICE,
        path, QLatin1String("org.freedesktop.DBus.Properties"),
        QLatin1String("MmPropertiesChanged"), QLatin1String("sa{sv}"),
        this, SLOT(propertiesChanged(QString,QVariantMap)));
#endif
}

ModemManager::ModemSimCardInterface::~ModemSimCardInterface()
{
}

#if 0
void ModemManager::ModemSimCardInterface::propertiesChanged(const QString & interface, const QVariantMap & properties)
{
    mmDebug() << interface << properties.keys();

    if (interface == QString("org.freedesktop.ModemManager.Modem.Gsm.Card")) {
        QLatin1String supportedBands("SupportedBands");
        QLatin1String supportedModes("SupportedModes");

        QVariantMap::const_iterator it = properties.find(supportedBands);
        if ( it != properties.end()) {
            emit supportedBandsChanged((ModemManager::ModemInterface::Band) it->toInt());
        }
        it = properties.find(supportedModes);
        if ( it != properties.end()) {
            emit supportedModesChanged((ModemManager::ModemInterface::Mode) it->toInt());
        }
    }
}
#endif

QString ModemManager::ModemSimCardInterface::simIdentifier() const
{
    Q_D(const ModemSimCardInterface);
    return d->modemSimCardIface.simIdentifier();
}

QString ModemManager::ModemSimCardInterface::imsi() const
{
    Q_D(const ModemSimCardInterface);
    return d->modemSimCardIface.imsi();
}

QString ModemManager::ModemSimCardInterface::operatorIdentifier() const
{
    Q_D(const ModemSimCardInterface);
    return d->modemSimCardIface.operatorIdentifier();
}

QString ModemManager::ModemSimCardInterface::operatorName() const
{
    Q_D(const ModemSimCardInterface);
    return d->modemSimCardIface.operatorName();
}

QDBusPendingReply<> ModemManager::ModemSimCardInterface::sendPuk(const QString & puk, const QString & pin)
{
    Q_D(ModemSimCardInterface);
    return d->modemSimCardIface.SendPuk(puk, pin);
}

QDBusPendingReply<> ModemManager::ModemSimCardInterface::sendPin(const QString & pin)
{
    Q_D(ModemSimCardInterface);
    return d->modemSimCardIface.SendPin(pin);
}

void ModemManager::ModemSimCardInterface::enablePin(const QString & pin, bool enabled)
{
    Q_D(ModemSimCardInterface);
    d->modemSimCardIface.EnablePin(pin, enabled);
}

void ModemManager::ModemSimCardInterface::changePin(const QString & oldPin, const QString & newPin)
{
    Q_D(ModemSimCardInterface);
    d->modemSimCardIface.ChangePin(oldPin, newPin);
}
