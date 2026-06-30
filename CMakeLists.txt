cmake_minimum_required(VERSION 3.16)

project(mod_ble_host_demo C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

add_compile_options(-Wall -Wextra -Wpedantic)

set(PROJECT_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)

set(MOD_BLE_HEADERS
    ${PROJECT_INCLUDE_DIR}/mod_ble_types.h
    ${PROJECT_INCLUDE_DIR}/mod_ble_log.h
    ${PROJECT_INCLUDE_DIR}/ble_client.h
    ${PROJECT_INCLUDE_DIR}/proto_codec.h
    ${PROJECT_INCLUDE_DIR}/proto_session.h
    ${PROJECT_INCLUDE_DIR}/mod_ble.h
)

set(MOD_BLE_SOURCES
    ${PROJECT_SOURCE_DIR}/src/mod_ble_log.c
    ${PROJECT_SOURCE_DIR}/src/ble_client.c
    ${PROJECT_SOURCE_DIR}/src/proto_codec.c
    ${PROJECT_SOURCE_DIR}/src/proto_session.c
    ${PROJECT_SOURCE_DIR}/src/mod_ble.c
)

add_library(mod_ble STATIC ${MOD_BLE_HEADERS} ${MOD_BLE_SOURCES})
target_include_directories(mod_ble PUBLIC ${PROJECT_INCLUDE_DIR})

add_executable(mod_ble_demo ${PROJECT_SOURCE_DIR}/src/main.c)
target_link_libraries(mod_ble_demo PRIVATE mod_ble)
