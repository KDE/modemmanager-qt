/*
    Copyright 2013 Lukas Tinkl <ltinkl@redhat.com>
    Copyright 2013 Jan Grulich <jgrulich@redhat.com>

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

#ifndef MODEMMANAGERQT_MODEM_P_H
#define MODEMMANAGERQT_MODEM_P_H

#include <QObject>
#include <QStringList>
#include <QMap>

#include "modem.h"
#include "interface.h"

class ModemPrivate
{
public:
    ModemPrivate(const QString &path);
    virtual ~ModemPrivate();
    QString uni;
    QMap<ModemManager::Modem::InterfaceType, ModemManager::Interface::Ptr> interfaceList;
    ModemManager::Interface::List interfaces() ;
    ModemManager::Interface::Ptr interface(ModemManager::Modem::InterfaceType type);
    ModemManager::Interface::Ptr createInterface(ModemManager::Modem::InterfaceType type);
};

#endif
