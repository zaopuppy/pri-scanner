cmake_minimum_required(VERSION 3.1)
project(pri_scanner)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

add_subdirectory(libframework)
add_subdirectory(gw)

add_dependencies(gw framework)


