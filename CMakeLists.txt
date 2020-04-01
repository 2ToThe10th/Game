cmake_minimum_required(VERSION 3.2)
project(Game)

set(CMAKE_CXX_STANDARD 17)

if (DEBUG)
    add_subdirectory(googletest)
    include_directories(googletest/googletest/include)
    include_directories(googletest/googlemock/include)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -DDEBUG")
    include_directories(test)
    set(SOURCE_DEBUG_FILES)
endif ()

if (NDEBUG)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DNDEBUG")
endif ()

if (SERVER)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DSERVER")
endif ()

if (CLIENT)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DCLIENT")
endif ()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

set(SOURCE_FILES src/main.cpp src/Client/ClientAgent/Map.h src/Client/ClientAgent/Map.cpp src/Player.cpp src/Player.h src/Location.cpp src/Location.h src/Client/ClientAgent/ClientAgent.cpp src/Client/ClientAgent/ClientAgent.h src/Client/GraphicAgent/GraphicAgent.cpp src/Client/GraphicAgent/GraphicAgent.h src/Angle.h src/Client/GraphicAgent/GraphicMap.cpp src/Client/GraphicAgent/GraphicMap.h src/Client/InputAgent/InputAgent.cpp src/Client/InputAgent/InputAgent.h src/Marshaling.cpp src/Marshaling.h src/Client/TCPSocketAgent/TCPSocketAgent.cpp src/Client/TCPSocketAgent/TCPSocketAgent.h src/Client/UDPSocketAgent/UDPSocketAgent.cpp src/Client/UDPSocketAgent/UDPSocketAgent.h)

add_executable(Game ${SOURCE_FILES} ${SOURCE_DEBUG_FILES})
target_link_libraries(Game sfml-graphics sfml-window sfml-system sfml-audio sfml-network)
if (DEBUG)
    target_link_libraries(Game gtest gtest_main)
endif ()