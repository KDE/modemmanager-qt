/*
 * This file was generated by qdbusxml2cpp version 0.7
 * Command line was: qdbusxml2cpp -N -m -i generictypes.h -p simpleinterface \
 * /home/jgrulich/development/projects/libmm-qt/dbus/introspection/org.freedesktop.ModemManager1.Modem.Simple.xml
 *
 * qdbusxml2cpp is Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
 *
 * This is an auto-generated file.
 * This file may have been hand-edited. Look for HAND-EDIT comments
 * before re-generating it.
 */

#include "simpleinterface.h"

/*
 * Implementation of interface class OrgFreedesktopModemManager1ModemSimpleInterface
 */

OrgFreedesktopModemManager1ModemSimpleInterface::OrgFreedesktopModemManager1ModemSimpleInterface(const QString &service,
                                                                                                 const QString &path,
                                                                                                 const QDBusConnection &connection,
                                                                                                 QObject *parent)
    : QDBusAbstractInterface(service, path, staticInterfaceName(), connection, parent)
{
}

OrgFreedesktopModemManager1ModemSimpleInterface::~OrgFreedesktopModemManager1ModemSimpleInterface()
{
}

#include "moc_simpleinterface.cpp"
