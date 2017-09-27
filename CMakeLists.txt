cmake_minimum_required(VERSION 3.8)
project(unp_prac)

set(CMAKE_C_STANDARD 11)

set(SIMPLE_SERVER simple/server.c unp_prac.h lib)
add_executable(simple_server ${SIMPLE_SERVER})

set(SIMPLE_CLIENT simple/client.c unp_prac.h lib)
add_executable(simple_client ${SIMPLE_CLIENT})