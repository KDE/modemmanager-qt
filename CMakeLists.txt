cmake_minimum_required(VERSION 2.8)

project(ModemManagerQt)

set(VERSION 0.5.0)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_MODULE_PATH})

set(LIB_SUFFIX "" CACHE STRING "Define suffix of library directory name (32/64)" )

find_package(Qt4 REQUIRED)

add_definitions(${QT_DEFINITIONS})

include_directories(${CMAKE_SOURCE_DIR} ${CMAKE_BINARY_DIR} ${QT_INCLUDES} ${CMAKE_INCLUDE_PATH})
link_directories(${CMAKE_LIBRARY_PATH})

include_directories(
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
    modemgsmcontactsinterface.h
    modemgsmnetworkinterface.h
    modemgsmsmsinterface.h
    modemgsmhsointerface.h
    modemgsmussdinterface.h
    ModemManagerQt-export.h
)

set(ModemManagerQt_PRIVATE_HEADERS
    manager_p.h
    modemcdmainterface_p.h
    modemgsmcardinterface_p.h
    modemgsmcontactsinterface_p.h
    modemgsmhsointerface_p.h
    modemgsmnetworkinterface_p.h
    modemgsmsmsinterface_p.h
    modemgsmussdinterface_p.h
    modeminterface_p.h
    modemlocationinterface_p.h
)

set(ModemManagerQt_SRCS
   manager.cpp
   modeminterface.cpp
   modemlocationinterface.cpp
   modemcdmainterface.cpp
   modemgsmcardinterface.cpp
   modemgsmcontactsinterface.cpp
   modemgsmnetworkinterface.cpp
   modemgsmsmsinterface.cpp
   modemgsmhsointerface.cpp
   modemgsmussdinterface.cpp
)

set( DBUS_INTERFACES_FILES
   dbus/generic-types.cpp
   dbus/mm-manager-clientinterface.cpp
   dbus/mm-modeminterface.cpp
   dbus/mm-modem-simpleinterface.cpp
   dbus/mm-modem-locationinterface.cpp
   dbus/mm-modem-cdmainterface.cpp
   dbus/mm-modem-gsm-cardinterface.cpp
   dbus/mm-modem-gsm-contactsinterface.cpp
   dbus/mm-modem-gsm-networkinterface.cpp
   dbus/mm-modem-gsm-smsinterface.cpp
   dbus/mm-modem-gsm-hsointerface.cpp
   dbus/mm-modem-gsm-ussdinterface.cpp
)

qt4_wrap_cpp(ModemManagerQt_MOC ${ModemManagerQt_HEADERS} ${ModemManagerQt_PRIVATE_HEADERS})
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
