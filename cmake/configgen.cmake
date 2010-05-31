
# generate config.h file

execute_process(
	COMMAND gcc --version
	OUTPUT_VARIABLE GCC_INFO
)

# string(REGEX REPLACE " " "\1" GCC_INFO ${GCC_INFO})

# message(STATUS ${GCC_INFO})

FILE(WRITE config.h
  "#ifndef _HAVE_CONFIG_\n"
  "# define _HAVE_CONFIG_\n"
  "  #define HYPERTABLE_CONF ${HYPERTABLE_CONF}\n"
  "  #define SYSTEM_NAME \"${CMAKE_SYSTEM_NAME}\"\n"
  "  #define SYSTEM_MACHINE \"${CMAKE_SYSTEM_PROCESSOR}\"\n"
  "  #define COMPILER_NAME \"${CMAKE_C_COMPILER}\"\n"
  "#endif // _HAVE_CONFIG_\n"
)
