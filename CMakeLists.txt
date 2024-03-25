cmake_minimum_required(VERSION 3.10)
project(TelltaleDevTool)

find_package(ZLIB REQUIRED)

set(SOURCES
    src/blowfish.c
    src/lua.c
    src/crc64.c
    src/ttarch.c
    src/prop.c
    src/meta.c
)

# Create the executable
add_executable(devtool ${SOURCES})
target_link_libraries(devtool PRIVATE ZLIB::ZLIB)
