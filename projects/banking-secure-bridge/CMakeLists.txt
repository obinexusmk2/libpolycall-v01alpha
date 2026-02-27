cmake_minimum_required(VERSION 3.20)
project(banking_secure_bridge C)

find_package(libpolycall CONFIG REQUIRED)
find_package(OpenSSL REQUIRED)

add_executable(banking_bridge
    src/banking_bridge.c
    src/zero_trust_auth.c
    src/telemetry_logger.c
)

target_include_directories(banking_bridge PRIVATE src)
target_link_libraries(banking_bridge PRIVATE libpolycall::libpolycall OpenSSL::SSL OpenSSL::Crypto)
