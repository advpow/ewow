###############################################################################
cmake_minimum_required(VERSION 2.6)
set(CMAKE_ALLOW_LOOSE_LOOP_CONSTRUCTS ON)

project(ewow)

###############################################################################
option(64BIT "64BIT" FALSE)
if (64BIT)
	add_definitions(-D__64BIT__)
endif()

option(WINDOWS "WINDOWS" FALSE)
if (WINDOWS)
	add_definitions(-D__WINDOWS__)
	add_definitions(-D_WIN32_WINNT=0x0501)
	add_definitions(-W3)
else ()
	add_definitions(-D__LINUX__)
	add_definitions(-Wall)
	if (CMAKE_COMPILER_IS_GNUCXX)
		add_definitions(-std=gnu++0x)
	else ()
		add_definitions(-std=stdc++11)
	endif()
endif()

option(DEBUG "DEBUG" FALSE)
if (DEBUG)
	add_definitions(-D__DEBUG__)
endif()

## find library ###############################################################
set(Boost_USE_STATIC_LIBS ON)
find_package(Boost REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})
link_directories(${Boost_LIBRARY_DIRS})

if(WINDOWS)
	set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake/" ${CMAKE_MODULE_PATH})
	find_package(libevent)
	if(libevent_FOUND)
		include_directories(${libevent_INCLUDE_DIR})
		link_directories(${libevent_LIBRARY_DIR})
	else()
		message(FATAL_ERROR "libevent not found!")
	endif()
endif()

###############################################################################
include_directories(
	${PROJECT_SOURCE_DIR}/shared
)

###############################################################################
add_subdirectory(shared)
add_subdirectory(realmd)