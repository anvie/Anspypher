PROJECT(anspypher)
CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
#SET(CMAKE_VERBOSE_MAKEFILE TRUE)

SET(Boost_USE_STATIC_LIBS   ON)
SET(Boost_USE_MULTITHREADED ON)
FIND_PACKAGE (Boost COMPONENTS system filesystem thread program_options iostreams)

#MESSAGE(STATUS "boost inc path: ${Boost_INCLUDE_DIR}")
#MESSAGE(STATUS "boost lib path: ${Boost_LIBRARIES}")


IF(MSVC)
foreach(flag_var
        CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE
        CMAKE_CXX_FLAGS_MINSIZEREL CMAKE_CXX_FLAGS_RELWITHDEBINFO
        CMAKE_C_FLAGS CMAKE_C_FLAGS_DEBUG CMAKE_C_FLAGS_RELEASE
        CMAKE_C_FLAGS_MINIZIREL CMAKE_C_FLAGS_RELWITHDEBINFO)
   if(${flag_var} MATCHES "/MD")
      string(REGEX REPLACE "/MD" "/MT" ${flag_var} "${${flag_var}}")
   endif(${flag_var} MATCHES "/MD")
endforeach(flag_var)
ENDIF(MSVC)

#MESSAGE(STATUS "CMAKE_C_FLAGS_DEBUG: ${CMAKE_C_FLAGS_DEBUG}")
#MESSAGE(STATUS "CMAKE_CXX_FLAGS_DEBUG: ${CMAKE_CXX_FLAGS_DEBUG}")

add_definitions(-D_LARGEFILE_SOURCE -D_FILE_OFFSET_BITS=64 -D_REENTRANT)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
add_definitions(-DDEBUG -gdb)
endif()

FILE(GLOB CPPS src/*.c src/*.cpp src/*.h gen/*.cpp gen/*.h gen/*.cc)

SET(SOURCES main.cpp ${CPPS})

message(STATUS ${SOURCES})

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

find_package(Ice REQUIRED)
find_package(LibEvent REQUIRED)
find_package(Log4cpp REQUIRED)
find_package(Protobuf REQUIRED)
find_package(SIGAR REQUIRED)
find_package(Readline REQUIRED)
find_package(Hypertable REQUIRED)
find_package(Tcmalloc)

INCLUDE_DIRECTORIES(
	./
	${Boost_INCLUDE_DIR}
	/opt/local/include
	/usr/local/include
	/usr/local/BerkeleyDB.4.8/include
	/opt/hypertable/current/include
	${ICE_INCLUDE_DIR}
	${LibEvent_INCLUDE_DIR}
	${Log4cpp_INCLUDE_DIR}
	${PROTOBUF_INCLUDE_DIR}
	${HYPERTABLE_INCLUDE_DIR}
	${HYPERTABLE_COMMON_INCLUDE_DIR}
	${SIGAR_INCLUDE_DIR}
	${READLINE_INCLUDE_DIR}
	)
	

SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY bin)
	
IF(WIN32)
	SET_TARGET_PROPERTIES( ansavlib
		PROPERTIES
		COMPILE_DEFINITIONS _CRT_SECURE_NO_WARNINGS
		)
ENDIF(WIN32)


ADD_EXECUTABLE(anspypher ${SOURCES})

TARGET_LINK_LIBRARIES(anspypher
	${Boost_LIBRARIES}
	${ICE_LIBRARIES}
	${LibEvent_LIBS}
	${Log4cpp_LIBRARIES}
	${PROTOBUF_LIBRARIES}
	${HYPERTABLE_LIBRARIES}
	${SIGAR_LIBRARIES}
	${READLINE_LIBRARIES}
	z
	ncurses
	pthread
	expat
)

ADD_CUSTOM_TARGET(deploy
	cp build/Debug/anspypher /opt/hypertable/current/bin/anspypher
	)


