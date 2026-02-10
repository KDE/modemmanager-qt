/*
    SPDX-FileCopyrightText: 2026 Devin Lin <devin@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "cbm.h"
#include "cbm_p.h"
#include "mmdebug_p.h"

#ifdef MMQT_STATIC
#include "dbus/fakedbus.h"
#else
#include "dbus/dbus.h"
#endif

#include <ModemManager/ModemManager.h>

ModemManager::CbmPrivate::CbmPrivate(const QString &path, Cbm *q)
#ifdef MMQT_STATIC
    : cbmIface(QLatin1String(MMQT_DBUS_SERVICE), path, QDBusConnection::sessionBus())
#else
    : cbmIface(QLatin1String(MMQT_DBUS_SERVICE), path, QDBusConnection::systemBus())
#endif
    , q_ptr(q)
{
    if (cbmIface.isValid()) {
        uni = path;
        state = (MMCbmState)cbmIface.state();
        text = cbmIface.text();
        channel = cbmIface.channel();
        messageCode = cbmIface.messageCode();
        update = cbmIface.update();
    }
}

ModemManager::Cbm::Cbm(const QString &path, QObject *parent)
    : QObject(parent)
    , d_ptr(new CbmPrivate(path, this))
{
    Q_D(Cbm);

    qRegisterMetaType<MMCbmState>();

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

ModemManager::Cbm::~Cbm()
{
    delete d_ptr;
}

QString ModemManager::Cbm::uni() const
{
    Q_D(const Cbm);
    return d->uni;
}

MMCbmState ModemManager::Cbm::state() const
{
    Q_D(const Cbm);
    return d->state;
}

QString ModemManager::Cbm::text() const
{
    Q_D(const Cbm);
    return d->text;
}

uint ModemManager::Cbm::channel() const
{
    Q_D(const Cbm);
    return d->channel;
}

uint ModemManager::Cbm::messageCode() const
{
    Q_D(const Cbm);
    return d->messageCode;
}

uint ModemManager::Cbm::update() const
{
    Q_D(const Cbm);
    return d->update;
}

void ModemManager::Cbm::setTimeout(int timeout)
{
    Q_D(Cbm);
    d->cbmIface.setTimeout(timeout);
}

int ModemManager::Cbm::timeout() const
{
    Q_D(const Cbm);
    return d->cbmIface.timeout();
}

void ModemManager::CbmPrivate::onPropertiesChanged(const QString &interfaceName, const QVariantMap &changedProperties, const QStringList &invalidatedProperties)
{
    Q_UNUSED(invalidatedProperties);
    Q_Q(Cbm);

    if (interfaceName == QLatin1String(MMQT_DBUS_INTERFACE_CBM)) {
        QVariantMap::const_iterator it = changedProperties.constFind(QLatin1String(MM_CBM_PROPERTY_STATE));
        if (it != changedProperties.constEnd()) {
            state = (MMCbmState)it->toUInt();
            Q_EMIT q->stateChanged(state);
        }
        it = changedProperties.constFind(QLatin1String(MM_CBM_PROPERTY_TEXT));
        if (it != changedProperties.constEnd()) {
            text = it->toString();
            Q_EMIT q->textChanged(text);
        }
        it = changedProperties.constFind(QLatin1String(MM_CBM_PROPERTY_CHANNEL));
        if (it != changedProperties.constEnd()) {
            channel = it->toUInt();
            Q_EMIT q->channelChanged(channel);
        }
        it = changedProperties.constFind(QLatin1String(MM_CBM_PROPERTY_MESSAGECODE));
        if (it != changedProperties.constEnd()) {
            messageCode = it->toUInt();
            Q_EMIT q->messageCodeChanged(messageCode);
        }
        it = changedProperties.constFind(QLatin1String(MM_CBM_PROPERTY_UPDATE));
        if (it != changedProperties.constEnd()) {
            update = it->toUInt();
            Q_EMIT q->updateChanged(update);
        }
    }
}

#include "moc_cbm.cpp"
#include "moc_cbm_p.cpp"
