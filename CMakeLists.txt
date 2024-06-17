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
    src/style.c
    src/ptable.c
    src/wbox.c
)

if(UNIX)
    list(APPEND SOURCES src/stream_unix.c)
endif(UNIX)

if(MSVC)
    list(APPEND SOURCES src/stream_windows.c)
endif(MSVC)

# Create the executable
add_executable(devtool ${SOURCES})
target_link_libraries(devtool PRIVATE ZLIB::ZLIB)

# target_link_libraries(devtool PRIVATE PkgConfig::GTK4)

# Add includes
target_include_directories(devtool PRIVATE include)
