/*
Copyright 2008,2011 Will Stephenson <wstephenson@kde.org>
Copyright 2010 Lamarque Souza <lamarque@kde.org>

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License as
published by the Free Software Foundation; either version 2 of
the License or (at your option) version 3 or any later version
accepted by the membership of KDE e.V. (or its successor approved
by the membership of KDE e.V.), which shall act as a proxy
defined in Section 14 of version 3 of the license.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef MODEMMANAGER_MODEMCDMAINTERFACE_H
#define MODEMMANAGER_MODEMCDMAINTERFACE_H

#include "QtModemManager-export.h"

#include <QObject>
#include "modeminterface.h"

class ModemCdmaInterfacePrivate;

namespace ModemManager {
class QTMODEMMANAGER_EXPORT ModemCdmaInterface : public ModemInterface
{
Q_OBJECT
Q_DECLARE_PRIVATE(ModemCdmaInterface)
public:
    enum BandClass { Unknown = 0x0, B800 = 0x01, B1900 = 0x2 };

    struct ServingSystemType {
        BandClass bandClass;
        QString band;
        uint systemId;
    };

    enum RegistrationState { UnknownState = 0x0, Registered = 0x1, Home = 0x2, Roaming = 0x3 };

    struct RegistrationStateResult
    {
        RegistrationState cdma_1x_state, evdo_state;
    };

    ModemCdmaInterface(const QString & path, QObject * parent);
    ~ModemCdmaInterface();

    uint getSignalQuality();
    QString getEsn();
    ServingSystemType getServingSystem();
    RegistrationStateResult getRegistrationState();

Q_SIGNALS:
    /**
     * This signal is emitted when the signal quality of this network changes.
     *
     * @param signalQuality the new signal quality value for this network.
     */
    void signalQualityChanged(uint signalQuality);
    /**
     * This signal is emitted when the registration info this network changes
     *
     * @param registrationInfo the new registration info (status, operatorCode, operatorName)
     */
    void registrationStateChanged(const ModemManager::ModemCdmaInterface::RegistrationState cdma_1x_state,
                                  const ModemManager::ModemCdmaInterface::RegistrationState evdo_state);
};
} // namespace ModemManager

#endif

