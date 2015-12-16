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
	util/String.h
	util/String.cc
	
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
	
	database/SqlField.h
	database/SqlField.cc
	database/SqlResultSet.h
	database/SqlResultSet.cc
	database/SqlConnection.h
	database/SqlConnection.cc
	database/SqlDatabase.h
	database/SqlDatabase.inl
	database/MysqlConnection.h
	database/MysqlConnection.cc
	database/MysqlResultSet.h
	database/MysqlResultSet.cc
)

###############################################################################
add_library(shared STATIC ${shared_SRC})
