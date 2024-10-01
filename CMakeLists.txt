cmake_minimum_required(VERSION 3.10)
project(TelltaleDevTool)

set(CMAKE_BUILD_TYPE Debug)


find_package(ZLIB REQUIRED)

# find_package(PkgConfig REQUIRED)
# pkg_check_modules(GTK4 REQUIRED IMPORTED_TARGET gtk4)
set(SOURCES
    src/main.c
    src/blowfish.c
    src/lua.c
    src/crc64.c
    src/ttarch.c
    src/ttstring.c
    src/intrinsic.c
    src/container.c
    src/dlg.c
    src/tree.c
    src/meta.c
    src/prop.c
    src/landb.c
    src/skeleton.c
    src/chore.c
    src/scene.c
    src/d3dmesh.c
    src/animation.c
    src/linalg.c
    src/keyframe.c
    src/bankwavemap.c
    src/wbox.c
    src/sprite.c
    src/style.c
    src/ptable.c
    src/gltf2.c
)

# Create the executable
add_executable(devtool ${SOURCES})
target_link_libraries(devtool PRIVATE ZLIB::ZLIB)
target_link_libraries(devtool PRIVATE m)
target_compile_options(devtool PRIVATE -Wall -Werror=vla)

# target_link_libraries(devtool PRIVATE PkgConfig::GTK4)

# Add includes
target_include_directories(devtool PRIVATE include)

if(UNIX)
    add_subdirectory(src/unix)
elseif(WIN32)
    add_subdirectory(src/win32)
else()
    message( FATAL_ERROR "Your platform is not supported" )
endif()
