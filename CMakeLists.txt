cmake_minimum_required(VERSION 2.8)

project(ModemManagerQt)

include(FindPkgConfig)

set(VERSION 0.7.991)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_MODULE_PATH})

set(LIB_SUFFIX "" CACHE STRING "Define suffix of library directory name (32/64)" )

#find_package(Qt4 REQUIRED)
find_package(ECM 0.0.8 REQUIRED NO_MODULE)

include(CMakePackageConfigHelpers)
include(FeatureSummary)
#include(WriteBasicConfigVersionFile)

# where to look first for cmake modules, before ${CMAKE_ROOT}/Modules/ is checked
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${ECM_MODULE_PATH})

#include(ECMOptionalAddSubdirectory)

find_package(Qt5Core REQUIRED NO_MODULE)
find_package(Qt5DBus REQUIRED NO_MODULE)
find_package(Qt5Network REQUIRED NO_MODULE)
find_package(Qt5Widgets REQUIRED NO_MODULE)
find_package(Qt5Xml REQUIRED NO_MODULE)

include_directories(${Qt5Widgets_INCLUDE_DIRS} ${Qt5DBus_INCLUDE_DIRS} ${Qt5Core_INCLUDE_DIRS} ${Qt5Network_INCLUDE_DIRS} ${Qt5Xml_INCLUDE_DIRS} )

pkg_check_modules(MODEMMANAGER REQUIRED ModemManager>=0.7.9)

add_definitions(${QT_DEFINITIONS})

add_definitions(-DQT_USE_FAST_CONCATENATION -DQT_USE_FAST_OPERATOR_PLUS)
add_definitions(-DQT_NO_URL_CAST_FROM_STRING)

remove_definitions(-DQT3_SUPPORT_WARNINGS -DQT3_SUPPORT)
remove_definitions(-DQT_NO_CAST_FROM_ASCII -DQT_STRICT_ITERATORS -DQT_NO_CAST_FROM_BYTEARRAY -DQT_NO_KEYWORDS)

include(CheckCXXCompilerFlag)
check_cxx_compiler_flag("-Werror=return-type" HAVE_GCC_ERROR_RETURN_TYPE)
if (HAVE_GCC_ERROR_RETURN_TYPE)
    set(CMAKE_CXX_FLAGS "-Werror=return-type")
endif()

include_directories(${CMAKE_SOURCE_DIR} ${CMAKE_BINARY_DIR} ${QT_INCLUDES} ${CMAKE_INCLUDE_PATH} ${MODEMMANAGER_INCLUDE_DIRS})
link_directories(${CMAKE_LIBRARY_PATH})

include_directories(
   ${CMAKE_CURRENT_SOURCE_DIR}
   ${CMAKE_CURRENT_SOURCE_DIR}/dbus
   ${CMAKE_CURRENT_BUILD_DIR}
)

#add_subdirectory(dbus)

set(ModemManagerQt_HEADERS
    dbus/generic-types.h
    ModemManagerQt-export.h
    bearer.h
    interface.h
    manager.h
    modem.h
    modem3gpp.h
    modem3gppussd.h
    modemcdma.h
    modemdevice.h
    modemlocation.h
    modemmessaging.h
    modemtime.h
    sim.h
    sms.h
)

set(ModemManagerQt_PRIVATE_HEADERS
    bearer_p.h
    interface_p.h
    manager_p.h
    modem_p.h
    modem3gpp_p.h
    modem3gppussd_p.h
    modemcdma_p.h
    modemdevice_p.h
    modemlocation_p.h
    modemmessaging_p.h
    modemtime_p.h
    sim_p.h
    sms_p.h
)

set(ModemManagerQt_SRCS
    bearer.cpp
    interface.cpp
    manager.cpp
    modem.cpp
    modem3gpp.cpp
    modem3gppussd.cpp
    modemcdma.cpp
    modemdevice.cpp
    modemlocation.cpp
    modemmessaging.cpp
    modemtime.cpp
    sim.cpp
    sms.cpp
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

#qt4_wrap_cpp(ModemManagerQt_MOC ${ModemManagerQt_HEADERS} ${ModemManagerQt_PRIVATE_HEADERS} ${DBUS_INTERFACES_HEADERS})
#qt_automoc(${DBUS_INTERFACES_FILES})

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
