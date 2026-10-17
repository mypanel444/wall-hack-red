cmake_minimum_required(VERSION 3.18)
project(longrange)
set(CMAKE_CXX_STANDARD 17)

add_library(longrange SHARED main.cpp)

target_include_directories(longrange PRIVATE Dobby/include)

target_link_libraries(longrange
    log
    android
    dl
    ${CMAKE_SOURCE_DIR}/Dobby/build/libdobby.a
)
