/*
    SPDX-FileCopyrightText: 2026 Devin Lin <devin@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef MODEMMANAGERQT_CBM_H
#define MODEMMANAGERQT_CBM_H

#include <modemmanagerqt_export.h>

#include <QObject>
#include <QSharedPointer>

#include "generictypes.h"

namespace ModemManager
{
class CbmPrivate;

/*!
 * \class ModemManager::Cbm
 *
 * \inheaderfile ModemManagerQt/Cbm
 *
 * \inmodule ModemManagerQt
 *
 * \brief Provides an interface to access a cell broadcast message.
 *
 * \note MMCbmState enum is defined in <ModemManager/ModemManager-enums.h>
 *
 * \since 6.24.0
 */
class MODEMMANAGERQT_EXPORT Cbm : public QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(Cbm)

public:
    /*!
     * \typedef ModemManager::Cbm::Ptr
     */
    typedef QSharedPointer<Cbm> Ptr;
    /*!
     * \typedef ModemManager::Cbm::List
     */
    typedef QList<Ptr> List;

    /*!
     */
    explicit Cbm(const QString &path, QObject *parent = nullptr);
    ~Cbm() override;

    /*!
     * Returns the D-Bus object path of the cell broadcast message.
     */
    QString uni() const;

    /*!
     * Returns the MMCbmState value describing the state of the message.
     */
    MMCbmState state() const;

    /*!
     * Returns the message text, in UTF-8.
     */
    QString text() const;

    /*!
     * Returns the channel (or message identifier) specifying the source and
     * type of the cell broadcast message.
     */
    uint channel() const;

    /*!
     * Returns the message code of this CBM. The message code differentiates
     * between CBMs from the same channel.
     */
    uint messageCode() const;

    /*!
     * Returns the update number of this CBM. The update number allows to
     * update CBMs with the same channel and message code.
     */
    uint update() const;

    /*!
     * Sets the timeout in milliseconds for all async method DBus calls.
     * -1 means the default DBus timeout (usually 25 seconds).
     */
    void setTimeout(int timeout);

    /*!
     * Returns the current value of the DBus timeout in milliseconds.
     * -1 means the default DBus timeout (usually 25 seconds).
     */
    int timeout() const;

Q_SIGNALS:
    /*!
     */
    void stateChanged(MMCbmState state);
    /*!
     */
    void textChanged(const QString &text);
    /*!
     */
    void channelChanged(uint channel);
    /*!
     */
    void messageCodeChanged(uint messageCode);
    /*!
     */
    void updateChanged(uint update);

private:
    CbmPrivate *const d_ptr;
};

} // namespace ModemManager

#endif
