cmake_minimum_required(VERSION 2.8)

project(ModemManagerQt)

include(FindPkgConfig)

set(VERSION 0.7.991)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_MODULE_PATH})

set(LIB_SUFFIX "" CACHE STRING "Define suffix of library directory name (32/64)" )

find_package(Qt4 REQUIRED)
pkg_check_modules(MODEMMANAGER REQUIRED ModemManager>=0.8)

add_definitions(${QT_DEFINITIONS})

include_directories(${CMAKE_SOURCE_DIR} ${CMAKE_BINARY_DIR} ${QT_INCLUDES} ${CMAKE_INCLUDE_PATH} ${MODEMMANAGER_INCLUDE_DIRS})
link_directories(${CMAKE_LIBRARY_PATH})

include_directories(
   ${CMAKE_CURRENT_SOURCE_DIR}
   ${CMAKE_CURRENT_SOURCE_DIR}/dbus
   ${CMAKE_CURRENT_BUILD_DIR}
)

#add_subdirectory(dbus)

set(ModemManagerQt_HEADERS
    manager.h
    modeminterface.h
    modemlocationinterface.h
    modemcdmainterface.h
    modemgsmcardinterface.h
    modemgsmnetworkinterface.h
    modemgsmussdinterface.h
    modemtimeinterface.h
    modemmessaginginterface.h
    modembearerinterface.h
    smsinterface.h
    ModemManagerQt-export.h
    dbus/generic-types.h
)

set(ModemManagerQt_PRIVATE_HEADERS
    manager_p.h
    modemcdmainterface_p.h
    modemgsmcardinterface_p.h
    modemgsmnetworkinterface_p.h
    modemgsmussdinterface_p.h
    modeminterface_p.h
    modemlocationinterface_p.h
    modemtimeinterface_p.h
    modemmessaginginterface_p.h
    modembearerinterface_p.h
    smsinterface_p.h
)

set(ModemManagerQt_SRCS
   manager.cpp
   modeminterface.cpp
   modemlocationinterface.cpp
   modemcdmainterface.cpp
   modemgsmcardinterface.cpp
   modemgsmnetworkinterface.cpp
   modemgsmussdinterface.cpp
   modemtimeinterface.cpp
   modemmessaginginterface.cpp
   modembearerinterface.cpp
   smsinterface.cpp
)

set( DBUS_INTERFACES_FILES
   dbus/generic-types.cpp
   dbus/Bearer.cpp
   dbus/Location.cpp
   dbus/Manager.cpp
   dbus/Messaging.cpp
   dbus/Modem.cpp
   dbus/Modem3gpp.cpp
   dbus/Modem3gppUssd.cpp
   dbus/ModemCdma.cpp
   dbus/ModemSimple.cpp
   dbus/Sim.cpp
   dbus/Sms.cpp
   dbus/Time.cpp
   dbus/dbus_manager.cpp
)

set( DBUS_INTERFACES_HEADERS
   dbus/generic-types.h
   dbus/Bearer.h
   dbus/Location.h
   dbus/Manager.h
   dbus/Messaging.h
   dbus/Modem.h
   dbus/Modem3gpp.h
   dbus/Modem3gppUssd.h
   dbus/ModemCdma.h
   dbus/ModemSimple.h
   dbus/Sim.h
   dbus/Sms.h
   dbus/Time.h
   dbus/dbus_manager.h
)

qt4_wrap_cpp(ModemManagerQt_MOC ${ModemManagerQt_HEADERS} ${ModemManagerQt_PRIVATE_HEADERS} ${DBUS_INTERFACES_HEADERS})
qt4_automoc(${DBUS_INTERFACES_FILES})

add_library(ModemManagerQt SHARED ${ModemManagerQt_MOC} ${ModemManagerQt_SRCS} ${DBUS_INTERFACES_FILES})

target_link_libraries(ModemManagerQt ${QT_QTCORE_LIBRARY} ${QT_QTDBUS_LIBRARY})
install(TARGETS ModemManagerQt DESTINATION lib${LIB_SUFFIX})

install( FILES
${ModemManagerQt_HEADERS}
DESTINATION
include/ModemManagerQt COMPONENT Devel)

set_property(TARGET ModemManagerQt PROPERTY COMPILE_DEFINITIONS MAKE_MODEMMANAGERQT_LIB)
set_property(TARGET ModemManagerQt PROPERTY VERSION ${VERSION})
set_property(TARGET ModemManagerQt PROPERTY SOVERSION 0)

message(STATUS "Writing pkg-config file...")
configure_file(${CMAKE_SOURCE_DIR}/ModemManagerQt.pc.cmake ${CMAKE_BINARY_DIR}/ModemManagerQt.pc @ONLY)
install(FILES ${CMAKE_BINARY_DIR}/ModemManagerQt.pc DESTINATION "${CMAKE_INSTALL_PREFIX}/lib${LIB_SUFFIX}/pkgconfig/")
