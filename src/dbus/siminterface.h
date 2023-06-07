/*
 * This file was generated by qdbusxml2cpp version 0.7
 * Command line was: qdbusxml2cpp -N -m -i generictypes.h -p siminterface \
 * /home/jgrulich/development/projects/libmm-qt/dbus/introspection/org.freedesktop.ModemManager1.Sim.xml
 *
 * qdbusxml2cpp is Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
 *
 * This is an auto-generated file.
 * Do not edit! All changes made to it will be lost.
 */

#ifndef MODEMMANAGERQT_SIMINTERFACE_H
#define MODEMMANAGERQT_SIMINTERFACE_H

#include "generictypes.h"
#include <QDBusAbstractInterface>
#include <QDBusPendingReply>
#include <QList>
#include <QObject>
#include <QString>
#include <QVariant>

/*
 * Proxy class for interface org.freedesktop.ModemManager1.Sim
 */
class OrgFreedesktopModemManager1SimInterface : public QDBusAbstractInterface
{
    Q_OBJECT
public:
    static inline const char *staticInterfaceName()
#ifdef MMQT_STATIC
    {
        return "org.kde.fakemodem.Sim";
    }
#else
    {
        return "org.freedesktop.ModemManager1.Sim";
    }
#endif
public:
    OrgFreedesktopModemManager1SimInterface(const QString &service, const QString &path, const QDBusConnection &connection, QObject *parent = nullptr);

    ~OrgFreedesktopModemManager1SimInterface() override;

    Q_PROPERTY(QString Imsi READ imsi)
    inline QString imsi() const
    {
        return qvariant_cast<QString>(property("Imsi"));
    }

    Q_PROPERTY(QString OperatorIdentifier READ operatorIdentifier)
    inline QString operatorIdentifier() const
    {
        return qvariant_cast<QString>(property("OperatorIdentifier"));
    }

    Q_PROPERTY(QString OperatorName READ operatorName)
    inline QString operatorName() const
    {
        return qvariant_cast<QString>(property("OperatorName"));
    }

    Q_PROPERTY(QString SimIdentifier READ simIdentifier)
    inline QString simIdentifier() const
    {
        return qvariant_cast<QString>(property("SimIdentifier"));
    }

public Q_SLOTS: // METHODS
    inline QDBusPendingReply<> ChangePin(const QString &old_pin, const QString &new_pin)
    {
        QList<QVariant> argumentList;
        argumentList << QVariant::fromValue(old_pin) << QVariant::fromValue(new_pin);
        return asyncCallWithArgumentList(QLatin1String("ChangePin"), argumentList);
    }

    inline QDBusPendingReply<> EnablePin(const QString &pin, bool enabled)
    {
        QList<QVariant> argumentList;
        argumentList << QVariant::fromValue(pin) << QVariant::fromValue(enabled);
        return asyncCallWithArgumentList(QLatin1String("EnablePin"), argumentList);
    }

    inline QDBusPendingReply<> SendPin(const QString &pin)
    {
        QList<QVariant> argumentList;
        argumentList << QVariant::fromValue(pin);
        return asyncCallWithArgumentList(QLatin1String("SendPin"), argumentList);
    }

    inline QDBusPendingReply<> SendPuk(const QString &puk, const QString &pin)
    {
        QList<QVariant> argumentList;
        argumentList << QVariant::fromValue(puk) << QVariant::fromValue(pin);
        return asyncCallWithArgumentList(QLatin1String("SendPuk"), argumentList);
    }

Q_SIGNALS: // SIGNALS
};

#endif
