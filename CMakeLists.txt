cmake_minimum_required(VERSION 2.8)

project(libmm-qt)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_MODULE_PATH})

if(NOT DEFINED LIB_SUFFIX)
  if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(LIB_SUFFIX "64" CACHE STRING "Define suffix of library directory name (32/64)" )
  else()
    set(LIB_SUFFIX "" CACHE STRING "Define suffix of library directory name (32/64)" )
  endif()
endif()

find_package(Qt4 REQUIRED)

add_definitions(${QT_DEFINITIONS})

include_directories(${CMAKE_SOURCE_DIR} ${CMAKE_BINARY_DIR} ${QT_INCLUDES})

include_directories(
   ${CMAKE_CURRENT_SOURCE_DIR}/dbus
   ${CMAKE_CURRENT_BUILD_DIR}
)

#add_subdirectory(dbus)

set(mmqt_HEADERS
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
    mmqt-export.h
)

set(mmqt_PRIVATE_HEADERS
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

set(mmqt_SRCS
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

qt4_wrap_cpp(mmqt_MOC ${mmqt_HEADERS} ${mmqt_PRIVATE_HEADERS})
qt4_automoc(${DBUS_INTERFACES_FILES})

add_library(mm-qt SHARED ${mmqt_MOC} ${mmqt_SRCS} ${DBUS_INTERFACES_FILES})

target_link_libraries(mm-qt ${QT_QTCORE_LIBRARY} ${QT_QTDBUS_LIBRARY})
install(TARGETS mm-qt DESTINATION lib${LIB_SUFFIX})

install( FILES
${mmqt_HEADERS}
DESTINATION
include/libmm-qt COMPONENT Devel)

set_property(TARGET mm-qt PROPERTY COMPILE_DEFINITIONS MAKE_MMQT_LIB)
set_property(TARGET mm-qt PROPERTY VERSION 0.5.0)
set_property(TARGET mm-qt PROPERTY SOVERSION 0)
