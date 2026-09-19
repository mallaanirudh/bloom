cmake_minimum_required(VERSION 3.16)

project(bloom C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

find_package(microxrcedds_client REQUIRED)

add_library(bloom
       src/protocol/protocol.c
       src/protocol/xrce_protocol.c
       src/transport/mock_transport.c
)

target_include_directories(bloom
    PUBLIC
        ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(bloom
    PUBLIC
        microxrcedds_client
)

add_executable(bloom_test
    tests/test_basic.c
)

target_link_libraries(bloom_test
    PRIVATE bloom
)