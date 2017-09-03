cmake_minimum_required(VERSION 3.8)
project(unp_prac)

set(CMAKE_C_STANDARD 11)

set(SOURCE_FILES server.c unp_prac.h.h client.c lib/inet_pton.c lib/inet_pton.h)
add_executable(unp_prac ${SOURCE_FILES})