/*
Copyright 2008,2011 Will Stephenson <wstephenson@kde.org>
Copyright 2010-2011 Lamarque Souza <lamarque@kde.org>
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

#ifndef MODEMMANAGER_SIM_H
#define MODEMMANAGER_SIM_H

#include "ModemManagerQt-export.h"

#include <QObject>
#include <QSharedPointer>
#include <QDBusPendingReply>

class SimPrivate;

namespace ModemManager
{
/**
 * @brief The Sim class
 *
 * The SIM class handles communication with SIM, USIM, and RUIM (CDMA SIM) cards.
 */
class MODEMMANAGERQT_EXPORT Sim : public QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(Sim)

public:
    typedef QSharedPointer<Sim> Ptr;
    typedef QList<Ptr> List;

    explicit Sim(const QString &path, QObject *parent = 0);
    ~Sim();

    /**
     * @return An obfuscated SIM identifier based on the IMSI or the ICCID.
     *
     * This may be available before the PIN has been entered depending on the device itself.
     */
    QString simIdentifier() const;

    /**
     * @return The IMSI of the SIM card, if any.
     */
    QString imsi() const;

    /**
     * @return The ID of the network operator, as given by the SIM card, if known.
     */
    QString operatorIdentifier() const;

    /**
     * @return The name of the network operator, as given by the SIM card, if known.
     */
    QString operatorName() const;

    /**
     * Send the PIN to unlock the SIM card.
     * @param pin A string containing the PIN code.
     */
    QDBusPendingReply<> sendPin(const QString &pin);

    /**
     * Send the PUK and a new PIN to unlock the SIM card.
     * @param puk A string containing the PUK code.
     * @param pin A string containing the PIN code.
     */
    QDBusPendingReply<> sendPuk(const QString &puk, const QString &pin);

    /**
     * Enable or disable the PIN checking.
     * @param pin A string containing the PIN code.
     * @param enabled TRUE to enable PIN checking, FALSE otherwise.
     */
    QDBusPendingReply<> enablePin(const QString &pin, bool enabled);

    /**
     * Change the PIN code.
     * @param oldPin A string containing the current PIN code.
     * @param newPin A string containing the new PIN code.
     */
    QDBusPendingReply<> changePin(const QString &oldPin, const QString &newPin);

    QString uni() const;

private:
    SimPrivate *const d_ptr;
};
} // namespace ModemManager

#endif
