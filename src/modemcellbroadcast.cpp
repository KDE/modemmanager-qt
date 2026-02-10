/*
    SPDX-FileCopyrightText: 2026 Devin Lin <devin@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "modemcellbroadcast.h"
#include "modemcellbroadcast_p.h"

#ifdef MMQT_STATIC
#include "dbus/fakedbus.h"
#else
#include "dbus/dbus.h"
#endif
#include "mmdebug_p.h"

#include <ModemManager/ModemManager.h>

ModemManager::ModemCellBroadcastPrivate::ModemCellBroadcastPrivate(const QString &path, ModemCellBroadcast *q)
    : InterfacePrivate(path, q)
#ifdef MMQT_STATIC
    , modemCellBroadcastIface(QLatin1String(MMQT_DBUS_SERVICE), path, QDBusConnection::sessionBus())
#else
    , modemCellBroadcastIface(QLatin1String(MMQT_DBUS_SERVICE), path, QDBusConnection::systemBus())
#endif
    , q_ptr(q)
{
    if (modemCellBroadcastIface.isValid()) {
        channels = modemCellBroadcastIface.channels();

        connect(&modemCellBroadcastIface,
                &OrgFreedesktopModemManager1ModemCellBroadcastInterface::Added,
                this,
                &ModemCellBroadcastPrivate::onCellBroadcastAdded);
        connect(&modemCellBroadcastIface,
                &OrgFreedesktopModemManager1ModemCellBroadcastInterface::Deleted,
                this,
                &ModemCellBroadcastPrivate::onCellBroadcastDeleted);
    }
}

ModemManager::ModemCellBroadcast::ModemCellBroadcast(const QString &path, QObject *parent)
    : Interface(*new ModemCellBroadcastPrivate(path, this), parent)
{
    Q_D(ModemCellBroadcast);

#ifdef MMQT_STATIC
    QDBusConnection::sessionBus().connect(QLatin1String(MMQT_DBUS_SERVICE),
                                          path,
                                          QLatin1String(DBUS_INTERFACE_PROPS),
                                          QStringLiteral("PropertiesChanged"),
                                          this,
                                          SLOT(onPropertiesChanged(QString, QVariantMap, QStringList)));
#else
    QDBusConnection::systemBus().connect(QLatin1String(MMQT_DBUS_SERVICE),
                                         path,
                                         QLatin1String(DBUS_INTERFACE_PROPS),
                                         QStringLiteral("PropertiesChanged"),
                                         this,
                                         SLOT(onPropertiesChanged(QString, QVariantMap, QStringList)));
#endif

    QList<QDBusObjectPath> cellBroadcasts = d->modemCellBroadcastIface.cellBroadcasts();
    Q_FOREACH (const QDBusObjectPath &op, cellBroadcasts) {
        const QString objectPathAsString = op.path();
        d->cellBroadcastList.insert(objectPathAsString, ModemManager::Cbm::Ptr());
        Q_EMIT cellBroadcastAdded(objectPathAsString);
    }
}

ModemManager::ModemCellBroadcast::~ModemCellBroadcast()
{
}

ModemManager::Cbm::Ptr ModemManager::ModemCellBroadcastPrivate::findCellBroadcast(const QString &uni)
{
    ModemManager::Cbm::Ptr cbm;
    if (cellBroadcastList.contains(uni)) {
        if (cellBroadcastList.value(uni)) {
            cbm = cellBroadcastList.value(uni);
        } else {
            cbm = ModemManager::Cbm::Ptr(new ModemManager::Cbm(uni), &QObject::deleteLater);
            cellBroadcastList[uni] = cbm;
        }
    }
    return cbm;
}

ModemManager::Cbm::List ModemManager::ModemCellBroadcastPrivate::cellBroadcasts()
{
    ModemManager::Cbm::List list;

    QMap<QString, ModemManager::Cbm::Ptr>::const_iterator i;
    for (i = cellBroadcastList.constBegin(); i != cellBroadcastList.constEnd(); ++i) {
        ModemManager::Cbm::Ptr cbm = findCellBroadcast(i.key());
        if (cbm) {
            list.append(cbm);
        } else {
            qCWarning(MMQT) << "warning: null cell broadcast for" << i.key();
        }
    }

    return list;
}

void ModemManager::ModemCellBroadcastPrivate::onPropertiesChanged(const QString &interfaceName,
                                                                  const QVariantMap &changedProperties,
                                                                  const QStringList &invalidatedProperties)
{
    Q_UNUSED(invalidatedProperties);

    if (interfaceName == QLatin1String(MMQT_DBUS_INTERFACE_MODEM_CELLBROADCAST)) {
        QVariantMap::const_iterator it = changedProperties.constFind(QLatin1String(MM_MODEM_CELLBROADCAST_PROPERTY_CHANNELS));
        if (it != changedProperties.constEnd()) {
            channels = qdbus_cast<ModemManager::CellBroadcastChannelRangeList>(*it);
            Q_Q(ModemCellBroadcast);
            Q_EMIT q->channelsChanged(channels);
        }
        // CellBroadcasts should be handled by Added/Deleted signals
    }
}

void ModemManager::ModemCellBroadcastPrivate::onCellBroadcastAdded(const QDBusObjectPath &path)
{
    Q_Q(ModemCellBroadcast);
    cellBroadcastList.insert(path.path(), ModemManager::Cbm::Ptr());
    Q_EMIT q->cellBroadcastAdded(path.path());
}

void ModemManager::ModemCellBroadcastPrivate::onCellBroadcastDeleted(const QDBusObjectPath &path)
{
    Q_Q(ModemCellBroadcast);
    cellBroadcastList.remove(path.path());
    Q_EMIT q->cellBroadcastDeleted(path.path());
}

ModemManager::Cbm::List ModemManager::ModemCellBroadcast::cellBroadcasts() const
{
    Q_D(const ModemCellBroadcast);
    return const_cast<ModemCellBroadcastPrivate *>(d)->cellBroadcasts();
}

ModemManager::Cbm::Ptr ModemManager::ModemCellBroadcast::findCellBroadcast(const QString &uni)
{
    Q_D(ModemCellBroadcast);
    return d->findCellBroadcast(uni);
}

QDBusPendingReply<void> ModemManager::ModemCellBroadcast::deleteCellBroadcast(const QString &uni)
{
    Q_D(ModemCellBroadcast);
    return d->modemCellBroadcastIface.Delete(QDBusObjectPath(uni));
}

QDBusPendingReply<void> ModemManager::ModemCellBroadcast::setChannels(const ModemManager::CellBroadcastChannelRangeList &channels)
{
    Q_D(ModemCellBroadcast);
    return d->modemCellBroadcastIface.SetChannels(channels);
}

ModemManager::CellBroadcastChannelRangeList ModemManager::ModemCellBroadcast::channels() const
{
    Q_D(const ModemCellBroadcast);
    return d->channels;
}

void ModemManager::ModemCellBroadcast::setTimeout(int timeout)
{
    Q_D(ModemCellBroadcast);
    d->modemCellBroadcastIface.setTimeout(timeout);
}

int ModemManager::ModemCellBroadcast::timeout() const
{
    Q_D(const ModemCellBroadcast);
    return d->modemCellBroadcastIface.timeout();
}

#include "moc_modemcellbroadcast.cpp"
