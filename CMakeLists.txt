cmake_minimum_required(VERSION 3.19.6)
project(tgrpc)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY      "${CMAKE_CURRENT_SOURCE_DIR}/bin")

add_subdirectory(libs)
add_subdirectory(rpc_model)
add_subdirectory(app)