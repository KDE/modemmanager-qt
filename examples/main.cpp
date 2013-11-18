/*
  Copyright 2013 Jan Grulich <jgrulich@redhat.com>

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

#include <ModemManagerQt/interface.h>
#include <ModemManagerQt/manager.h>
#include <ModemManagerQt/modemdevice.h>
#include <ModemManagerQt/modemmessaging.h>
#include <ModemManagerQt/modem.h>
#include <ModemManagerQt/sim.h>
#include <ModemManagerQt/sms.h>

#include <QDebug>

int main(int argc, char *argv[])
{
    qDebug() << "List of Modem Devices:";
    foreach (ModemManager::ModemDevice::Ptr modemdevice, ModemManager::modemDevices()) {
        ModemManager::Modem::Ptr modemInterface = modemdevice->modemInterface();
        if (modemInterface) {
            qDebug() << "Modem: " << modemInterface->manufacturer() << modemInterface->model();
        } else {
            qDebug() << "Modem: " << modemdevice->uni();
        }

        QStringList interfaces;
        if (modemInterface) {
            interfaces << "modem";
        }
        if (modemdevice->hasInterface(ModemManager::ModemDevice::GsmInterface)) {
            interfaces << "gsm";
        }
        if (modemdevice->hasInterface(ModemManager::ModemDevice::GsmUssdInterface)) {
            interfaces << "gsm ussd";
        }
        if (modemdevice->hasInterface(ModemManager::ModemDevice::CdmaInterface)) {
            interfaces << "cdma";
        }
        if (modemdevice->hasInterface(ModemManager::ModemDevice::MessagingInterface)) {
            interfaces << "messaging";
        }
        if (modemdevice->hasInterface(ModemManager::ModemDevice::LocationInterface)) {
            interfaces << "location";
        }
        if (modemdevice->hasInterface(ModemManager::ModemDevice::TimeInterface)) {
            interfaces << "time";
        }

        qDebug() << "Interfaces: " << interfaces;
        qDebug() << "SIM Cards: ";
        if (modemdevice->sims().isEmpty()) {
            qDebug() << "   no SIM card";
        } else {
            foreach (ModemManager::Sim::Ptr sim, modemdevice->sims()) {
                qDebug() << "   " << sim->uni() << "Operator: " << sim->operatorName();
            }
        }

        qDebug() << "Messages: ";
        ModemManager::ModemMessaging::Ptr messaging = modemdevice->messagingInterface();
        if (messaging && !messaging->messages().isEmpty()) {
            ModemManager::Sms::List messages = messaging->messages();
            foreach (ModemManager::Sms::Ptr sms, messages) {
                qDebug() << "   " << sms->number() << sms->text();
            }
        } else {
            qDebug() << "   no messaging interface or message";
        }

        if (messaging) {
            ModemManager::ModemMessaging::Message msg;
            msg.number = "number";
            msg.text = "message text";
            QString sms = messaging->createMessage(msg);
            if (!sms.isEmpty()) {
                qDebug() << "Message created";
                ModemManager::Sms::Ptr tmp = messaging->findMessage(sms);
                if (tmp) {
                    qDebug() << "Message text:";
                    qDebug() << tmp->text();
                    QDBusPendingReply<> reply = tmp->send();
                    reply.waitForFinished();
                    if (reply.isError()) {
                        qDebug() << reply.error().message();
                    }
                }
            }
        }
    }
}
