/*
Copyright 2008,2011 Will Stephenson <wstephenson@kde.org>
Copyright 2010 Lamarque Souza <lamarque@kde.org>

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

#ifndef MODEMMANAGER_MODEMGSMSMSINTERFACE_H
#define MODEMMANAGER_MODEMGSMSMSINTERFACE_H

#include "ModemManagerQt-export.h"

#include <QObject>
#include "modeminterface.h"

class ModemGsmSmsInterfacePrivate;

namespace ModemManager
{
class MODEMMANAGERQT_EXPORT ModemGsmSmsInterface : public ModemInterface
{
Q_OBJECT
Q_DECLARE_PRIVATE(ModemGsmSmsInterface)

public:
    typedef QSharedPointer<ModemGsmSmsInterface> Ptr;
    typedef QList<Ptr> List;
    ModemGsmSmsInterface(const QString & path, QObject * parent);
    ~ModemGsmSmsInterface();

    void deleteSms(int index);
    QVariantMap get(int index);
    int getFormat();
    void setFormat(int format);
    QString getSmsc();
    QList<QVariantMap> list();
    void save(const QVariantMap & properties);
    void send(const QVariantMap & properties);
    void sendFromStorage(int index);
    void setIndication(int mode, int mt, int bm, int ds, int brf);

Q_SIGNALS:
    void smsReceived(int index, bool complete);
    void completed(int index, bool completed);
};

} // namespace ModemManager


#endif
