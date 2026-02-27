cmake_minimum_required(VERSION 3.20)
project(edge_iot_mesh C)

find_package(libpolycall CONFIG REQUIRED)

add_executable(edge_node
    src/edge_node.c
    src/trigger_update.c
    src/mesh_routing.c
    src/conflict_resolution.c
)

target_include_directories(edge_node PRIVATE src)
target_link_libraries(edge_node PRIVATE libpolycall::libpolycall)
