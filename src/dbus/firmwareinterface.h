/*
 * This file was generated by qdbusxml2cpp version 0.7
 * Command line was: qdbusxml2cpp -N -m -i generictypes.h -p firmwareinterface introspection/org.freedesktop.ModemManager1.Modem.Firmware.xml
 *
 * qdbusxml2cpp is Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
 *
 * This is an auto-generated file.
 * Do not edit! All changes made to it will be lost.
 */

#ifndef MODEMMANAGERQT_FIRMWAREINTERFACE_H
#define MODEMMANAGERQT_FIRMWAREINTERFACE_H

#include <QObject>
#include <QList>
#include <QString>
#include <QVariant>
#include "generictypes.h"

/*
 * Proxy class for interface org.freedesktop.ModemManager1.Modem.Firmware
 */
class OrgFreedesktopModemManager1ModemFirmwareInterface: public QDBusAbstractInterface
{
    Q_OBJECT
public:
    static inline const char *staticInterfaceName()
#ifdef MMQT_STATIC
    { return "org.kde.fakemodem.Modem.Firmware"; }
#else
    { return "org.freedesktop.ModemManager1.Modem.Firmware"; }
#endif
public:
    OrgFreedesktopModemManager1ModemFirmwareInterface(const QString &service, const QString &path, const QDBusConnection &connection, QObject *parent = nullptr);

    ~OrgFreedesktopModemManager1ModemFirmwareInterface();

public Q_SLOTS: // METHODS
    inline QDBusPendingReply<QString, ModemManager::QVariantMapList> List()
    {
        QList<QVariant> argumentList;
        return asyncCallWithArgumentList(QLatin1String("List"), argumentList);
    }
    inline QDBusPendingReply<> Select(const QString &uniqueid)
    {
        QList<QVariant> argumentList;
        argumentList << QVariant::fromValue(uniqueid);
        return asyncCallWithArgumentList(QLatin1String("Select"), argumentList);
    }

Q_SIGNALS: // SIGNALS
};

#endif
