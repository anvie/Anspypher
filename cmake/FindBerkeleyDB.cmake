# -*- cmake -*-

# - Find BerkeleyDB
# Find the BerkeleyDB includes and library
# This module defines
#  BDB_INCLUDE_DIR, where to find db.h, etc.
#  BDB_LIBRARIES, the libraries needed to use BerkeleyDB.
#  BDB_FOUND, If false, do not try to use BerkeleyDB.
#  also defined, but not for general use are
#  BDB_LIBRARY, where to find the BerkeleyDB library.

find_path(BDB_INCLUDE_DIR db_cxx.h NO_DEFAULT_PATH PATHS
    ${HT_DEPENDENCY_INCLUDE_DIR}
    /usr/local/BerkeleyDB.4.8/include
    /usr/local/include/db48
    /opt/local/include/db48
    /usr/local/include
    /usr/include/db4
    /usr/include
    )

set(BDB_NAMES ${BDB_NAMES} db_cxx)
find_library(BDB_LIBRARY NAMES ${BDB_NAMES} NO_DEFAULT_PATH PATHS
    ${HT_DEPENDENCY_LIB_DIR}
    /usr/local/BerkeleyDB.4.8/lib
    /usr/local/lib/db48
    /opt/local/lib/db48
    /usr/local/lib
    /usr/lib
    )

if (BDB_LIBRARY AND BDB_INCLUDE_DIR)
  set(BDB_LIBRARIES ${BDB_LIBRARY})
  set(BDB_FOUND "YES")
else ()
  set(BDB_FOUND "NO")
endif ()


if (BDB_FOUND)
  if (NOT BDB_FIND_QUIETLY)
    message(STATUS "Found BerkeleyDB: ${BDB_LIBRARIES}")
  endif ()
else ()
  if (BDB_FIND_REQUIRED)
    message(FATAL_ERROR "Could not find BerkeleyDB library")
  endif ()
endif ()


mark_as_advanced(
  BDB_LIBRARY
  BDB_INCLUDE_DIR
  )
