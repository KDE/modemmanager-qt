/*
 * This file was generated by qdbusxml2cpp version 0.7
 * Command line was: qdbusxml2cpp -N -m -i generictypes.h -p bearerinterface \
 * /home/jgrulich/development/projects/libmm-qt/dbus/introspection/org.freedesktop.ModemManager1.Bearer.xml
 *
 * qdbusxml2cpp is Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
 *
 * This is an auto-generated file.
 * This file may have been hand-edited. Look for HAND-EDIT comments
 * before re-generating it.
 */

#include "bearerinterface.h"

/*
 * Implementation of interface class OrgFreedesktopModemManager1BearerInterface
 */

OrgFreedesktopModemManager1BearerInterface::OrgFreedesktopModemManager1BearerInterface(const QString &service,
                                                                                       const QString &path,
                                                                                       const QDBusConnection &connection,
                                                                                       QObject *parent)
    : QDBusAbstractInterface(service, path, staticInterfaceName(), connection, parent)
{
}

OrgFreedesktopModemManager1BearerInterface::~OrgFreedesktopModemManager1BearerInterface()
{
}

#include "moc_bearerinterface.cpp"
