/*
    Copyright 2008 Will Stephenson <wstephenson@kde.org>
    Copyright 2010 Lamarque Souza <lamarque@kde.org>
    Copyright 2013 Lukas Tinkl <ltinkl@redhat.com>
    Copyright 2013 Anant Kamath <kamathanant@gmail.com>
    Copyright 2015 Jan Grulich <jgrulich@redhat.com>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) version 3, or any
    later version accepted by the membership of KDE e.V. (or its
    successor approved by the membership of KDE e.V.), which shall
    act as a proxy defined in Section 6 of version 3 of the license.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef MODEMMANAGER_DBUS_H
#define MODEMMANAGER_DBUS_H

#define DBUS_INTERFACE_PROPS             QLatin1String("org.freedesktop.DBus.Properties")
#define DBUS_INTERFACE_INTROSPECT        QLatin1String("org.freedesktop.DBus.Introspectable")
#define DBUS_INTERFACE_MANAGER           QLatin1String("org.freedesktop.DBus.ObjectManager")

#define MMQT_DBUS_SERVICE   "org.kde.fakemodem"
#define MMQT_DBUS_PATH      "/org/kde/fakemodem"
/* Prefix for object paths */
#define MMQT_DBUS_MODEM_PREFIX  MMQT_DBUS_PATH "/Modem"
#define MMQT_DBUS_BEARER_PREFIX MMQT_DBUS_PATH "/Bearer"
#define MMQT_DBUS_SIM_PREFIX    MMQT_DBUS_PATH "/SIM"
#define MMQT_DBUS_SMS_PREFIX    MMQT_DBUS_PATH "/SMS"
/* Interfaces */
#define MMQT_DBUS_INTERFACE                         "org.kde.fakemodem"
#define MMQT_DBUS_INTERFACE_SIM                     "org.kde.fakemodem.Sim"
#define MMQT_DBUS_INTERFACE_BEARER                  "org.kde.fakemodem.Bearer"
#define MMQT_DBUS_INTERFACE_SMS                     "org.kde.fakemodem.Sms"
#define MMQT_DBUS_INTERFACE_MODEM                   "org.kde.fakemodem.Modem"
#define MMQT_DBUS_INTERFACE_MODEM_SIMPLE            "org.kde.fakemodem.Modem.Simple"
#define MMQT_DBUS_INTERFACE_MODEM_MODEM3GPP         "org.kde.fakemodem.Modem.Modem3gpp"
#define MMQT_DBUS_INTERFACE_MODEM_MODEM3GPP_USSD    "org.kde.fakemodem.Modem.Modem3gpp.Ussd"
#define MMQT_DBUS_INTERFACE_MODEM_MODEMCDMA         "org.kde.fakemodem.Modem.ModemCdma"
#define MMQT_DBUS_INTERFACE_MODEM_MESSAGING         "org.kde.fakemodem.Modem.Messaging"
#define MMQT_DBUS_INTERFACE_MODEM_LOCATION          "org.kde.fakemodem.Modem.Location"
#define MMQT_DBUS_INTERFACE_MODEM_TIME              "org.kde.fakemodem.Modem.Time"
#define MMQT_DBUS_INTERFACE_MODEM_FIRMWARE          "org.kde.fakemodem.Modem.Firmware"
#define MMQT_DBUS_INTERFACE_MODEM_SIGNAL            "org.kde.fakemodem.Modem.Signal"
#define MMQT_DBUS_INTERFACE_MODEM_OMA               "org.kde.fakemodem.Modem.Oma"

#endif
