cmake_minimum_required(VERSION 2.8.12)

include(FindPkgConfig)
pkg_check_modules(MODEMMANAGER REQUIRED ModemManager>=1.0)

find_package(KF5ModemManagerQt ${KF_VERSION} REQUIRED)

find_package(Qt5 ${QT_MIN_VERSION} CONFIG REQUIRED COMPONENTS
    Core
    DBus
    Xml
)

include_directories(${MODEMMANAGER_INCLUDE_DIRS})

set(exampleModemManagerQt_SRCS
   main.cpp
)

add_executable(exampleModemManagerQt ${exampleModemManagerQt_SRCS})
target_link_libraries(exampleModemManagerQt
    KF5::ModemManagerQt
    Qt5::Core
)
