cmake_minimum_required(VERSION 3.2)
project(Game)

set(CMAKE_CXX_STANDARD 14)

if(DEBUG)
    add_subdirectory(googletest)
    include_directories(googletest/googletest/include)
    include_directories(googletest/googlemock/include)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -g -DDEBUG")
    include_directories(test)
    set(SOURCE_DEBUG_FILES)
endif()

set(SOURCE_FILES src/main.cpp)

add_executable(Game ${SOURCE_FILES} ${SOURCE_DEBUG_FILES})
if(DEBUG)
    target_link_libraries(Game gtest gtest_main)
endif()