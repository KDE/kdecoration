project(decoration-api)
set(PROJECT_VERSION "5.12.5")

cmake_minimum_required(VERSION 2.8.12 FATAL_ERROR)

find_package(ECM 0.0.11 REQUIRED NO_MODULE)
# where to look first for cmake modules, before ${CMAKE_ROOT}/Modules/ is checked
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${ECM_MODULE_PATH} ${ECM_KDE_MODULE_DIR})

include(FeatureSummary)
include(GenerateExportHeader)
include(ECMPackageConfigHelpers)
include(ECMSetupVersion)
include(ECMGenerateHeaders)

ecm_setup_version(${PROJECT_VERSION} VARIABLE_PREFIX KDECORATION2
                        VERSION_HEADER "${CMAKE_CURRENT_BINARY_DIR}/kdecoration2_version.h"
                        PACKAGE_VERSION_FILE "${CMAKE_CURRENT_BINARY_DIR}/KDecoration2ConfigVersion.cmake"
                        SOVERSION 5)

#dependencies
set(REQUIRED_QT_VERSION 5.4.0)
find_package(Qt5 ${REQUIRED_QT_VERSION} CONFIG REQUIRED COMPONENTS
    Core
    Gui
    Test
)

include(KDEInstallDirs)
include(KDEFrameworkCompilerSettings NO_POLICY_SCOPE)
include(KDECMakeSettings)

# require at least gcc 4.8
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    if ("${CMAKE_CXX_COMPILER_VERSION}" VERSION_LESS "4.8")
        message(SEND_ERROR "Version ${CMAKE_CXX_COMPILER_VERSION} of the ${CMAKE_CXX_COMPILER_ID} C++ compiler is not supported. Please use version 4.8 or later.")
    endif()
endif()

set(KDECORATION2_INCLUDEDIR "${CMAKE_INSTALL_INCLUDEDIR}/KDecoration2")

# Subdirectories
add_subdirectory(src)
add_subdirectory(autotests)

# create a Config.cmake and a ConfigVersion.cmake file and install them
set(CMAKECONFIG_INSTALL_DIR "${CMAKECONFIG_INSTALL_PREFIX}/KDecoration2")

ecm_configure_package_config_file("${CMAKE_CURRENT_SOURCE_DIR}/KDecoration2Config.cmake.in"
                                  "${CMAKE_CURRENT_BINARY_DIR}/KDecoration2Config.cmake"
                                  INSTALL_DESTINATION  ${CMAKECONFIG_INSTALL_DIR}
                                  )

install(FILES  "${CMAKE_CURRENT_BINARY_DIR}/KDecoration2Config.cmake"
               "${CMAKE_CURRENT_BINARY_DIR}/KDecoration2ConfigVersion.cmake"
        DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
        COMPONENT Devel )

install(EXPORT KDecoration2Targets DESTINATION "${CMAKECONFIG_INSTALL_DIR}" FILE KDecoration2Targets.cmake NAMESPACE KDecoration2:: )


install(FILES ${CMAKE_CURRENT_BINARY_DIR}/kdecoration2_version.h
        DESTINATION ${KF5_INCLUDE_INSTALL_DIR} COMPONENT Devel )

feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)
