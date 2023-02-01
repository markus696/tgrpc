cmake_minimum_required(VERSION 3.19.6)
project(tdlib_example)

file(GLOB_RECURSE SRC_LIST LIST_DIRECTORIES false *.cpp)

add_executable(${PROJECT_NAME} ${SRC_LIST})


target_link_libraries(${PROJECT_NAME} PRIVATE
        Td::TdStatic)