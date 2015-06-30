cmake_minimum_required(VERSION 3.1)
project(demo)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -g")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ../bin)

# add_definitions(-DNDEBUG)

include_directories(../libs/include)
# include_directories(../libframework)
# include_directories(..)

link_directories(../libs/lib)

aux_source_directory(. SOURCE_FILES)

add_executable(crawler crawler.cc)
target_link_libraries(crawler framework event pthread)

add_executable(demo main.cc)
target_link_libraries(demo event pthread)

# target_link_libraries (gw protobuf framework event_core jansson zlog pthread)


