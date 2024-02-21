cmake_minimum_required(VERSION 3.10)
project(Blowfish)

set(SOURCES
    src/blowfish.c
    
)

# Create the executable
add_executable(blowfish ${SOURCES})
