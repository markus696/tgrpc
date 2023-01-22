cmake_minimum_required(VERSION 3.19.6)
project(tgrpc_model)

file(GLOB_RECURSE PROTO_FILES LIST_DIRECTORIES false proto/*.proto)

foreach (PROTO_FILE ${PROTO_FILES})
    string(REGEX REPLACE "[.]proto$" ".pb.cc" CC_FILE ${PROTO_FILE})
    string(REGEX REPLACE "[.]proto$" ".pb.h" H_FILE ${PROTO_FILE})

    cmake_path(GET CC_FILE FILENAME CC_FILE_NAME)
    cmake_path(GET H_FILE FILENAME H_FILE_NAME)

    list(APPEND CC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/${CC_FILE_NAME})
    list(APPEND H_FILES  ${CMAKE_CURRENT_SOURCE_DIR}/${H_FILE_NAME})
endforeach ()


message(STATUS ${CC_FILES} ${H_FILES})

add_custom_command(
        OUTPUT ${CC_FILES} ${H_FILES}
        COMMAND ${_PROTOBUF_PROTOC}
        ARGS --grpc_out "${CMAKE_CURRENT_SOURCE_DIR}"
        --cpp_out "${CMAKE_CURRENT_SOURCE_DIR}"
        -I "${CMAKE_CURRENT_SOURCE_DIR}/proto"
        --plugin=protoc-gen-grpc="${_GRPC_CPP_PLUGIN_EXECUTABLE}"
        ${PROTO_FILES}
        DEPENDS ${PROTO_FILES})

add_library(${PROJECT_NAME} STATIC ${CC_FILES} ${H_FILES})

target_include_directories(${PROJECT_NAME}
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(${PROJECT_NAME}
        PUBLIC libprotobuf grpc++)




