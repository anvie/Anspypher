# -*- cmake -*-

# - Find Hypertable
# VARS:
#	HYPERTABLE_INCLUDE_DIR, where to find KeySpec.h.
#	HYPERTABLE_COMMON_INCLUDE_DIR.
#	HYPERTABLE_LIBRARIES, the libraries needed to use Ice.
#	HYPERTABLE_FOUND, If false, threat as error.


find_path(HYPERTABLE_INCLUDE_DIR Hypertable/Lib/KeySpec.h NO_DEFAULT_PATH PATHS
  /opt/hypertable/current/include
	$ENV{HYPERTABLE_ROOT_DIR}/include
	)

find_path(HYPERTABLE_COMMON_INCLUDE_DIR Common/Compat.h NO_DEFAULT_PATH PATHS
	/opt/hypertable/current/include
	${HYPERTABLE_INCLUDE_DIR}
	$ENV{HYPERTABLE_ROOT_DIR}/include
	)
	
find_path(HYPERTABLE_LIB_DIR libHypertable.a NO_DEFAULT_PATH PATHS
	/opt/hypertable/current/lib
	$ENV{HYPERTABLE_ROOT_DIR}/lib
	)
	
#message(STATUS ${HYPERTABLE_COMMON_INCLUDE_DIR})

if(DEFINED $ENV{HYPERTABLE_ROOT_DIR})
  find_library(HYPERTABLE_LIBRARIES NAME Hypertable NO_DEFAULT_PATH PATHS
    $ENV{HYPERTABLE_ROOT_DIR}
    )
else()
  find_library(HYPERTABLE_LIBRARIES NAMES Hypertable NO_DEFAULT_PATH PATHS
    /opt/hypertable/current/lib
    $ENV{HYPERTABLE_ROOT_DIR}/lib
    )
endif()

set(HYPERTABLE_LIBRARIES Hypertable HyperDfsBroker Hypertable HyperComm HyperCommon Hyperspace HyperTools)

#set(HYPERTABLE_LIBRARIES
#  ${HYPERTABLE_LIB_DIR}/libHypertable.a
#  ${HYPERTABLE_LIB_DIR}/libHyperDfsBroker.a
#  ${HYPERTABLE_LIB_DIR}/libHypertable.a
#  ${HYPERTABLE_LIB_DIR}/libHyperComm.a
#  ${HYPERTABLE_LIB_DIR}/libHyperCommon.a
#  ${HYPERTABLE_LIB_DIR}/libHyperspace.a
#  ${HYPERTABLE_LIB_DIR}/libHyperTools.a
#)

if (HYPERTABLE_LIBRARIES AND HYPERTABLE_INCLUDE_DIR AND HYPERTABLE_COMMON_INCLUDE_DIR AND HYPERTABLE_LIB_DIR)
	SET(HYPERTABLE_FOUND "YES")
else()
	SET(HYPERTABLE_FOUND "NO")
endif()

if (HYPERTABLE_FOUND)
  message(STATUS "Found Hypertable inc dir: ${HYPERTABLE_INCLUDE_DIR}")
  message(STATUS "Found Hypertable: ${HYPERTABLE_LIBRARIES}")
else()
  message(FATAL_ERROR "Couldn't find Hypertable library, set root path to HYPERTABLE_ROOT_DIR")
endif()






