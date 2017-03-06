project(kclock)

cmake_minimum_required(VERSION 3.3.0 FATAL_ERROR)
set(QT_MIN_VERSION "5.2.0")

if (APPLE)
    option(BUILD_X11 "Build X11 support" OFF)
    set(APPLE_FORCE_X11 ON)
else()
    option(BUILD_X11 "Build X11 support" ON)
endif()

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
    Crash
    I18n
    WidgetsAddons
    WindowSystem
)

if (BUILD_X11)
    find_package(X11 REQUIRED)
    if (X11_FOUND)
        find_package(XCB COMPONENTS XCB)
        find_package(Qt5 ${QT_MIN_VERSION} CONFIG COMPONENTS X11Extras)
        if (Qt5X11Extras_FOUND)
            add_definitions(-DHAVE_X11)
        endif()
    endif()
endif()

include_directories(libkscreensaver)

set(kclock.k5s_SRCS kclock.cpp libkscreensaver/kscreensaver.cpp libkscreensaver/main.cpp)

add_executable(kclock.k5s ${kclock.k5s_SRCS})

target_link_libraries(kclock.k5s
    Qt5::Core Qt5::Gui Qt5::Widgets
    KF5::ConfigWidgets KF5::Crash KF5::I18n KF5::WidgetsAddons KF5::WindowSystem)
if (X11_FOUND)
    if (Qt5X11Extras_FOUND)
        target_link_libraries(kclock.k5s Qt5::X11Extras)
    endif()
    target_link_libraries(kclock.k5s ${XCB_LIBRARIES} ${X11_LIBRARIES})
endif()

install(TARGETS kclock.k5s ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})
install(PROGRAMS org.kde.kclock.desktop DESTINATION ${KDE_INSTALL_APPDIR})

feature_summary(WHAT ALL INCLUDE_QUIET_PACKAGES FATAL_ON_MISSING_REQUIRED_PACKAGES)

