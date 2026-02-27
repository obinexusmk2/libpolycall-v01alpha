cmake_minimum_required(VERSION 3.20)
project(telemetry_dashboard C)

find_package(libpolycall CONFIG REQUIRED)

add_executable(telemetry_server
    src/telemetry_server.c
    src/cir_object.c
    src/huffman_avl.c
    src/state_machine.c
)

target_include_directories(telemetry_server PRIVATE src)
target_link_libraries(telemetry_server PRIVATE libpolycall::libpolycall)
