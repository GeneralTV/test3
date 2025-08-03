cmake_minimum_required(VERSION 3.10)
project(RadmirMod)

set(CMAKE_CXX_STANDARD 17)
add_library(RadmirMod SHARED main.cpp)
target_link_libraries(RadmirMod wininet)
