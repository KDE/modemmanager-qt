include_directories(
    ${MODEMMANAGER_INCLUDE_DIRS}
)

include(ECMAddTests)

find_package(Qt5 ${REQUIRED_QT_VERSION} NO_MODULE REQUIRED Test)
set_package_properties(Qt5Test PROPERTIES PURPOSE "Required for autotests")

ecm_add_test(bearerpropertiestest.cpp LINK_LIBRARIES Qt5::Test KF5ModemManagerQt_static fakeModem)
ecm_add_test(modemtest.cpp LINK_LIBRARIES Qt5::Test KF5ModemManagerQt_static fakeModem)
ecm_add_test(modempropertiestest.cpp LINK_LIBRARIES Qt5::Test KF5ModemManagerQt_static fakeModem)
ecm_add_test(modem3gpppropertiestest.cpp LINK_LIBRARIES Qt5::Test KF5ModemManagerQt_static fakeModem)
ecm_add_test(modem3gppussdpropertiestest.cpp LINK_LIBRARIES Qt5::Test KF5ModemManagerQt_static fakeModem)
ecm_add_test(modemcdmapropertiestest.cpp LINK_LIBRARIES Qt5::Test KF5ModemManagerQt_static fakeModem)
ecm_add_test(modemlocationpropertiestest.cpp LINK_LIBRARIES Qt5::Test KF5ModemManagerQt_static fakeModem)
ecm_add_test(modemtimepropertiestest.cpp LINK_LIBRARIES Qt5::Test KF5ModemManagerQt_static fakeModem)

if (${MODEMMANAGER_VERSION} VERSION_EQUAL 1.2.0 OR ${MODEMMANAGER_VERSION} VERSION_GREATER 1.2.0)
    ecm_add_test(modemomapropertiestest.cpp LINK_LIBRARIES Qt5::Test KF5ModemManagerQt_static fakeModem)
endif()
