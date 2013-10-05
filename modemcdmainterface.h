/*
Copyright 2008,2011 Will Stephenson <wstephenson@kde.org>
Copyright 2010 Lamarque Souza <lamarque@kde.org>
Copyright 2013 Lukas Tinkl <ltinkl@redhat.com>

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

#ifndef MODEMMANAGERQT_MODEMCDMAINTERFACE_H
#define MODEMMANAGERQT_MODEMCDMAINTERFACE_H

#include "ModemManagerQt-export.h"

#include <QObject>
#include <QSharedPointer>
#include <QVariant>

#include "generic-types.h"
#include "interface.h"

class ModemCdmaInterfacePrivate;

namespace ModemManager {

class MODEMMANAGERQT_EXPORT ModemCdmaInterface : public Interface
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(ModemCdmaInterface)
public:
    typedef QSharedPointer<ModemCdmaInterface> Ptr;
    typedef QList<Ptr> List;

    explicit ModemCdmaInterface(const QString & path, QObject * parent = 0);
    ~ModemCdmaInterface();

    // methods
    void activate(const QString & carrierCode);
    void activateManual(const QVariantMap & properties);

    // properties
    MMModemCdmaActivationState activationState() const;
    QString meid() const;
    QString esn() const;
    uint sid() const;
    uint nid() const;
    MMModemCdmaRegistrationState cdma1xRegistrationState() const;
    MMModemCdmaRegistrationState evdoRegistrationState() const;

private slots:
    void onActivationStateChanged(uint activation_state, uint activation_error, const QVariantMap &status_changes);

Q_SIGNALS:
    /**
     * This signal is emitted when the activation info this network changes
     *
     */
    void activationStateChanged(MMModemCdmaActivationState state, MMCdmaActivationError error, const QVariantMap &status_changes);
};
} // namespace ModemManager

#endif
