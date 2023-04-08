cmake_minimum_required(VERSION 3.24)
project(data_lab_stress)

set(CMAKE_CXX_STANDARD 14)

add_executable(data_lab_stress data_lab.hpp data_lab.cpp main.cpp)
