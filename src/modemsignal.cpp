/*
    SPDX-FileCopyrightText: 2014 Lukas Tinkl <ltinkl@redhat.com>
    SPDX-FileCopyrightText: 2015 Jan Grulich <jgrulich@redhat.com>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "modemsignal.h"
#include "mmdebug_p.h"
#include "modemsignal_p.h"
#ifdef MMQT_STATIC
#include "dbus/fakedbus.h"
#else
#include "dbus/dbus.h"
#endif

ModemManager::ModemSignalPrivate::ModemSignalPrivate(const QString &path, ModemSignal *q)
    : InterfacePrivate(path, q)
#ifdef MMQT_STATIC
    , modemSignalIface(QLatin1String(MMQT_DBUS_SERVICE), path, QDBusConnection::sessionBus())
#else
    , modemSignalIface(QLatin1String(MMQT_DBUS_SERVICE), path, QDBusConnection::systemBus())
#endif
    , q_ptr(q)
{
    if (modemSignalIface.isValid()) {
        rate = modemSignalIface.rate();
        rssiThreshold = modemSignalIface.rssiThreshold();
        errorRateThreshold = modemSignalIface.errorRateThreshold();
        cdma = modemSignalIface.cdma();
        evdo = modemSignalIface.evdo();
        gsm = modemSignalIface.gsm();
        umts = modemSignalIface.umts();
        lte = modemSignalIface.lte();
        nr5g = modemSignalIface.nr5g();
    }
}

ModemManager::ModemSignal::ModemSignal(const QString &path, QObject *parent)
    : Interface(*new ModemSignalPrivate(path, this), parent)
{
    Q_D(ModemSignal);
#ifdef MMQT_STATIC
    QDBusConnection::sessionBus().connect(QLatin1String(MMQT_DBUS_SERVICE),
                                          d->uni,
                                          QLatin1String(DBUS_INTERFACE_PROPS),
                                          QStringLiteral("PropertiesChanged"),
                                          d,
                                          SLOT(onPropertiesChanged(QString, QVariantMap, QStringList)));
#else
    QDBusConnection::systemBus().connect(QLatin1String(MMQT_DBUS_SERVICE),
                                         d->uni,
                                         QLatin1String(DBUS_INTERFACE_PROPS),
                                         QStringLiteral("PropertiesChanged"),
                                         d,
                                         SLOT(onPropertiesChanged(QString, QVariantMap, QStringList)));
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

uint ModemManager::ModemSignal::rssiThreshold() const
{
    Q_D(const ModemSignal);
    return d->rssiThreshold;
}

bool ModemManager::ModemSignal::errorRateThreshold() const
{
    Q_D(const ModemSignal);
    return d->errorRateThreshold;
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

QVariantMap ModemManager::ModemSignal::nr5g() const
{
    Q_D(const ModemSignal);
    return d->nr5g;
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

QDBusPendingReply<void> ModemManager::ModemSignal::setupThresholds(const QVariantMap &settings)
{
    Q_D(ModemSignal);
    return d->modemSignalIface.SetupThresholds(settings);
}

void ModemManager::ModemSignal::setTimeout(int timeout)
{
    Q_D(ModemSignal);
    d->modemSignalIface.setTimeout(timeout);
}

int ModemManager::ModemSignal::timeout() const
{
    Q_D(const ModemSignal);
    return d->modemSignalIface.timeout();
}

void ModemManager::ModemSignalPrivate::onPropertiesChanged(const QString &interface, const QVariantMap &properties, const QStringList &invalidatedProps)
{
    Q_Q(ModemSignal);
    Q_UNUSED(invalidatedProps);
    qCDebug(MMQT) << interface << properties.keys();

    if (interface == QLatin1String(MMQT_DBUS_INTERFACE_MODEM_SIGNAL)) {
        QVariantMap::const_iterator it = properties.constFind(QLatin1String(MM_MODEM_SIGNAL_PROPERTY_RATE));
        if (it != properties.constEnd()) {
            rate = it->toUInt();
            Q_EMIT q->rateChanged(rate);
        }
        it = properties.constFind(QLatin1String(MM_MODEM_SIGNAL_PROPERTY_RSSITHRESHOLD));
        if (it != properties.constEnd()) {
            rssiThreshold = it->toUInt();
            Q_EMIT q->rssiThresholdChanged(rssiThreshold);
        }
        it = properties.constFind(QLatin1String(MM_MODEM_SIGNAL_PROPERTY_ERRORRATETHRESHOLD));
        if (it != properties.constEnd()) {
            errorRateThreshold = it->toBool();
            Q_EMIT q->errorRateThresholdChanged(errorRateThreshold);
        }
        it = properties.constFind(QLatin1String(MM_MODEM_SIGNAL_PROPERTY_CDMA));
        if (it != properties.constEnd()) {
            cdma = qdbus_cast<QVariantMap>(*it);
            Q_EMIT q->cdmaChanged(cdma);
        }
        it = properties.constFind(QLatin1String(MM_MODEM_SIGNAL_PROPERTY_EVDO));
        if (it != properties.constEnd()) {
            evdo = qdbus_cast<QVariantMap>(*it);
            Q_EMIT q->evdoChanged(evdo);
        }
        it = properties.constFind(QLatin1String(MM_MODEM_SIGNAL_PROPERTY_GSM));
        if (it != properties.constEnd()) {
            gsm = qdbus_cast<QVariantMap>(*it);
            Q_EMIT q->gsmChanged(gsm);
        }
        it = properties.constFind(QLatin1String(MM_MODEM_SIGNAL_PROPERTY_UMTS));
        if (it != properties.constEnd()) {
            umts = qdbus_cast<QVariantMap>(*it);
            Q_EMIT q->umtsChanged(umts);
        }
        it = properties.constFind(QLatin1String(MM_MODEM_SIGNAL_PROPERTY_LTE));
        if (it != properties.constEnd()) {
            lte = qdbus_cast<QVariantMap>(*it);
            Q_EMIT q->lteChanged(lte);
        }
        it = properties.constFind(QLatin1String(MM_MODEM_SIGNAL_PROPERTY_NR5G));
        if (it != properties.constEnd()) {
            nr5g = qdbus_cast<QVariantMap>(*it);
            Q_EMIT q->nr5gChanged(nr5g);
        }
    }
}

#include "moc_modemsignal.cpp"
