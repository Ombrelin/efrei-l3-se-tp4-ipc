cmake_minimum_required(VERSION 3.10)
project(l3_se_tp4_ipc C)

set(CMAKE_C_STANDARD 11)

add_executable(ex1 fileno.c)
add_executable(ex2 redirection_sdtout.c)
add_executable(ex3 copy.c)
add_executable(ex4 juste_prix.c)