cmake_minimum_required(VERSION 3.18)
project(longrange)
set(CMAKE_CXX_STANDARD 17)

# Idinagdag natin ito para si CMake na ang bahala mag-build kay Dobby
add_subdirectory(Dobby)

add_library(longrange SHARED main.cpp)

target_link_libraries(longrange
    log
    android
    dl
    dobby
)
