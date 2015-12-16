###############################################################################
cmake_minimum_required(VERSION 2.6)
set(CMAKE_ALLOW_LOOSE_LOOP_CONSTRUCTS ON)

project(shared)

###############################################################################
set(shared_SRC
	Typedef.h
	Thread.h
	Thread.cc
	
	util/CircleBuffer.h
	util/CircleBuffer.cc
	util/ByteBuffer.h
	util/ByteBuffer.cc
	
	crypto/BigNumber.h
	crypto/BigNumber.cc
	crypto/Sha1.h
	crypto/Sha1.cc

	net/Socket.h
	net/Socket.cc
	net/SocketManager.h
	net/SocketManager.cc
	net/SocketWorker.h
	net/SocketWorker.cc
)

###############################################################################
add_library(shared STATIC ${shared_SRC})
