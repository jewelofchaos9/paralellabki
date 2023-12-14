cmake_minimum_required(VERSION 3.12)
project(lab7)

find_package(MPI REQUIRED)
find_package(OpenMP REQUIRED)

add_executable(lab7.out lab.c)

include_directories(SYSTEM ${MPI_INCLUDE_PATH})

target_link_libraries(lab7.out PRIVATE -lm ${MPI_C_LIBRARIES} OpenMP::OpenMP_C)