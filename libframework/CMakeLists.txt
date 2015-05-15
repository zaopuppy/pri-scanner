cmake_minimum_required(VERSION 3.1)
project(framework)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ../libs/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ../libs/lib)
# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include_directories(../libs/include)

aux_source_directory(. SOURCE_FILES)

add_library(framework ${SOURCE_FILES})

