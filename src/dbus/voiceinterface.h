/*
 * This file was generated by qdbusxml2cpp version 0.8
 * Command line was: qdbusxml2cpp -N -m -i generictypes.h -p voiceinterface ./introspection/org.freedesktop.ModemManager1.Modem.Voice.xml
 *
 * qdbusxml2cpp is Copyright (C) 2017 The Qt Company Ltd.
 *
 * This is an auto-generated file.
 * Do not edit! All changes made to it will be lost.
 */

#ifndef VOICEINTERFACE_H
#define VOICEINTERFACE_H

#include <QObject>
#include <QList>
#include <QString>
#include <QVariant>
#include "generictypes.h"

/*
 * Proxy class for interface org.freedesktop.ModemManager1.Modem.Voice
 */
class OrgFreedesktopModemManager1ModemVoiceInterface: public QDBusAbstractInterface
{
    Q_OBJECT
public:
    static inline const char *staticInterfaceName()
    { return "org.freedesktop.ModemManager1.Modem.Voice"; }

public:
    OrgFreedesktopModemManager1ModemVoiceInterface(const QString &service, const QString &path, const QDBusConnection &connection, QObject *parent = nullptr);

    ~OrgFreedesktopModemManager1ModemVoiceInterface();

    Q_PROPERTY(QList<QDBusObjectPath> Calls READ calls)
    inline QList<QDBusObjectPath> calls() const
    { return qvariant_cast< QList<QDBusObjectPath> >(property("Calls")); }

public Q_SLOTS: // METHODS
    inline QDBusPendingReply<QDBusObjectPath> CreateCall(const QVariantMap &properties)
    {
        QList<QVariant> argumentList;
        argumentList << QVariant::fromValue(properties);
        return asyncCallWithArgumentList(QStringLiteral("CreateCall"), argumentList);
    }

    inline QDBusPendingReply<> DeleteCall(const QDBusObjectPath &path)
    {
        QList<QVariant> argumentList;
        argumentList << QVariant::fromValue(path);
        return asyncCallWithArgumentList(QStringLiteral("DeleteCall"), argumentList);
    }

    inline QDBusPendingReply<QList<QDBusObjectPath> > ListCalls()
    {
        QList<QVariant> argumentList;
        return asyncCallWithArgumentList(QStringLiteral("ListCalls"), argumentList);
    }

Q_SIGNALS: // SIGNALS
    void CallAdded(const QDBusObjectPath &path);
    void CallDeleted(const QDBusObjectPath &path);
};

#endif
