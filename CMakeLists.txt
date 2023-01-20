cmake_minimum_required(VERSION 3.19.6)
project(tgrpc)

set(COMPILE_OPTIONS -Wall -Wno-unused-variable -gdwarf-3)
set(COMPILE_OPTIONS "--std=gnu++2b;${COMPILE_OPTIONS}")
set(LINK_OPTIONS "$<IF:$<CONFIG:Debug>,-g,-s>")
set(LINK_OPTIONS "${LINK_OPTIONS};-static-libstdc++;-static-libgcc")

add_subdirectory(libs)
add_subdirectory(app)