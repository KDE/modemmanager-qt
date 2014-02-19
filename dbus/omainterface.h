/*
 * This file was generated by qdbusxml2cpp version 0.7
 * Command line was: qdbusxml2cpp -N -m -i generic-types.h -p omainterface /home/jgrulich/development/projects/libmm-qt/dbus/introspection/org.freedesktop.ModemManager1.Modem.Oma.xml
 *
 * qdbusxml2cpp is Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
 *
 * This is an auto-generated file.
 * Do not edit! All changes made to it will be lost.
 */

#ifndef MODEMMANAGERQT_OMAINTERFACE_H
#define MODEMMANAGERQT_OMAINTERFACE_H

#include <QtCore/QObject>
#include <QtCore/QByteArray>
#include <QtCore/QList>
#include <QtCore/QMap>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QVariant>
#include <QtDBus/QtDBus>
#include "generic-types.h"

/*
 * Proxy class for interface org.freedesktop.ModemManager1.Modem.Oma
 */
class OrgFreedesktopModemManager1ModemOmaInterface: public QDBusAbstractInterface
{
    Q_OBJECT
public:
    static inline const char *staticInterfaceName()
    { return "org.freedesktop.ModemManager1.Modem.Oma"; }

public:
    OrgFreedesktopModemManager1ModemOmaInterface(const QString &service, const QString &path, const QDBusConnection &connection, QObject *parent = 0);

    ~OrgFreedesktopModemManager1ModemOmaInterface();

    Q_PROPERTY(uint Features READ features)
    inline uint features() const
    { return qvariant_cast< uint >(property("Features")); }

    Q_PROPERTY(OmaSessionTypes PendingNetworkInitiatedSessions READ pendingNetworkInitiatedSessions)
    inline OmaSessionTypes pendingNetworkInitiatedSessions() const
    { return qvariant_cast< OmaSessionTypes >(property("PendingNetworkInitiatedSessions")); }

    Q_PROPERTY(int SessionState READ sessionState)
    inline int sessionState() const
    { return qvariant_cast< int >(property("SessionState")); }

    Q_PROPERTY(uint SessionType READ sessionType)
    inline uint sessionType() const
    { return qvariant_cast< uint >(property("SessionType")); }

public Q_SLOTS: // METHODS
    inline QDBusPendingReply<> AcceptNetworkInitiatedSession(uint session_id, bool accept)
    {
        QList<QVariant> argumentList;
        argumentList << QVariant::fromValue(session_id) << QVariant::fromValue(accept);
        return asyncCallWithArgumentList(QLatin1String("AcceptNetworkInitiatedSession"), argumentList);
    }

    inline QDBusPendingReply<> CancelSession()
    {
        QList<QVariant> argumentList;
        return asyncCallWithArgumentList(QLatin1String("CancelSession"), argumentList);
    }

    inline QDBusPendingReply<> Setup(uint features)
    {
        QList<QVariant> argumentList;
        argumentList << QVariant::fromValue(features);
        return asyncCallWithArgumentList(QLatin1String("Setup"), argumentList);
    }

    inline QDBusPendingReply<> StartClientInitiatedSession(uint session_type)
    {
        QList<QVariant> argumentList;
        argumentList << QVariant::fromValue(session_type);
        return asyncCallWithArgumentList(QLatin1String("StartClientInitiatedSession"), argumentList);
    }

Q_SIGNALS: // SIGNALS
    void SessionStateChanged(int old_session_state, int new_session_state, uint session_state_failed_reason);
};

#endif