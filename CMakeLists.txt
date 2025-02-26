cmake_minimum_required(VERSION 3.16)

project(sm05 VERSION 0.1 LANGUAGES CXX)

# Установка кодировки
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fexec-charset=UTF-8")

# Проверка наличия переменной окружения VULKAN_SDK
message(STATUS "VULKAN_SDK:  ${VULKAN_SDK}")
if(NOT DEFINED ENV{VULKAN_SDK})
    #message(FATAL_ERROR "VULKAN_SDK не установлена. Пожалуйста, установите Vulkan SDK и настройте переменную окружения VULKAN_SDK.")
    # FATAL_ERROR ведет к прерыванию процесса сборки
    message(STATUS "VULKAN_SDK не установлена. Пожалуйста, установите Vulkan SDK и настройте переменную окружения VULKAN_SDK.")

endif()

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
find_package(QT NAMES Qt6 REQUIRED COMPONENTS Core Gui Widgets Sql)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Gui Widgets Sql)
set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
)
message(STATUS "QT_VERSION_MAJOR: ${QT_VERSION_MAJOR}")
if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    message(STATUS "QT_VERSION_MAJOR")
    qt_add_executable(sm05
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        mainwindowstate.cpp
        mainwindowstate.h
        dbfacade.h
        dbfacade.cpp
        resourcesSM05.qrc
        calculategrade.h
        calculategrade.cpp
        checkinservices.h
        checkinservices.cpp
    )
   message(STATUS "PROJECT_SOURCES: ${PROJECT_SOURCES}")
else()
    if(ANDROID)
        add_library(sm05 SHARED
            ${PROJECT_SOURCES}
        )
    else()
        add_executable(sm05
            ${PROJECT_SOURCES}
        )
    endif()
endif()
target_link_libraries(sm05 PRIVATE Qt${QT_VERSION_MAJOR}::Core
                                   Qt${QT_VERSION_MAJOR}::Widgets
                                   Qt${QT_VERSION_MAJOR}::Gui
                                   Qt${QT_VERSION_MAJOR}::Sql)
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.sm05)
endif()

set_target_properties(sm05 PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)
set(DEBUG_LIB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/debug")
set(RELEASE_LIB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/release")
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(LIB_DIR ${DEBUG_LIB_DIR})
else()
    set(LIB_DIR ${RELEASE_LIB_DIR})
endif()
target_link_libraries(sm05 PRIVATE ${LIB_DIR}/createTasks.dll ${LIB_DIR}/custtb.dll)
include_directories(${LIB_DIR}/createTasks ${LIB_DIR}/custtb)
include(GNUInstallDirs)
install(TARGETS sm05
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(sm05)
endif()
#set(CMAKE_CXX_COMPILER "clazy")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic")
