include_directories(${MODEMMANAGER_INCLUDE_DIRS})
include_directories(
   ${CMAKE_SOURCE_DIR}/src/settings
)

set(fakeModem_SRCS
    bearer.cpp
    fakemodem.cpp
    modem.cpp
    modem3gpp.cpp
    modem3gppussd.cpp
    modemcdma.cpp
    modemlocation.cpp
    modemoma.cpp
    modemsimple.cpp
    objectmanager.cpp
)

add_library(fakeModem STATIC ${fakeModem_SRCS})

target_link_libraries(fakeModem
    KF5ModemManagerQt_static
    Qt5::Core
    Qt5::DBus)
