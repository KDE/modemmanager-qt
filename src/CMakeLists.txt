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
    modemsimple.cpp
    modem3gpp.cpp
    modem3gppussd.cpp
    modemcdma.cpp
    modemdevice.cpp
    modemfirmware.cpp
    modemlocation.cpp
    modemmessaging.cpp
    modemtime.cpp
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
   dbus/siminterface.cpp
   dbus/simpleinterface.cpp
   dbus/smsinterface.cpp
   dbus/timeinterface.cpp
   dbus/ussdinterface.cpp
)

if (${MODEMMANAGER_VERSION} VERSION_EQUAL 1.2.0 OR ${MODEMMANAGER_VERSION} VERSION_GREATER 1.2.0)
    set(ModemManagerQt_SRCS
        ${ModemManagerQt_SRCS}
        modemoma.cpp
        modemsignal.cpp
        )

    set(DBUS_INTERFACES_FILES
        ${DBUS_INTERFACES_FILES}
        dbus/omainterface.cpp
        dbus/signalinterface.cpp
        )
endif()

if (${MODEMMANAGER_VERSION} VERSION_EQUAL 1.6.0 OR ${MODEMMANAGER_VERSION} VERSION_GREATER 1.6.0)
    set(ModemManagerQt_SRCS
        ${ModemManagerQt_SRCS}
        modemvoice.cpp
        call.cpp
        )

    set(DBUS_INTERFACES_FILES
        ${DBUS_INTERFACES_FILES}
        dbus/callinterface.cpp
        dbus/voiceinterface.cpp
        )
endif()

ecm_qt_export_logging_category(
    IDENTIFIER MMQT
    CATEGORY_NAME kf.modemmanagerqt
    OLD_CATEGORY_NAMES modemmanager-qt
    DEFAULT_SEVERITY Warning
    DESCRIPTION "ModemManagerQt"
    EXPORT MODEMMANAGERQT
)

add_library(KF5ModemManagerQt SHARED ${ModemManagerQt_SRCS} ${DBUS_INTERFACES_FILES})
generate_export_header(KF5ModemManagerQt BASE_NAME ModemManagerQt)
add_library(KF5::ModemManagerQt ALIAS KF5ModemManagerQt)

target_include_directories(KF5ModemManagerQt INTERFACE "$<INSTALL_INTERFACE:${KDE_INSTALL_INCLUDEDIR_KF5}/ModemManagerQt>")

target_link_libraries(KF5ModemManagerQt PUBLIC Qt5::Core Qt5::DBus Qt5::Xml)

set_target_properties(KF5ModemManagerQt PROPERTIES VERSION ${MODEMMANAGERQT_VERSION_STRING}
                                                   SOVERSION ${MODEMMANAGERQT_SOVERSION}
                                                   EXPORT_NAME ModemManagerQt
)

target_include_directories(KF5ModemManagerQt PUBLIC ${MODEMMANAGER_INCLUDE_DIRS})

########### static lib for tests  ###############
add_library(KF5ModemManagerQt_static STATIC ${ModemManagerQt_SRCS} ${DBUS_INTERFACES_FILES})
set_target_properties(KF5ModemManagerQt_static PROPERTIES COMPILE_FLAGS -DMMQT_STATIC=1)

target_link_libraries(KF5ModemManagerQt_static PUBLIC Qt5::Core Qt5::DBus Qt5::Xml)
target_include_directories(KF5ModemManagerQt_static PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/..;${CMAKE_CURRENT_BINARY_DIR}/..>")

ecm_generate_headers(ModemManagerQt_HEADERS
  HEADER_NAMES
  Call
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
  ModemSimple
  ModemTime
  ModemOma
  ModemVoice
  Sim
  Sms

  REQUIRED_HEADERS ModemManagerQt_HEADERS
)

install(TARGETS KF5ModemManagerQt EXPORT KF5ModemManagerQtTargets ${KF5_INSTALL_TARGETS_DEFAULT_ARGS})

# KF6: install to path having both include visibility layer & namespace dir prefix
# CamelCase includes to ${KDE_INSTALL_INCLUDEDIR_KF5}/ModemManagerQt/ModemManagerQt
# normal headers to ${KDE_INSTALL_INCLUDEDIR_KF5}/ModemManagerQt/modemmanagerqt
# The generated cmake config files are already accidentally pointing to the first layer, by
# target_include_directories(KF5ModemManagerQt INTERFACE "$<INSTALL_INTERFACE:${KDE_INSTALL_INCLUDEDIR_KF5}/ModemManagerQt>")
# next to the actual include base ${KDE_INSTALL_INCLUDEDIR_KF5} being set via KF5_INSTALL_TARGETS_DEFAULT_ARGS
# so this can stay untouched
install(FILES
  ${CMAKE_CURRENT_BINARY_DIR}/modemmanagerqt_export.h
  ${ModemManagerQt_HEADERS}
  DESTINATION ${KDE_INSTALL_INCLUDEDIR_KF5}/ModemManagerQt COMPONENT Devel
)

ecm_qt_install_logging_categories(
    EXPORT MODEMMANAGERQT
    FILE modemmanagerqt.categories
    DESTINATION ${KDE_INSTALL_LOGGINGCATEGORIESDIR}
)

if(BUILD_QCH)
    ecm_add_qch(
        KF5ModemManagerQt_QCH
        NAME ModemManagerQt
        BASE_NAME KF5ModemManagerQt
        VERSION ${KF_VERSION}
        ORG_DOMAIN org.kde
        SOURCES # using only public headers, to cover only public API
            ${ModemManagerQt_HEADERS}
        MD_MAINPAGE "${CMAKE_SOURCE_DIR}/README.md"
        LINK_QCHS
            Qt5Core_QCH
            Qt5DBus_QCH
            Qt5Xml_QCH
        BLANK_MACROS
            MODEMMANAGERQT_EXPORT
            MODEMMANAGERQT_DEPRECATED
            MODEMMANAGERQT_DEPRECATED_EXPORT
        TAGFILE_INSTALL_DESTINATION ${KDE_INSTALL_QTQCHDIR}
        QCH_INSTALL_DESTINATION ${KDE_INSTALL_QTQCHDIR}
        COMPONENT Devel
    )
endif()
