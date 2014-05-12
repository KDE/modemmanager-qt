#add_subdirectory(dbus)

include_directories(
    ${MODEMMANAGER_INCLUDE_DIRS}
)

set(ModemManagerQt_SRCS
    bearer.cpp
    generictypes.cpp
    interface.cpp
    manager.cpp
    modem.cpp
    modem3gpp.cpp
    modem3gppussd.cpp
    modemcdma.cpp
    modemdevice.cpp
    modemfirmware.cpp
    modemlocation.cpp
    modemmessaging.cpp
    modemsignal.cpp
    modemtime.cpp
    modemoma.cpp
    sim.cpp
    sms.cpp
)

set(DBUS_INTERFACES_FILES
   dbus/bearerinterface.cpp
   dbus/dbus_manager.cpp
   dbus/firmwareinterface.cpp
   dbus/locationinterface.cpp
   dbus/messaginginterface.cpp
   dbus/modem3gppinterface.cpp
   dbus/modemcdmainterface.cpp
   dbus/modeminterface.cpp
   dbus/modemmanager1interface.cpp
   dbus/omainterface.cpp
   dbus/signalinterface.cpp
   dbus/siminterface.cpp
   dbus/simpleinterface.cpp
   dbus/smsinterface.cpp
   dbus/timeinterface.cpp
   dbus/ussdinterface.cpp
)

add_library(KF5ModemManagerQt SHARED ${ModemManagerQt_SRCS} ${DBUS_INTERFACES_FILES})
generate_export_header(KF5ModemManagerQt BASE_NAME ModemManagerQt)
add_library(KF5::ModemManagerQt ALIAS KF5ModemManagerQt)

target_include_directories(KF5ModemManagerQt INTERFACE "$<INSTALL_INTERFACE:${KF5_INCLUDE_INSTALL_DIR}/ModemManagerQt>")

target_link_libraries(KF5ModemManagerQt PUBLIC Qt5::Core Qt5::DBus Qt5::Xml)

set_target_properties(KF5ModemManagerQt PROPERTIES VERSION ${MODEMMANAGERQT_VERSION_STRING}
                                                   SOVERSION ${MODEMMANAGERQT_SOVERSION}
                                                   EXPORT_NAME ModemManagerQt
)

ecm_generate_headers(ModemManagerQt_HEADERS
  HEADER_NAMES
  Bearer
  GenericTypes
  Interface
  Manager
  Modem
  Modem3Gpp
  Modem3GppUssd
  ModemCdma
  ModemDevice
  ModemFirmware
  ModemLocation
  ModemMessaging
  ModemSignal
  ModemTime
  ModemOma
  Sim
  Sms

  REQUIRED_HEADERS ModemManagerQt_HEADERS
)

install(TARGETS KF5ModemManagerQt EXPORT KF5ModemManagerQtTargets ${KF5_INSTALL_TARGETS_DEFAULT_ARGS})

install(FILES
  ${CMAKE_CURRENT_BINARY_DIR}/modemmanagerqt_export.h
  ${ModemManagerQt_HEADERS}
  DESTINATION ${KF5_INCLUDE_INSTALL_DIR}/ModemManagerQt COMPONENT Devel
)

include(ECMGeneratePriFile)
ecm_generate_pri_file(BASE_NAME ModemManagerQt LIB_NAME KF5ModemManagerQt DEPS "core" FILENAME_VAR PRI_FILENAME)
install(FILES ${PRI_FILENAME} DESTINATION ${ECM_MKSPECS_INSTALL_DIR})
