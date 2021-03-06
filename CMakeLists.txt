PROJECT(anspypher)
CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

message(STATUS "System: ${CMAKE_SYSTEM}")

#SET(CMAKE_OSX_ARCHITECTURES ppc64 x86_64)
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

add_definitions(-D_LARGEFILE_SOURCE -D_FILE_OFFSET_BITS=64 -D_REENTRANT -Wall -Wno-long-long -Wno-variadic-macros)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-deprecated")

FILE(GLOB CPPS src/*.c src/*.cpp src/*.h gen/*.cpp gen/*.h gen/*.cc)

SET(SOURCES ${CPPS})

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

find_package(Ice REQUIRED)
find_package(LibEvent REQUIRED)
find_package(Log4cpp REQUIRED)
find_package(Protobuf REQUIRED)
find_package(SIGAR REQUIRED)
find_package(Readline REQUIRED)
find_package(Hypertable REQUIRED)
find_package(Tcmalloc)
find_package(Gtest)
find_package(BerkeleyDB REQUIRED)

# show info
message(STATUS "Hypertable conf file: ${HYPERTABLE_CONF}")

if(CMAKE_SYSTEM_NAME MATCHES "Darwin")
  # just strip it tcmalloc won't work in OSX SLP
  set(Tcmalloc_LIBRARIES "")
endif()

INCLUDE_DIRECTORIES(
	./
	/usr/include
	/opt/include
	${Boost_INCLUDE_DIR}
	/opt/local/include
	/usr/local/include
	/usr/local/BerkeleyDB.4.8/include
	${ICE_INCLUDE_DIR}
	${LibEvent_INCLUDE_DIR}
	${Log4cpp_INCLUDE_DIR}
	${PROTOBUF_INCLUDE_DIR}
	${HYPERTABLE_INCLUDE_DIR}
	${HYPERTABLE_COMMON_INCLUDE_DIR}
	${SIGAR_INCLUDE_DIR}
	${READLINE_INCLUDE_DIR}
	${Tcmalloc_INCLUDE_DIR}
	)

include(cmake/configgen.cmake)
	
SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY lib)
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY bin)
	
ADD_LIBRARY(anspypher ${SOURCES})

ADD_EXECUTABLE(anspypherd main.cpp)
ADD_EXECUTABLE(test test.cpp)
ADD_EXECUTABLE(test_raw tests/test_raw.cpp)


SET_TARGET_PROPERTIES(anspypherd
	PROPERTIES
	COMPILE_FLAGS	-Wno-deprecated
	LINK_FLAGS  "-L${HYPERTABLE_LIB_DIR} -lHypertable -lHyperDfsBroker -lHypertable -lHyperComm -lHyperCommon -lHyperspace -lHyperTools"
	)

SET_TARGET_PROPERTIES(test 
	PROPERTIES 
	COMPILE_FLAGS "-I${GTEST_INCLUDE_DIR}"
	LINK_FLAGS  "-L${HYPERTABLE_LIB_DIR} -lHypertable -lHyperDfsBroker -lHypertable -lHyperComm -lHyperCommon -lHyperspace -lHyperTools"
	)

SET_TARGET_PROPERTIES(test_raw 
	PROPERTIES 
	COMPILE_FLAGS "-I${GTEST_INCLUDE_DIR}"
	LINK_FLAGS  "-L${HYPERTABLE_LIB_DIR} -lHypertable -lHyperDfsBroker -lHypertable -lHyperComm -lHyperCommon -lHyperspace -lHyperTools"
	)

TARGET_LINK_LIBRARIES(anspypherd
	anspypher
	${Boost_LIBRARIES}
	${ICE_LIBRARIES}
	${LibEvent_LIBS}
	${Log4cpp_LIBRARIES}
	${SIGAR_LIBRARIES}
	${READLINE_LIBRARIES}
	${Tcmalloc_LIBRARIES}
	z
	pthread
	expat
	${PROTOBUF_LIBRARIES}
	Hypertable
)

if(CMAKE_SYSTEM_NAME MATCHES "Linux")
 set(ADD_LIBS Hypertable)
endif()

TARGET_LINK_LIBRARIES(test 
  anspypher
  ${Tcmalloc_LIBRARIES}
  Hypertable HyperDfsBroker HyperComm HyperCommon Hyperspace HyperTools
  ${Boost_LIBRARIES}
  ${ICE_LIBRARIES}
  ${LibEvent_LIBS}
  ${Log4cpp_LIBRARIES}
  ${SIGAR_LIBRARIES}
  ${READLINE_LIBRARIES}
  z
  pthread
  expat
  ${PROTOBUF_LIBRARIES}
  ${GTEST_LIBRARIES}
  ${ADD_LIBS}
)

TARGET_LINK_LIBRARIES(test_raw
  anspypher
  ${Tcmalloc_LIBRARIES}
  Hypertable HyperDfsBroker HyperComm HyperCommon Hyperspace HyperTools
  ${Boost_LIBRARIES}
  ${ICE_LIBRARIES}
  ${LibEvent_LIBS}
  ${Log4cpp_LIBRARIES}
  ${SIGAR_LIBRARIES}
  ${READLINE_LIBRARIES}
  z
  pthread
  expat
  ${PROTOBUF_LIBRARIES}
  ${GTEST_LIBRARIES}
  ${ADD_LIBS}
)

ADD_CUSTOM_TARGET(generate_doc doxygen anspypher.cfg)





