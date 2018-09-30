/*
    Copyright 2008,2011 Will Stephenson <wstephenson@kde.org>
    Copyright 2010-2011 Lamarque Souza <lamarque@kde.org>
    Copyright 2013 Lukas Tinkl <ltinkl@redhat.com>
    Copyright 2013-2015 Jan Grulich <jgrulich@redhat.com>

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

#include "sim.h"
#include "sim_p.h"
#include "mmdebug_p.h"
#ifdef MMQT_STATIC
#include "dbus/fakedbus.h"
#else
#include "dbus/dbus.h"
#endif

ModemManager::SimPrivate::SimPrivate(const QString &path, Sim *q)
#ifdef MMQT_STATIC
    : simIface(QLatin1String(MMQT_DBUS_SERVICE), path, QDBusConnection::sessionBus())
#else
    : simIface(QLatin1String(MMQT_DBUS_SERVICE), path, QDBusConnection::systemBus())
#endif
    , uni(path)
    , q_ptr(q)
{
    if (simIface.isValid()) {
        simIdentifier = simIface.simIdentifier();
        imsi = simIface.imsi();
        operatorIdentifier = simIface.operatorIdentifier();
        operatorName = simIface.operatorName();
    }
}

ModemManager::Sim::Sim(const QString &path, QObject *parent)
    : QObject(parent)
    , d_ptr(new SimPrivate(path, this))
{
    Q_D(Sim);
#ifdef MMQT_STATIC
    QDBusConnection::sessionBus().connect(QLatin1String(MMQT_DBUS_SERVICE), path, QLatin1String(DBUS_INTERFACE_PROPS), QStringLiteral("PropertiesChanged"), d,
                                         SLOT(onPropertiesChanged(QString,QVariantMap,QStringList)));
#else
    QDBusConnection::systemBus().connect(QLatin1String(MMQT_DBUS_SERVICE), path, QLatin1String(DBUS_INTERFACE_PROPS), QStringLiteral("PropertiesChanged"), d,
                                         SLOT(onPropertiesChanged(QString,QVariantMap,QStringList)));
#endif
}

ModemManager::Sim::~Sim()
{
    delete d_ptr;
}

QString ModemManager::Sim::simIdentifier() const
{
    Q_D(const Sim);
    return d->simIdentifier;
}

QString ModemManager::Sim::imsi() const
{
    Q_D(const Sim);
    return d->imsi;
}

QString ModemManager::Sim::operatorIdentifier() const
{
    Q_D(const Sim);
    return d->operatorIdentifier;
}

QString ModemManager::Sim::operatorName() const
{
    Q_D(const Sim);
    return d->operatorName;
}

QDBusPendingReply<> ModemManager::Sim::sendPuk(const QString &puk, const QString &pin)
{
    Q_D(Sim);
    return d->simIface.SendPuk(puk, pin);
}

QDBusPendingReply<> ModemManager::Sim::sendPin(const QString &pin)
{
    Q_D(Sim);
    return d->simIface.SendPin(pin);
}

QDBusPendingReply<> ModemManager::Sim::enablePin(const QString &pin, bool enabled)
{
    Q_D(Sim);
    return d->simIface.EnablePin(pin, enabled);
}

QDBusPendingReply<> ModemManager::Sim::changePin(const QString &oldPin, const QString &newPin)
{
    Q_D(Sim);
    return d->simIface.ChangePin(oldPin, newPin);
}

QString ModemManager::Sim::uni() const
{
    Q_D(const Sim);
    return d->uni;
}

void ModemManager::SimPrivate::onPropertiesChanged(const QString &interface, const QVariantMap &properties, const QStringList &invalidatedProps)
{
    Q_Q(Sim);
    Q_UNUSED(invalidatedProps);
    qCDebug(MMQT) << interface << properties.keys();

    if (interface == QLatin1String(MMQT_DBUS_INTERFACE_SIM)) {
        QVariantMap::const_iterator it = properties.constFind(QLatin1String(MM_SIM_PROPERTY_SIMIDENTIFIER));
        if (it != properties.constEnd()) {
            simIdentifier = it->toString();
            Q_EMIT q->simIdentifierChanged(simIdentifier);
        }
        it = properties.constFind(QLatin1String(MM_SIM_PROPERTY_IMSI));
        if (it != properties.constEnd()) {
            imsi = it->toString();
            Q_EMIT q->imsiChanged(imsi);
        }
        it = properties.constFind(QLatin1String(MM_SIM_PROPERTY_OPERATORIDENTIFIER));
        if (it != properties.constEnd()) {
            operatorIdentifier = it->toString();
            Q_EMIT q->operatorIdentifierChanged(operatorIdentifier);
        }
        it = properties.constFind(QLatin1String(MM_SIM_PROPERTY_OPERATORNAME));
        if (it != properties.constEnd()) {
            operatorName = it->toString();
            Q_EMIT q->operatorNameChanged(operatorName);
        }
    }
}
