cmake_minimum_required(VERSION 3.8)
project(unp_prac)
set(CMAKE_C_STANDARD 11)

set(SIMPLE_SERVER simple/simple_server.c unp_prac.h lib_io/writen.c lib/error.c)
add_executable(simple_server ${SIMPLE_SERVER})

set(SIMPLE_CLIENT simple/simple_client.c unp_prac.h lib_io/readn.c lib/error.c)
add_executable(simple_client ${SIMPLE_CLIENT})

set(ECHO_SERVER echo/echo_server.c unp_prac.h lib_io/readn.c lib_io/writen.c lib/error.c)
add_executable(echo_server ${ECHO_SERVER})

set(ECHO_CLIENT echo/echo_client.c unp_prac.h lib_io/readn.c lib_io/writen.c lib_io/readline.c lib/error.c)
add_executable(echo_client ${ECHO_CLIENT})

set(SELECT_ECHO_SERVER select/select_echo_server.h unp_prac.h select/select_echo_server.cpp lib_io/readn.c lib_io/writen.c lib_io/readline.c lib/error.c)
add_executable(select_echo_server ${SELECT_ECHO_SERVER})