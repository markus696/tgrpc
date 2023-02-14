cmake_minimum_required(VERSION 3.19.6)
project(telegram_module)

file(GLOB_RECURSE SRC_LIST LIST_DIRECTORIES false *.cpp)
file(GLOB_RECURSE HEADER_LIST LIST_DIRECTORIES false *.h)

add_library(${PROJECT_NAME} STATIC ${SRC_LIST} ${HEADER_LIST})

target_include_directories(${PROJECT_NAME} PUBLIC
        include)

target_link_libraries(${PROJECT_NAME} PUBLIC
        Td::TdStatic quill)