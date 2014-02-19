/*
    Copyright 2014 Lukas Tinkl <ltinkl@redhat.com>

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

#ifndef MODEMMANAGERQT_MODEMFIRMWARE_H
#define MODEMMANAGERQT_MODEMFIRMWARE_H

#include "ModemManagerQt-export.h"
#include "interface.h"

#include <QObject>
#include <QSharedPointer>

class ModemFirmwarePrivate;

namespace ModemManager
{
/**
 * @brief The ModemFirmware class
 *
 * This class allows clients to select or install firmware images on modems.
 *
 * Firmware slots and firmware images are identified by arbitrary opaque strings.
 *
 * @since 1.1.94
 */
class MODEMMANAGERQT_EXPORT ModemFirmware: public Interface
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(ModemFirmware)

public:
    typedef QSharedPointer<ModemFirmware> Ptr;
    typedef QList<Ptr> List;

    explicit ModemFirmware(const QString &path, QObject *parent = 0);
    ~ModemFirmware();

    /**
     * List installed firmware images.
     *
     * Depending on the type of modem, installed images may be stored on the host or the modem.
     * Installed images can be selected non-destructively.
     *
     * @return @param selected (QString)
     *         The unique name of the selected firmware image, or the empty string if no image is selected.
     *         @param installed (QVariantMapList)
     *         A map of dictionaries containing the properties of the installed firmware images.
     */
    QDBusPendingReply<QString, QVariantMapList> listImages();

    /**
     * Selects a different firmware image to use, and immediately resets the modem so that it
     * begins using the new firmware image.
     *
     * The method will fail if the identifier does not match any of the names returned by listImages(),
     * or if the image could not be selected for some reason.
     *
     * @param uniqueid The unique ID of the firmware image to select.
     */
    void selectImage(const QString &uniqueid);
};

} // namespace ModemManager

#endif
