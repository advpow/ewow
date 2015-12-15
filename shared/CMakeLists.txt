###############################################################################
cmake_minimum_required(VERSION 2.6)
set(CMAKE_ALLOW_LOOSE_LOOP_CONSTRUCTS ON)

project(shared)

###############################################################################
set(shared_SRC
	Typedef.h

	net/Socket.h
	net/Socket.cc
	net/SocketManager.h
	net/SocketManager.cc
	net/SocketWorker.h
	net/SocketWorker.cc
)

###############################################################################
add_library(shared STATIC ${shared_SRC})
