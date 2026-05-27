cmake_minimum_required(VERSION 3.20)

option(UWP_BUILD "Build the minimal UWP port" OFF)

if(UWP_BUILD)
    set(CMAKE_SYSTEM_NAME WindowsStore)
    set(CMAKE_SYSTEM_VERSION 10.0.19041.0 CACHE STRING "Target Windows SDK version for UWP builds" FORCE)
endif()

project(PekkaKana2 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE PK2_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)

add_library(pk2_core STATIC ${PK2_SOURCES})

target_include_directories(pk2_core PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/src"
)

target_compile_definitions(pk2_core PUBLIC
    PK2_USE_ZIP
    PK2_USE_LUA
    PK2_VERSION="v0.316"
)

if(MSVC)
    target_compile_definitions(pk2_core PUBLIC
        _CRT_SECURE_NO_WARNINGS
        _USE_MATH_DEFINES
        NOMINMAX
    )
endif()

if(UWP_BUILD)
    set(PK2_MANIFEST_DEPS_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/vcpkg_installed/x64-uwp")
    if(EXISTS "${PK2_MANIFEST_DEPS_ROOT}")
        set(PK2_UWP_DEPS_ROOT_DEFAULT "${PK2_MANIFEST_DEPS_ROOT}")
    elseif(DEFINED ENV{VCPKG_ROOT})
        file(TO_CMAKE_PATH "$ENV{VCPKG_ROOT}/installed/x64-uwp" PK2_UWP_DEPS_ROOT_DEFAULT)
    else()
        set(PK2_UWP_DEPS_ROOT_DEFAULT "")
    endif()

    set(PK2_UWP_DEPS_ROOT "${PK2_UWP_DEPS_ROOT_DEFAULT}" CACHE PATH "Root of the x64-uwp dependency install")

    target_compile_definitions(pk2_core PUBLIC
        UWP_BUILD=1
    )

    target_include_directories(pk2_core PUBLIC
        "${PK2_UWP_DEPS_ROOT}/include"
        "${PK2_UWP_DEPS_ROOT}/include/SDL2"
    )

    target_link_directories(pk2_core PUBLIC
        "${PK2_UWP_DEPS_ROOT}/lib"
    )

    target_link_libraries(pk2_core PUBLIC
        SDL2.lib
        SDL2_image.lib
        SDL2_mixer_ext.lib
        zip.lib
        lua.lib
        WindowsApp.lib
        OneCore.lib
    )

    add_subdirectory(ports/uwp)
endif()
