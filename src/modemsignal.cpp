/*
    Copyright 2014 Lukas Tinkl <ltinkl@redhat.com>
    Copyright 2015 Jan Grulich <jgrulich@redhat.com>

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

#include "mmdebug.h"
#include "modemsignal.h"
#include "modemsignal_p.h"
#ifdef MMQT_STATIC
#include "dbus/fakedbus.h"
#else
#include "dbus/dbus.h"
#endif

ModemManager::ModemSignalPrivate::ModemSignalPrivate(const QString &path, ModemSignal *q)
    : InterfacePrivate(path, q)
#ifdef MMQT_STATIC
    , modemSignalIface(MMQT_DBUS_SERVICE, path, QDBusConnection::sessionBus())
#else
    , modemSignalIface(MMQT_DBUS_SERVICE, path, QDBusConnection::systemBus())
#endif
    , q_ptr(q)
{
    if (modemSignalIface.isValid()) {
        rate = modemSignalIface.rate();
        cdma = modemSignalIface.cdma();
        evdo = modemSignalIface.evdo();
        gsm = modemSignalIface.gsm();
        umts = modemSignalIface.umts();
        lte = modemSignalIface.lte();
    }
}

ModemManager::ModemSignal::ModemSignal(const QString &path, QObject *parent)
    : Interface(*new ModemSignalPrivate(path, this), parent)
{
    Q_D(ModemSignal);
#ifdef MMQT_STATIC
    QDBusConnection::sessionBus().connect(MMQT_DBUS_SERVICE, d->uni, DBUS_INTERFACE_PROPS, QStringLiteral("PropertiesChanged"), d,
                                        SLOT(onPropertiesChanged(QString,QVariantMap,QStringList)));
#else
    QDBusConnection::systemBus().connect(MMQT_DBUS_SERVICE, d->uni, DBUS_INTERFACE_PROPS, QStringLiteral("PropertiesChanged"), d,
                                        SLOT(onPropertiesChanged(QString,QVariantMap,QStringList)));
#endif
}

ModemManager::ModemSignal::~ModemSignal()
{
}

uint ModemManager::ModemSignal::rate() const
{
    Q_D(const ModemSignal);
    return d->rate;
}

QVariantMap ModemManager::ModemSignal::cdma() const
{
    Q_D(const ModemSignal);
    return d->cdma;
}

QVariantMap ModemManager::ModemSignal::evdo() const
{
    Q_D(const ModemSignal);
    return d->evdo;
}

QVariantMap ModemManager::ModemSignal::gsm() const
{
    Q_D(const ModemSignal);
    return d->gsm;
}

QVariantMap ModemManager::ModemSignal::lte() const
{
    Q_D(const ModemSignal);
    return d->lte;
}

QVariantMap ModemManager::ModemSignal::umts() const
{
    Q_D(const ModemSignal);
    return d->umts;
}

QDBusPendingReply<void> ModemManager::ModemSignal::setup(uint rate)
{
    Q_D(ModemSignal);
    return d->modemSignalIface.Setup(rate);
}

void ModemManager::ModemSignalPrivate::onPropertiesChanged(const QString &interface, const QVariantMap &properties, const QStringList &invalidatedProps)
{
    Q_Q(ModemSignal);
    Q_UNUSED(invalidatedProps);
    qCDebug(MMQT) << interface << properties.keys();

    if (interface == QString(MMQT_DBUS_INTERFACE_MODEM_SIGNAL)) {
        QVariantMap::const_iterator it = properties.constFind(QLatin1String(MM_MODEM_SIGNAL_PROPERTY_RATE));
        if (it != properties.constEnd()) {
            rate = it->toUInt();
            Q_EMIT q->rateChanged(rate);
        }
        it = properties.constFind(QLatin1String(MM_MODEM_SIGNAL_PROPERTY_CDMA));
        if (it != properties.constEnd()) {
            cdma = it->toMap();
            Q_EMIT q->cdmaChanged(cdma);
        }
        it = properties.constFind(QLatin1String(MM_MODEM_SIGNAL_PROPERTY_EVDO));
        if (it != properties.constEnd()) {
            evdo = it->toMap();
            Q_EMIT q->cdmaChanged(evdo);
        }
        it = properties.constFind(QLatin1String(MM_MODEM_SIGNAL_PROPERTY_GSM));
        if (it != properties.constEnd()) {
            gsm = it->toMap();
            Q_EMIT q->cdmaChanged(gsm);
        }
        it = properties.constFind(QLatin1String(MM_MODEM_SIGNAL_PROPERTY_UMTS));
        if (it != properties.constEnd()) {
            umts = it->toMap();
            Q_EMIT q->cdmaChanged(umts);
        }
        it = properties.constFind(QLatin1String(MM_MODEM_SIGNAL_PROPERTY_LTE));
        if (it != properties.constEnd()) {
            lte = it->toMap();
            Q_EMIT q->cdmaChanged(lte);
        }
    }
}
