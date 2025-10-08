cmake_minimum_required( VERSION 3.31 )
project(ABOBA)
set( CMAKE_CXX_STANDARD 23 )
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

add_compile_options(-Wall -Wextra -Wpedantic)

add_executable( SumFinder SumFinder.cxx )
add_executable( IsNumFibonacci IsNumFibonacci.cxx )