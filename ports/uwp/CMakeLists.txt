set(PK2_UWP_TARGET PekkaKana2UWP)
set(PK2_UWP_DEPS_BIN "${PK2_UWP_DEPS_ROOT}/bin")

add_executable(${PK2_UWP_TARGET} WIN32
    main.cpp
)

target_include_directories(${PK2_UWP_TARGET} PRIVATE
    "${CMAKE_SOURCE_DIR}/src"
    "${PK2_UWP_DEPS_ROOT}/include"
    "${PK2_UWP_DEPS_ROOT}/include/SDL2"
)

target_link_libraries(${PK2_UWP_TARGET} PRIVATE
    pk2_core
)

set(PK2_UWP_RUNTIME_DLLS
    "${PK2_UWP_DEPS_BIN}/SDL2.dll"
    "${PK2_UWP_DEPS_BIN}/SDL2_image.dll"
    "${PK2_UWP_DEPS_BIN}/SDL2_mixer_ext.dll"
    "${PK2_UWP_DEPS_BIN}/zip.dll"
    "${PK2_UWP_DEPS_BIN}/lua.dll"
    "${PK2_UWP_DEPS_BIN}/z.dll"
    "${PK2_UWP_DEPS_BIN}/libpng16.dll"
    "${PK2_UWP_DEPS_BIN}/ogg.dll"
    "${PK2_UWP_DEPS_BIN}/vorbis.dll"
    "${PK2_UWP_DEPS_BIN}/vorbisenc.dll"
    "${PK2_UWP_DEPS_BIN}/vorbisfile.dll"
    "${PK2_UWP_DEPS_BIN}/libxmp.dll"
    "${PK2_UWP_DEPS_BIN}/wavpackdll.dll"
    "${PK2_UWP_DEPS_BIN}/bz2.dll"
)

set(PK2_UWP_PKG_ASSETS
    "${CMAKE_CURRENT_SOURCE_DIR}/Assets/LargeTile.png"
    "${CMAKE_CURRENT_SOURCE_DIR}/Assets/SmallTile.png"
    "${CMAKE_CURRENT_SOURCE_DIR}/Assets/SplashScreen.png"
    "${CMAKE_CURRENT_SOURCE_DIR}/Assets/Square44x44Logo.png"
    "${CMAKE_CURRENT_SOURCE_DIR}/Assets/Square150x150Logo.png"
    "${CMAKE_CURRENT_SOURCE_DIR}/Assets/StoreLogo.png"
    "${CMAKE_CURRENT_SOURCE_DIR}/Assets/WideTile.png"
)

foreach(PK2_UWP_FILE ${PK2_UWP_RUNTIME_DLLS} "${CMAKE_CURRENT_SOURCE_DIR}/Package.appxmanifest" ${PK2_UWP_PKG_ASSETS})
    if(NOT EXISTS "${PK2_UWP_FILE}")
        message(FATAL_ERROR "Missing UWP package file: ${PK2_UWP_FILE}")
    endif()
endforeach()

source_group("BinDeps" FILES ${PK2_UWP_RUNTIME_DLLS})
set_source_files_properties(${PK2_UWP_RUNTIME_DLLS} PROPERTIES
    VS_COPY_TO_OUT_DIR Always
    VS_DEPLOYMENT_CONTENT TRUE
    VS_DEPLOYMENT_LOCATION "."
)

source_group("PkgAssets" FILES ${PK2_UWP_PKG_ASSETS})
set_source_files_properties(${PK2_UWP_PKG_ASSETS} PROPERTIES
    VS_DEPLOYMENT_CONTENT TRUE
    VS_DEPLOYMENT_LOCATION "Assets"
)

file(GLOB_RECURSE PK2_RES_FILES
    RELATIVE "${CMAKE_SOURCE_DIR}/res"
    "${CMAKE_SOURCE_DIR}/res/*"
)

foreach(PK2_RES_FILE ${PK2_RES_FILES})
    set(PK2_FULL_RES_FILE "${CMAKE_SOURCE_DIR}/res/${PK2_RES_FILE}")
    get_filename_component(PK2_RES_DEST "${PK2_RES_FILE}" DIRECTORY)

    set_source_files_properties("${PK2_FULL_RES_FILE}" PROPERTIES
        VS_COPY_TO_OUT_DIR Always
        VS_DEPLOYMENT_CONTENT TRUE
        VS_DEPLOYMENT_LOCATION "res/${PK2_RES_DEST}"
    )

    target_sources(${PK2_UWP_TARGET} PRIVATE "${PK2_FULL_RES_FILE}")
endforeach()

target_sources(${PK2_UWP_TARGET} PRIVATE
    ${PK2_UWP_RUNTIME_DLLS}
    ${PK2_UWP_PKG_ASSETS}
    "${CMAKE_CURRENT_SOURCE_DIR}/Package.appxmanifest"
)

set_target_properties(${PK2_UWP_TARGET} PROPERTIES
    VS_SDK_REFERENCES "Microsoft.VCLibs, Version=14.0"
)
