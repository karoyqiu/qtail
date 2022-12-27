cmake_minimum_required(VERSION 3.12)

project(qtail VERSION 0.0.0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# QtCreator supports the following variables for Android, which are identical to qmake Android variables.
# Check https://doc.qt.io/qt/deployment-android.html for more information.
# They need to be set before the find_package( ...) calls below.

#if(ANDROID)
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
#    if (ANDROID_ABI STREQUAL "armeabi-v7a")
#        set(ANDROID_EXTRA_LIBS
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libcrypto.so
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libssl.so)
#    endif()
#endif()

# Add the cmake modules directory to our module path.
#set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR}/cmake)

include(CheckIPOSupported OPTIONAL)
if(COMMAND check_ipo_supported)
    check_ipo_supported(RESULT PROJECT_IS_IPO_SUPPORTED)

    if(PROJECT_IS_IPO_SUPPORTED)
        message(STATUS "IPO is supported.")
        set(CMAKE_INTERPROCEDURAL_OPTIMIZATION_RELEASE ON)
    endif()
endif()

set(QT_LIBS
    Widgets
)

find_package(QT NAMES Qt6 Qt5 COMPONENTS ${QT_LIBS} LinguistTools REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS ${QT_LIBS} LinguistTools REQUIRED)

#configure_file(resource.rc.in resource.rc @ONLY)

set(TS_FILES qtail_zh_CN.ts)

set(PROJECT_SOURCES
    main.cpp
    mainform.cpp mainform.h mainform.ui
    #resource.h "${CMAKE_CURRENT_BINARY_DIR}/resource.rc"
    ${TS_FILES}
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(qtail WIN32
        ${PROJECT_SOURCES}
    )

    qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
else()
    if(ANDROID)
        add_library(qtail SHARED
            ${PROJECT_SOURCES}
        )
    else()
        add_executable(qtail WIN32
            ${PROJECT_SOURCES}
        )
    endif()

    qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
endif()

set_directory_properties(PROPERTIES VS_STARTUP_PROJECT qtail)
target_sources(qtail PRIVATE ${QM_FILES})
target_compile_definitions(qtail
    PRIVATE	QT_DEPRECATED_WARNINGS QT_RESTRICTED_CAST_FROM_ASCII QT_NO_CAST_TO_ASCII
            QT_NO_CAST_FROM_BYTEARRAY QT_USE_QSTRINGBUILDER QT_NO_PROCESS_COMBINED_ARGUMENT_START
            WIN32_LEAN_AND_MEAN
)

foreach(L ${QT_LIBS})
    target_precompile_headers(qtail PRIVATE <Qt${L}>)
    target_link_libraries(qtail PRIVATE Qt${QT_VERSION_MAJOR}::${L})
endforeach()

target_precompile_headers(qtail PRIVATE "<QtWidgets>" "stable.h")
