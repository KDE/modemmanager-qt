include(FindPkgConfig)
pkg_check_modules(MODEMMANAGER REQUIRED ModemManager>=1.0)

find_package(KF6ModemManagerQt ${KF_VERSION} REQUIRED)

find_package(Qt6 ${QT_MIN_VERSION} CONFIG REQUIRED COMPONENTS
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
    KF6::ModemManagerQt
    Qt6::Core
)
