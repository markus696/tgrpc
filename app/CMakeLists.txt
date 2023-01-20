file(GLOB_RECURSE SRC_LIST LIST_DIRECTORIES true *.cpp)

add_executable(${PROJECT_NAME} ${SRC_LIST})

target_compile_options(${PROJECT_NAME} PUBLIC SHELL:${COMPILE_OPTIONS})
target_link_options(${PROJECT_NAME} PUBLIC SHELL:${LINK_OPTIONS})

target_link_libraries(${PROJECT_NAME} PRIVATE Td::TdStatic)