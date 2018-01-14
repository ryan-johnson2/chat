cmake_minimum_required(VERSION 2.8.2)
project(server)

set(src ${CMAKE_SOURCE_DIR}/src/)
set(server_SOURCES ${src}/server.c ${src}/listener.c ${src}/client_list.c ${src}/client_handler.c ${src}/message_queue.c ${src}/message_handler.c ${src}/server_data.c)
include_directories(${CMAKE_SOURCE_DIR}/include)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -pthread")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build)

add_executable(server ${server_SOURCES})
