cmake_minimum_required(VERSION 3.10)
project(TelltaleDevTool)

find_package(ZLIB REQUIRED)

set(SOURCES
    src/blowfish.c
    src/lua.c
    src/crc64.c
    src/ttarch.c
    src/meta.c
    src/ttstring.c
    src/intrinsic.c
    src/tree.c
    src/prop.c
    src/container.c
    src/dlg.c
)

# Create the executable
add_executable(devtool ${SOURCES})
target_link_libraries(devtool PRIVATE ZLIB::ZLIB)

# Add includes
target_include_directories(devtool PRIVATE include)