project(kclock)

cmake_minimum_required(VERSION 3.3.0 FATAL_ERROR)
set(QT_MIN_VERSION "5.2.0")

find_package(ECM 1.7.0 REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH})

add_definitions(-DTRANSLATION_DOMAIN="kclock")

include(KDEInstallDirs)
include(KDECompilerSettings NO_POLICY_SCOPE)
include(KDECMakeSettings)
include(ECMInstallIcons)
include(FeatureSummary)
include(ECMAddAppIcon)

find_package(Qt5 ${QT_MIN_VERSION} CONFIG REQUIRED COMPONENTS
    Core
    Gui
    Widgets
)

find_package(KF5 REQUIRED COMPONENTS
    Config
    ConfigWidgets
    I18n
    WidgetsAddons
)

if (NOT APPLE)
    find_package(X11)
    if (X11_FOUND)
        find_package(XCB COMPONENTS XCB)
    endif()
endif()

set(kclock.k5s_SRCS kclock.cpp )

add_executable(kclock.k5s ${kclock.k5s_SRCS})

target_link_libraries(kclock.k5s
    Qt5::Core Qt5::Gui Qt5::Widgets
    KF5::ConfigWidgets KF5::I18n KF5::WidgetsAddons)
if (X11_FOUND)
    target_link_libraries(kclock.k5s ${XCB_LIBRARIES})
endif()

install(TARGETS kclock.k5s ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})
install(PROGRAMS org.kde.kclock.desktop DESTINATION ${KDE_INSTALL_APPDIR})

feature_summary(WHAT ALL INCLUDE_QUIET_PACKAGES FATAL_ON_MISSING_REQUIRED_PACKAGES)

