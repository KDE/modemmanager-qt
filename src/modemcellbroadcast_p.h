/*
    SPDX-FileCopyrightText: 2026 Devin Lin <devin@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef MODEMMANAGER_MODEMCELLBROADCAST_P_H
#define MODEMMANAGER_MODEMCELLBROADCAST_P_H

#include "dbus/cellbroadcastinterface.h"
#include "interface_p.h"
#include "modemcellbroadcast.h"

namespace ModemManager
{
class ModemCellBroadcastPrivate : public InterfacePrivate
{
public:
    explicit ModemCellBroadcastPrivate(const QString &path, ModemCellBroadcast *q);
    OrgFreedesktopModemManager1ModemCellBroadcastInterface modemCellBroadcastIface;

    QMap<QString, ModemManager::Cbm::Ptr> cellBroadcastList;
    ModemManager::CellBroadcastChannelRangeList channels;

    ModemManager::Cbm::Ptr findCellBroadcast(const QString &uni);
    ModemManager::Cbm::List cellBroadcasts();

    Q_DECLARE_PUBLIC(ModemCellBroadcast)
    ModemCellBroadcast *q_ptr;

private Q_SLOTS:
    void onPropertiesChanged(const QString &interface, const QVariantMap &properties, const QStringList &invalidatedProps) override;
    void onCellBroadcastAdded(const QDBusObjectPath &path);
    void onCellBroadcastDeleted(const QDBusObjectPath &path);
};

} // namespace ModemManager

#endif
