# -*- cmake -*-

# - Find Google test
# VARS:
#	GTEST_INCLUDE_DIR, where to find gtest.h.
#	GTEST_LIBRARIES, the libraries needed to use Ice.
#	GTEST_FOUND, If false, threat as error.


find_path(GTEST_INCLUDE_DIR gtest/gtest.h NO_DEFAULT_PATH PATHS
	/usr/local/include
	/opt/local/include
	/usr/include
	/opt/include
	$ENV{GTEST_ROOT_DIR}/include
	)
	
find_library(GTEST_LIBRARIES NAMES gtest NO_DEFAULT_PATH PATHS
	/usr/local/lib
	/opt/local/lib
	/usr/lib
	/opt/lib
	$ENV{GTEST_ROOT_DIR}/lib
	)

if (GTEST_LIBRARIES AND GTEST_INCLUDE_DIR)
	SET(GTEST_FOUND "YES")
else()
	SET(GTEST_FOUND "NO")
endif()

if (GTEST_FOUND)
	message(STATUS "Found Gtest: ${GTEST_LIBRARIES}")
else()
	message(FATAL_ERROR "Couldn't find Gtest library. consider to fill GTEST_ROOT_DIR")
endif()






