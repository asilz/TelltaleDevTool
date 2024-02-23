cmake_minimum_required(VERSION 3.10)
project(TelltaleDevTool)

set(SOURCES
    src/blowfish.c
    src/lua.c
    
)

# Create the executable
add_executable(devtool ${SOURCES})
