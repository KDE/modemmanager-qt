cmake_minimum_required(VERSION 2.8)

project(ModemManagerQt)

include(FindPkgConfig)

set(VERSION 1.0.1)
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_MODULE_PATH})
set(LIB_SUFFIX "" CACHE STRING "Define suffix of library directory name (32/64)" )
set(LIB_DESTINATION "${CMAKE_INSTALL_PREFIX}/lib${LIB_SUFFIX}" CACHE STRING
    "Library directory name")
set(PKGCONFIG_INSTALL_PREFIX "${LIB_DESTINATION}/pkgconfig/" CACHE STRING
    "Base directory for pkgconfig files")

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

find_package(Qt4 REQUIRED)
pkg_check_modules(MODEMMANAGER REQUIRED ModemManager>=0.8)

add_definitions(${QT_DEFINITIONS})

include(CheckCXXCompilerFlag)
check_cxx_compiler_flag("-Werror=return-type" HAVE_GCC_ERROR_RETURN_TYPE)
if (HAVE_GCC_ERROR_RETURN_TYPE)
    set(CMAKE_CXX_FLAGS "-Werror=return-type")
endif()

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/dbus
    ${CMAKE_CURRENT_BUILD_DIR}
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_BINARY_DIR}
    ${CMAKE_INCLUDE_PATH}
    ${QT_INCLUDES}
    ${MODEMMANAGER_INCLUDE_DIRS}
)

link_directories(${CMAKE_LIBRARY_PATH})

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

qt4_wrap_cpp(ModemManagerQt_MOC ${ModemManagerQt_HEADERS} ${ModemManagerQt_PRIVATE_HEADERS} ${DBUS_INTERFACES_HEADERS})
qt4_automoc(${DBUS_INTERFACES_FILES})

add_library(ModemManagerQt SHARED ${ModemManagerQt_MOC} ${ModemManagerQt_SRCS} ${DBUS_INTERFACES_FILES})

target_link_libraries(ModemManagerQt ${QT_QTCORE_LIBRARY} ${QT_QTXML_LIBRARY} ${QT_QTDBUS_LIBRARY})
install(TARGETS ModemManagerQt DESTINATION ${LIB_DESTINATION})

install(FILES ${ModemManagerQt_HEADERS} DESTINATION include/ModemManagerQt COMPONENT Devel)

set_property(TARGET ModemManagerQt PROPERTY COMPILE_DEFINITIONS MAKE_MODEMMANAGERQT_LIB)
set_property(TARGET ModemManagerQt PROPERTY VERSION ${VERSION})
set_property(TARGET ModemManagerQt PROPERTY SOVERSION 0)

message(STATUS "Writing pkg-config file...")
configure_file(${CMAKE_SOURCE_DIR}/ModemManagerQt.pc.cmake ${CMAKE_BINARY_DIR}/ModemManagerQt.pc @ONLY)
install(FILES ${CMAKE_BINARY_DIR}/ModemManagerQt.pc DESTINATION "${PKGCONFIG_INSTALL_PREFIX}")

##################  apidox  ################################
find_package(Doxygen)

if(DOXYGEN_EXECUTABLE)
  configure_file(${CMAKE_SOURCE_DIR}/.Doxyfile.cmake ${CMAKE_BINARY_DIR}/Doxyfile)

  if(EXISTS ${QT_DOC_DIR}/html)
    set(QTDOCS "${QT_DOC_DIR}/html")
  else(EXISTS ${QT_DOC_DIR}/html)
    set(QTDOCS "http://qt-project.org/doc/latest/")
  endif(EXISTS ${QT_DOC_DIR}/html)

  add_custom_target(
    apidox
    COMMAND ${DOXYGEN_EXECUTABLE} Doxyfile)
endif(DOXYGEN_EXECUTABLE)

##################  examples  ################################
option(BUILD_EXAMPLES "Build example applications" FALSE)
if (BUILD_EXAMPLES)
    add_subdirectory(examples)
endif(BUILD_EXAMPLES)

##################  uninstall target  ########################
configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake_uninstall.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
    IMMEDIATE @ONLY)

add_custom_target(uninstall
    COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake)
