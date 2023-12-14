cmake_minimum_required(VERSION 3.12)
project(lab7_sequential)

add_executable(lab7.out lab.c)

target_link_libraries(lab7.out PRIVATE -lm)