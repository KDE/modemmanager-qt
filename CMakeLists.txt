cmake_minimum_required(VERSION 3.5)

set(KF5_VERSION "5.65.0") # handled by release scripts
project(ModemManagerQt VERSION ${KF5_VERSION})

include(FeatureSummary)
find_package(ECM 5.65.0  NO_MODULE)
set_package_properties(ECM PROPERTIES TYPE REQUIRED DESCRIPTION "Extra CMake Modules." URL "https://projects.kde.org/projects/kdesupport/extra-cmake-modules")
feature_summary(WHAT REQUIRED_PACKAGES_NOT_FOUND FATAL_ON_MISSING_REQUIRED_PACKAGES)


set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake ${ECM_MODULE_PATH})

find_package(ModemManager 1.0 REQUIRED)

include(KDEInstallDirs)
include(KDEFrameworkCompilerSettings NO_POLICY_SCOPE)
include(KDECMakeSettings)

set(REQUIRED_QT_VERSION 5.11.0)
find_package(Qt5 ${REQUIRED_QT_VERSION} REQUIRED Core DBus Xml)

add_definitions(${QT_DEFINITIONS})


include(GenerateExportHeader)

include(ECMSetupVersion)
include(ECMGenerateHeaders)
include(ECMAddQch)


option(BUILD_QCH "Build API documentation in QCH format (for e.g. Qt Assistant, Qt Creator & KDevelop)" OFF)
add_feature_info(QCH ${BUILD_QCH} "API documentation in QCH format (for e.g. Qt Assistant, Qt Creator & KDevelop)")

ecm_setup_version(PROJECT VARIABLE_PREFIX MODEMMANAGERQT
                        VERSION_HEADER "${CMAKE_CURRENT_BINARY_DIR}/modemmanagerqt_version.h"
                        PACKAGE_VERSION_FILE "${CMAKE_CURRENT_BINARY_DIR}/KF5ModemManagerQtConfigVersion.cmake"
                        SOVERSION 6)
add_definitions(-DQT_DISABLE_DEPRECATED_BEFORE=0x050d00)
# Enable unit testing
if (BUILD_TESTING)
    add_subdirectory(src/fakemodem)
    add_subdirectory(autotests)
endif ()

##################  examples  ################################
option(BUILD_EXAMPLES "Build example applications" FALSE)
if (BUILD_EXAMPLES)
    add_subdirectory(examples)
endif(BUILD_EXAMPLES)

add_subdirectory(src)

# create a Config.cmake and a ConfigVersion.cmake file and install them
set(CMAKECONFIG_INSTALL_DIR "${CMAKECONFIG_INSTALL_PREFIX}/KF5ModemManagerQt")

if (BUILD_QCH)
    ecm_install_qch_export(
        TARGETS KF5ModemManagerQt_QCH
        FILE KF5ModemManagerQtQchTargets.cmake
        DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
        COMPONENT Devel
    )
    set(PACKAGE_INCLUDE_QCHTARGETS "include(\"\${CMAKE_CURRENT_LIST_DIR}/KF5ModemManagerQtQchTargets.cmake\")")
endif()

include(CMakePackageConfigHelpers)

configure_package_config_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/KF5ModemManagerQtConfig.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/KF5ModemManagerQtConfig.cmake"
  INSTALL_DESTINATION  ${CMAKECONFIG_INSTALL_DIR}
)

install(FILES
  "${CMAKE_CURRENT_BINARY_DIR}/KF5ModemManagerQtConfig.cmake"
  "${CMAKE_CURRENT_BINARY_DIR}/KF5ModemManagerQtConfigVersion.cmake"
  DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
  COMPONENT Devel
)

install(EXPORT KF5ModemManagerQtTargets DESTINATION "${CMAKECONFIG_INSTALL_DIR}" FILE KF5ModemManagerQtTargets.cmake NAMESPACE KF5:: )

install(FILES
   ${CMAKE_CURRENT_BINARY_DIR}/modemmanagerqt_version.h
  DESTINATION ${KF5_INCLUDE_INSTALL_DIR} COMPONENT Devel
)
install(FILES modemmanager-qt.categories  DESTINATION  ${KDE_INSTALL_LOGGINGCATEGORIESDIR})

feature_summary(WHAT ALL   FATAL_ON_MISSING_REQUIRED_PACKAGES)
