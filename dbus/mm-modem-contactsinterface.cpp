/*
 * This file was generated by qdbusxml2cpp version 0.7
 * Command line was: qdbusxml2cpp -m introspection/org.freedesktop.ModemManager1.Modem.Contacts.xml -p mm-modem-contactsinterface
 *
 * qdbusxml2cpp is Copyright (C) 2012 Digia Plc and/or its subsidiary(-ies).
 *
 * This is an auto-generated file.
 * This file may have been hand-edited. Look for HAND-EDIT comments
 * before re-generating it.
 */

#include "mm-modem-contactsinterface.h"

/*
 * Implementation of interface class OrgFreedesktopModemManager1ModemContactsInterface
 */

OrgFreedesktopModemManager1ModemContactsInterface::OrgFreedesktopModemManager1ModemContactsInterface(const QString &service, const QString &path, const QDBusConnection &connection, QObject *parent)
    : QDBusAbstractInterface(service, path, staticInterfaceName(), connection, parent)
{
}

OrgFreedesktopModemManager1ModemContactsInterface::~OrgFreedesktopModemManager1ModemContactsInterface()
{
}


#include "mm-modem-contactsinterface.moc"
