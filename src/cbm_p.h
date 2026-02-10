/*
    SPDX-FileCopyrightText: 2026 Devin Lin <devin@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef MODEMMANAGER_CBM_P_H
#define MODEMMANAGER_CBM_P_H

#include "cbm.h"
#include "dbus/cbminterface.h"

namespace ModemManager
{
class CbmPrivate : public QObject
{
    Q_OBJECT
public:
    explicit CbmPrivate(const QString &path, Cbm *q);
    OrgFreedesktopModemManager1CbmInterface cbmIface;

    QString uni;
    MMCbmState state;
    QString text;
    uint channel;
    uint messageCode;
    uint update;

    Q_DECLARE_PUBLIC(Cbm)
    Cbm *q_ptr;

private Q_SLOTS:
    void onPropertiesChanged(const QString &interface, const QVariantMap &properties, const QStringList &invalidatedProps);
};

} // namespace ModemManager

#endif
