# -*- cmake -*-

# - Find ZeroC Ice
# VARS:
#	ICE_INCLUDE_DIR, where to find Ice.h.
#	ICE_LIBRARIES, the libraries needed to use Ice.
#	ICE_FOUND, If false, threat as error.


find_path(ICE_INCLUDE_DIR Ice/Ice.h NO_DEFAULT_PATH PATHS
	/usr/local/include
	/opt/local/include
	/usr/include
	/opt/include
	/opt/Ice-3.4/include
	$ENV{ICE_ROOT_DIR}/include
	)
	
find_library(ICE_LIBRARY_ICE NAMES Ice.34 Ice NO_DEFAULT_PATH PATHS
	/usr/local/lib
	/opt/local/lib
	/usr/lib
	/opt/lib
	$ENV{ICE_ROOT_DIR}/lib
	)

find_library(ICE_LIBRARY_ICEUTIL NAMES IceUtil NO_DEFAULT_PATH PATHS
	/usr/local/lib
	/opt/local/lib
	/usr/lib
	/opt/lib
	$ENV{ICE_ROOT_DIR}/lib
	)

if (ICE_LIBRARY_ICE AND ICE_LIBRARY_ICEUTIL)
 set(ICE_LIBRARIES ${ICE_LIBRARY_ICE} ${ICE_LIBRARY_ICEUTIL})
endif()

if (ICE_LIBRARIES AND ICE_INCLUDE_DIR)
	SET(ICE_FOUND "YES")
else()
	SET(ICE_FOUND "NO")
endif()

if (ICE_FOUND)
	message(STATUS "Found Ice: ${ICE_LIBRARIES}")
else()
	message(FATAL_ERROR "Couldn't find Ice library. consider to fill ICE_ROOT_DIR")
endif()






