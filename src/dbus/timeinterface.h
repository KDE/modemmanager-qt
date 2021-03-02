/*
 * This file was generated by qdbusxml2cpp version 0.7
 * Command line was: qdbusxml2cpp -N -m -i generictypes.h -p timeinterface \
 * /home/jgrulich/development/projects/libmm-qt/dbus/introspection/org.freedesktop.ModemManager1.Modem.Time.xml
 *
 * qdbusxml2cpp is Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
 *
 * This is an auto-generated file.
 * Do not edit! All changes made to it will be lost.
 */

#ifndef MODEMMANAGERQT_TIMEINTERFACE_H
#define MODEMMANAGERQT_TIMEINTERFACE_H

#include "generictypes.h"
#include <QList>
#include <QObject>
#include <QString>
#include <QVariant>

/*
 * Proxy class for interface org.freedesktop.ModemManager1.Modem.Time
 */
class OrgFreedesktopModemManager1ModemTimeInterface : public QDBusAbstractInterface
{
    Q_OBJECT
public:
    static inline const char *staticInterfaceName()
#ifdef MMQT_STATIC
    {
        return "org.kde.fakemodem.Modem.Time";
    }
#else
    {
        return "org.freedesktop.ModemManager1.Modem.Time";
    }
#endif
public:
    OrgFreedesktopModemManager1ModemTimeInterface(const QString &service, const QString &path, const QDBusConnection &connection, QObject *parent = nullptr);

    ~OrgFreedesktopModemManager1ModemTimeInterface();

    Q_PROPERTY(QVariantMap NetworkTimezone READ networkTimezone)
    inline QVariantMap networkTimezone() const
    {
        return qvariant_cast<QVariantMap>(property("NetworkTimezone"));
    }

public Q_SLOTS: // METHODS
    inline QDBusPendingReply<QString> GetNetworkTime()
    {
        QList<QVariant> argumentList;
        return asyncCallWithArgumentList(QLatin1String("GetNetworkTime"), argumentList);
    }

Q_SIGNALS: // SIGNALS
    void NetworkTimeChanged(const QString &time);
};

#endif
