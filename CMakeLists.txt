cmake_minimum_required(VERSION 3.10)
project(tcp-server C)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_C_STANDARD 11)

file(GLOB SRC "src/*.c")
add_executable(${PROJECT_NAME} ${SRC})
target_include_directories(${PROJECT_NAME} PRIVATE "include/")
