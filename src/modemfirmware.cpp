/*
    SPDX-FileCopyrightText: 2014 Lukas Tinkl <ltinkl@redhat.com>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "modemfirmware.h"
#include "modemfirmware_p.h"
#ifdef MMQT_STATIC
#include "dbus/fakedbus.h"
#else
#include "dbus/dbus.h"
#endif

ModemManager::ModemFirmwarePrivate::ModemFirmwarePrivate(const QString &path, ModemFirmware *q)
    : InterfacePrivate(path, q)
#ifdef MMQT_STATIC
    , modemFirmwareIface(QLatin1String(MMQT_DBUS_SERVICE), path, QDBusConnection::sessionBus())
#else
    , modemFirmwareIface(QLatin1String(MMQT_DBUS_SERVICE), path, QDBusConnection::systemBus())
#endif
    , q_ptr(q)
{
    if (modemFirmwareIface.isValid()) {
        updateSettings = modemFirmwareIface.updateSettings();
    }
}

ModemManager::ModemFirmware::ModemFirmware(const QString &path, QObject *parent)
    : Interface(*new ModemFirmwarePrivate(path, this), parent)
{
#ifdef MMQT_STATIC
    QDBusConnection::sessionBus().connect(QLatin1String(MMQT_DBUS_SERVICE),
                                          path,
                                          QLatin1String(DBUS_INTERFACE_PROPS),
                                          QStringLiteral("PropertiesChanged"),
                                          this,
                                          SLOT(onPropertiesChanged(QString, QVariantMap, QStringList)));
#else
    QDBusConnection::systemBus().connect(QLatin1String(MMQT_DBUS_SERVICE),
                                         path,
                                         QLatin1String(DBUS_INTERFACE_PROPS),
                                         QStringLiteral("PropertiesChanged"),
                                         this,
                                         SLOT(onPropertiesChanged(QString, QVariantMap, QStringList)));
#endif
}

ModemManager::ModemFirmware::~ModemFirmware()
{
}

QDBusPendingReply<QString, ModemManager::QVariantMapList> ModemManager::ModemFirmware::listImages()
{
    Q_D(ModemFirmware);
    return d->modemFirmwareIface.List();
}

void ModemManager::ModemFirmware::selectImage(const QString &uniqueid)
{
    Q_D(ModemFirmware);
    d->modemFirmwareIface.Select(uniqueid);
}

ModemManager::FirmwareUpdateSettings ModemManager::ModemFirmware::updateSettings() const
{
    Q_D(const ModemFirmware);
    return d->updateSettings;
}

void ModemManager::ModemFirmwarePrivate::onPropertiesChanged(const QString &interfaceName, const QVariantMap &properties, const QStringList &invalidatedProps)
{
    Q_UNUSED(invalidatedProps);
    if (interfaceName == QLatin1String(MMQT_DBUS_INTERFACE_MODEM_FIRMWARE)) {
        QVariantMap::const_iterator it = properties.constFind(QLatin1String(MM_MODEM_FIRMWARE_PROPERTY_UPDATESETTINGS));
        if (it != properties.constEnd()) {
            updateSettings = it->value<FirmwareUpdateSettings>();
            Q_Q(ModemFirmware);
            Q_EMIT q->updateSettingsChanged(updateSettings);
        }
    }
}

#include "moc_modemfirmware.cpp"
