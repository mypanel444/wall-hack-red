cmake_minimum_required(VERSION 3.18)
project(longrange)

set(CMAKE_CXX_STANDARD 17)

# Include Dobby
include_directories(Dobby/include)

add_library(longrange SHARED main.cpp)

# Link Dobby at Android libs
target_link_libraries(longrange
    log
    android
    dl
    ${CMAKE_SOURCE_DIR}/Dobby/build/libdobby.a
)
