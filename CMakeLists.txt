cmake_minimum_required(VERSION 3.22.1)

project(genN)

set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_CXX_STANDARD 11)

set(CMAKE_CXX_FLAGS "$ENV{CXXFLAGS} -rdynamic -O3 -g -std=c++11 -Werror -Wno-unused-function")

set(LIB_SRC
    src/log.cpp
)
add_library(gen SHARED ${LIB_SRC})


add_executable(test tests/test.cpp)
target_link_libraries(test gen)
#add_dependencies(test gen)
include_directories(.)

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)