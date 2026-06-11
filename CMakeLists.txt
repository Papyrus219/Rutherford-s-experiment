cmake_minimum_required(VERSION 3.10)

project(animation)

add_executable(animation main.cpp)

install(TARGETS animation RUNTIME DESTINATION bin)
