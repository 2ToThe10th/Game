cmake_minimum_required(VERSION 3.2)
project(Game)

set(CMAKE_CXX_STANDARD 17)

if(DEBUG)
    add_subdirectory(googletest)
    include_directories(googletest/googletest/include)
    include_directories(googletest/googlemock/include)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -DDEBUG")
    include_directories(test)
    set(SOURCE_DEBUG_FILES)
endif()

if(NDEBUG)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DNDEBUG")
endif()

if(SERVER)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DSERVER")
endif()

if(CLIENT)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DCLIENT")
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

set(SOURCE_FILES src/main.cpp src/Map.h src/Map.cpp src/Player.cpp src/Player.h src/Location.cpp src/Location.h src/GameClient.cpp src/GameClient.h src/GameGraphic.cpp src/GameGraphic.h src/Angel.h src/ClientMap.h)

add_executable(Game ${SOURCE_FILES} ${SOURCE_DEBUG_FILES})
target_link_libraries(Game sfml-graphics sfml-window sfml-system sfml-audio sfml-network)
if(DEBUG)
    target_link_libraries(Game gtest gtest_main)
endif()