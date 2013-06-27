/*
Copyright 2013 Anant Kamath <kamathanant@gmail.com>

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

#ifndef MODEMMANAGER_SMSINTERFACE_H
#define MODEMMANAGER_SMSINTERFACE_H

#include "ModemManagerQt-export.h"

#include <QObject>

class SmsInterfacePrivate;

namespace ModemManager
{
class MODEMMANAGERQT_EXPORT SmsInterface
{
Q_OBJECT
Q_DECLARE_PRIVATE(SmsInterface)

public:
    typedef QSharedPointer<SmsInterface> Ptr;
    typedef QList<Ptr> List;
    
    enum SmsState {
        SMS_STATE_UNKNOWN   = 0,
        SMS_STATE_STORED    = 1,
        SMS_STATE_RECEIVING = 2,
        SMS_STATE_RECEIVED  = 3,
        SMS_STATE_SENDING   = 4,
        SMS_STATE_SENT      = 5,
    };
    
    enum SmsPduType {
        SMS_PDU_TYPE_UNKNOWN       = 0,
        SMS_PDU_TYPE_DELIVER       = 1,
        SMS_PDU_TYPE_SUBMIT        = 2,
        SMS_PDU_TYPE_STATUS_REPORT = 3
    };
    
    enum SmsState {
        SMS_STATE_UNKNOWN   = 0,
        SMS_STATE_STORED    = 1,
        SMS_STATE_RECEIVING = 2,
        SMS_STATE_RECEIVED  = 3,
        SMS_STATE_SENDING   = 4,
        SMS_STATE_SENT      = 5,
    };
    
    enum SmsValidityType {
        SMS_VALIDITY_TYPE_UNKNOWN  = 0,
        SMS_VALIDITY_TYPE_RELATIVE = 1,
        SMS_VALIDITY_TYPE_ABSOLUTE = 2,
        SMS_VALIDITY_TYPE_ENHANCED = 3,
    };
    
    typedef QMap<SmsValidityType, QVariant> SmsValidity;
    
    enum SmsStorage {
        SMS_STORAGE_UNKNOWN = 0,
        SMS_STORAGE_SM      = 1,
        SMS_STORAGE_ME      = 2,
        SMS_STORAGE_MT      = 3,
        SMS_STORAGE_SR      = 4,
        SMS_STORAGE_BM      = 5,
        SMS_STORAGE_TA      = 6,
    };
    
    SmsInterface(const QString & path, QObject * parent);
    ~SmsInterface();

    void send();
    void store(ModemManager::SmsInterface::SmsStorage storage);
    
    ModemManager::SmsInterface::State state() const;
    ModemManager::SmsInterface::SmsPduType pduType() const;
    QString number() const;
    QString text() const;
    QString SMSC() const;
    QByteArray data() const;
    ModemManager::SmsInterface::SmsValidity validity() const;
    int smsClass() const;
    bool deliveryReportRequest() const;
    uint messageReference() const;
    QString timestamp() const;
    QString dischargeTimestamp() const;
    ModemManager::SmsInterface::SmsDeliveryState deliveryState() const;
    ModemManager::SmsInterface::SmsStorage storage() const;    

Q_SIGNALS:
    void stateChanged(ModemManager::SmsInterface::SmsState);
    void deliveryStateChanged((ModemManager::SmsInterface::SmsDeliveryState);
    void storageChanged(ModemManager::SmsInterface::SmsStorage);
    
private Q_SLOTS:
    void propertiesChanged();

};

} // namespace ModemManager


#endif
