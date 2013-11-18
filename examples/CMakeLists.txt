project(exampleModemManagerQt)
cmake_minimum_required(VERSION 2.6)

include(FindPkgConfig)
find_package(Qt4 REQUIRED)

pkg_check_modules(MODEMMANAGERQT REQUIRED ModemManagerQt)
if (NOT MODEMMANAGERQT_FOUND)
    message(FATAL_ERROR "ERROR: ModemManagerQt not found. http://projects.kde.org/search?q=networkmanager&scope=all&all_words=1&projects=1&submit=Submit")
endif (NOT MODEMMANAGERQT_FOUND)

pkg_check_modules(MODEMMANAGER REQUIRED ModemManager)

include_directories(${CMAKE_SOURCE_DIR} ${CMAKE_BINARY_DIR} ${QTNETWORK_INCLUDE_DIRS} ${MODEMMANAGERQT_INCLUDE_DIRS} ${MODEMMANAGER_INCLUDE_DIRS} ${QT_INCLUDES})
link_directories(${QTNETWORK_LIBRARY_DIRS} ${MODEMMANAGERQT_LIBRARY_DIRS} ${QT_QTDBUS_LIBRARY_DIRS})

set(exampleModemManagerQt_SRCS
   main.cpp
)

add_executable(exampleModemManagerQt ${exampleModemManagerQt_SRCS})
target_link_libraries(exampleModemManagerQt ModemManagerQt QtCore)
