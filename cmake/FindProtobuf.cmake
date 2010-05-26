# -*- cmake -*-

# - Find Google Protocol buffer
# VARS:
#	PROTOBUF_INCLUDE_DIR, where to find generated_message_util.h.
#	PROTOBUF_LIBRARIES, the libraries needed to use Ice.
#	PROTOBUF_FOUND, If false, threat as error.


find_path(PROTOBUF_INCLUDE_DIR google/protobuf/generated_message_util.h NO_DEFAULT_PATH PATHS
	/usr/loca/include
	/opt/local/include
	/usr/include
	/opt/include
	/opt/Ice-3.4/include
	$ENV{PROTOBUF_ROOT_PATH}/include
	)
	
	
find_library(PROTOBUF_LIBRARIES NAMES protobuf NO_DEFAULT_PATH PATHS
	/usr/local/lib
	/opt/local/lib
	/usr/lib
	/opt/lib
	$ENV{PROTOBUF_ROOT_PATH}/lib
	)

if (PROTOBUF_LIBRARIES AND PROTOBUF_INCLUDE_DIR)
	SET(PROTOBUF_FOUND "YES")
else()
	SET(PROTOBUF_FOUND "NO")
endif()

if (PROTOBUF_FOUND)
	message(STATUS "Found Protobuf: ${PROTOBUF_LIBRARIES}")
else()
	message(FATAL_ERROR "Couldn't find Protobuf library. try using exporting PROTOBUF_ROOT_PATH")
endif()






