/*
    Copyright 2008,2011 Will Stephenson <wstephenson@kde.org>
    Copyright 2010 Lamarque Souza <lamarque@kde.org>
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

#include "modem3gpp.h"
#include "modem3gpp_p.h"
#include "mmdebug.h"
#ifdef MMQT_STATIC
#include "dbus/fakedbus.h"
#else
#include "dbus/dbus.h"
#endif

ModemManager::Modem3gppPrivate::Modem3gppPrivate(const QString &path, Modem3gpp *q)
    : InterfacePrivate(path, q)
#ifdef MMQT_STATIC
    , modem3gppIface(MMQT_DBUS_SERVICE, path, QDBusConnection::sessionBus())
#else
    , modem3gppIface(MMQT_DBUS_SERVICE, path, QDBusConnection::systemBus())
#endif
    , q_ptr(q)
{
    if (modem3gppIface.isValid()) {
        imei = modem3gppIface.imei();
        registrationState = (MMModem3gppRegistrationState)modem3gppIface.registrationState();
        operatorCode = modem3gppIface.operatorCode();
        operatorName = modem3gppIface.operatorName();
        enabledFacilityLocks = (QFlags<MMModem3gppFacility>)modem3gppIface.enabledFacilityLocks();
        subscriptionState = (MMModem3gppSubscriptionState)modem3gppIface.subscriptionState();
    }
}

ModemManager::Modem3gpp::Modem3gpp(const QString &path, QObject *parent)
    : Interface(*new Modem3gppPrivate(path, this), parent)
{
    Q_D(Modem3gpp);
#ifdef MMQT_STATIC
    QDBusConnection::sessionBus().connect(MMQT_DBUS_SERVICE, d->uni, DBUS_INTERFACE_PROPS, QStringLiteral("PropertiesChanged"), d,
                                         SLOT(onPropertiesChanged(QString,QVariantMap,QStringList)));
#else
    QDBusConnection::systemBus().connect(MMQT_DBUS_SERVICE, d->uni, DBUS_INTERFACE_PROPS, QStringLiteral("PropertiesChanged"), d,
                                         SLOT(onPropertiesChanged(QString,QVariantMap,QStringList)));
#endif
}

ModemManager::Modem3gpp::~Modem3gpp()
{
}

QString ModemManager::Modem3gpp::imei() const
{
    Q_D(const Modem3gpp);
    return d->imei;
}

MMModem3gppRegistrationState ModemManager::Modem3gpp::registrationState() const
{
    Q_D(const Modem3gpp);
    return d->registrationState;
}

QString ModemManager::Modem3gpp::operatorCode() const
{
    Q_D(const Modem3gpp);
    return d->operatorCode;
}

QString ModemManager::Modem3gpp::operatorName() const
{
    Q_D(const Modem3gpp);
    return d->operatorName;
}

ModemManager::Modem3gpp::FacilityLocks ModemManager::Modem3gpp::enabledFacilityLocks() const
{
    Q_D(const Modem3gpp);
    return d->enabledFacilityLocks;
}

MMModem3gppSubscriptionState ModemManager::Modem3gpp::subscriptionState() const
{
    Q_D(const Modem3gpp);
    return d->subscriptionState;
}

void ModemManager::Modem3gpp::registerToNetwork(const QString &networkId)
{
    Q_D(Modem3gpp);
    d->modem3gppIface.Register(networkId);
}

QDBusPendingReply<QVariantMapList> ModemManager::Modem3gpp::scan()
{
    Q_D(Modem3gpp);
    return d->modem3gppIface.Scan();
}

void ModemManager::Modem3gppPrivate::onPropertiesChanged(const QString &interface, const QVariantMap &properties, const QStringList &invalidatedProps)
{
    Q_Q(Modem3gpp);
    Q_UNUSED(invalidatedProps);
    qCDebug(MMQT) << interface << properties.keys();

    if (interface == QString(MMQT_DBUS_INTERFACE_MODEM_MODEM3GPP)) {
        QVariantMap::const_iterator it = properties.constFind(QLatin1String(MM_MODEM_MODEM3GPP_PROPERTY_IMEI));
        if (it != properties.constEnd()) {
            imei = it->toString();
            Q_EMIT q->imeiChanged(imei);
        }
        it = properties.constFind(QLatin1String(MM_MODEM_MODEM3GPP_PROPERTY_REGISTRATIONSTATE));
        if (it != properties.constEnd()) {
            registrationState = (MMModem3gppRegistrationState)it->toUInt();
            Q_EMIT q->registrationStateChanged(registrationState);
        }
        it = properties.constFind(QLatin1String(MM_MODEM_MODEM3GPP_PROPERTY_OPERATORCODE));
        if (it != properties.constEnd()) {
            operatorCode = it->toString();
            Q_EMIT q->operatorCodeChanged(operatorCode);
        }
        it = properties.constFind(QLatin1String(MM_MODEM_MODEM3GPP_PROPERTY_OPERATORNAME));
        if (it != properties.constEnd()) {
            operatorName = it->toString();
            Q_EMIT q->operatorNameChanged(operatorName);
        }
        it = properties.constFind(QLatin1String(MM_MODEM_MODEM3GPP_PROPERTY_ENABLEDFACILITYLOCKS));
        if (it != properties.constEnd()) {
            enabledFacilityLocks = (QFlags<MMModem3gppFacility>)it->toUInt();
            Q_EMIT q->enabledFacilityLocksChanged(enabledFacilityLocks);
        }
        it = properties.constFind(QLatin1String(MM_MODEM_MODEM3GPP_PROPERTY_SUBSCRIPTIONSTATE));
        if (it != properties.constEnd()) {
            subscriptionState = (MMModem3gppSubscriptionState)it->toUInt();
            Q_EMIT q->subscriptionStateChanged(subscriptionState);
        }
    }
}
