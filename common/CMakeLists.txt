###############################################################################
cmake_minimum_required(VERSION 2.6)
set(CMAKE_ALLOW_LOOSE_LOOP_CONSTRUCTS ON)

project(common)

###############################################################################
set(common_SRC
	Common.h
	Common.cc
)

###############################################################################
add_library(common STATIC ${common_SRC})
