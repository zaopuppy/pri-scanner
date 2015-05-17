cmake_minimum_required(VERSION 3.1)
project(gateway)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ../bin)

include_directories(../libs/include)
include_directories(../libframework)
include_directories(..)

link_directories(../libs/lib)

aux_source_directory(. SOURCE_FILES)

add_executable(gw ${SOURCE_FILES})
target_link_libraries (gw protobuf framework event_core jansson zlog pthread)


