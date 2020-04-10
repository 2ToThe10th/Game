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

set(SOURCE_FILES src/main.cpp src/Client/ClientAgent/ClientMap.h src/Client/ClientAgent/ClientMap.cpp src/Player.cpp src/Player.h src/Location.cpp src/Location.h src/Client/ClientAgent/ClientAgent.cpp src/Client/ClientAgent/ClientAgent.h src/Client/GraphicAgent/GraphicAgent.cpp src/Client/GraphicAgent/GraphicAgent.h src/Angle.h src/Client/GraphicAgent/GraphicMap.cpp src/Client/GraphicAgent/GraphicMap.h src/Client/InputAgent/InputAgent.cpp src/Client/InputAgent/InputAgent.h src/Server/ServerAgent/ServerMap.h src/Server/ServerAgent/ServerMap.cpp src/Marshaling.cpp src/Marshaling.h src/Client/TCPSocketAgent/TCPSocketAgent.cpp src/Client/TCPSocketAgent/TCPSocketAgent.h src/Client/UDPSocketAgent/UDPSocketAgent.cpp src/Client/UDPSocketAgent/UDPSocketAgent.h src/UserAction.cpp src/UserAction.h src/TCPSocketHelper.cpp src/TCPSocketHelper.h src/Server/UDPSocketAgent/UDPSocketAgent.cpp src/Server/UDPSocketAgent/UDPSocketAgent.h src/Server/TCPSocketAgent/TCPSocketAgent.cpp src/Server/TCPSocketAgent/TCPSocketAgent.h src/Server/ServerAgent/ServerAgent.cpp src/Server/ServerAgent/ServerAgent.h src/Server/PhysicalAgent/PhysicalAgent.cpp src/Server/PhysicalAgent/PhysicalAgent.h src/UDPSocketHelper.cpp src/UDPSocketHelper.h src/EpollOneReturn.cpp src/EpollOneReturn.h src/Server/ServerAgent/UDPToPhysicsQueue.cpp src/Server/ServerAgent/UDPToPhysicsQueue.h src/Duration.h src/Duration.cpp src/Server/ServerAgent/PhysicsToMapQueue.cpp src/Server/ServerAgent/PhysicsToMapQueue.h src/Server/ServerAgent/UserUpdate.cpp src/Server/ServerAgent/UserUpdate.h src/Server/ServerAgent/PlayerState.cpp src/Server/ServerAgent/PlayerState.h)

add_executable(Game ${SOURCE_FILES} ${SOURCE_DEBUG_FILES})
target_link_libraries(Game sfml-graphics sfml-window sfml-system sfml-audio sfml-network)
if (DEBUG)
    target_link_libraries(Game gtest gtest_main)
endif ()