cmake_minimum_required(VERSION 2.6)
set(CMAKE_ALLOW_LOOSE_LOOP_CONSTRUCTS ON)

project(realmd)

###############################################################################

set(realmd_SOURCES
	Main.cc
	
	net/RealmdSocket.h
	net/RealmdSocket.cc
	net/RealmdSocketFactory.h
	net/RealmdSocketFactory.cc
)

###############################################################################

include_directories( 
	${PROJECT_SOURCE_DIR}
)

link_directories(
)

###############################################################################

add_executable(realmd ${realmd_SOURCES})
target_link_libraries(realmd shared)
if(WINDOWS)
target_link_libraries(realmd ws2_32 libevent libevent_core libevent_extras)
else()
endif()
