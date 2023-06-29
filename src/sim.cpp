/*
    SPDX-FileCopyrightText: 2008, 2011 Will Stephenson <wstephenson@kde.org>
    SPDX-FileCopyrightText: 2010-2011 Lamarque Souza <lamarque@kde.org>
    SPDX-FileCopyrightText: 2013 Lukas Tinkl <ltinkl@redhat.com>
    SPDX-FileCopyrightText: 2013-2015 Jan Grulich <jgrulich@redhat.com>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "sim.h"
#include "mmdebug_p.h"
#include "sim_p.h"
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
    QDBusConnection::sessionBus().connect(QLatin1String(MMQT_DBUS_SERVICE),
                                          path,
                                          QLatin1String(DBUS_INTERFACE_PROPS),
                                          QStringLiteral("PropertiesChanged"),
                                          d,
                                          SLOT(onPropertiesChanged(QString, QVariantMap, QStringList)));
#else
    QDBusConnection::systemBus().connect(QLatin1String(MMQT_DBUS_SERVICE),
                                         path,
                                         QLatin1String(DBUS_INTERFACE_PROPS),
                                         QStringLiteral("PropertiesChanged"),
                                         d,
                                         SLOT(onPropertiesChanged(QString, QVariantMap, QStringList)));
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

void ModemManager::Sim::setTimeout(int timeout)
{
    Q_D(Sim);
    d->simIface.setTimeout(timeout);
}

int ModemManager::Sim::timeout() const
{
    Q_D(const Sim);
    return d->simIface.timeout();
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

#include "moc_sim.cpp"
#include "moc_sim_p.cpp"
