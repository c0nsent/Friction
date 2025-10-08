cmake_minimum_required(VERSION 4.1)

project( Theme1-2 )

# C++ standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

add_compile_options(-Wall -Wextra -Wpedantic)

add_executable( A A.cxx )
add_executable( B B.cxx )
add_executable( C C.cxx )
add_executable( D D.cxx )
add_executable( E E.cxx )
add_executable( F F.cxx )
add_executable( G G.cxx )
add_executable( I I.cxx )
add_executable( J J.cxx )