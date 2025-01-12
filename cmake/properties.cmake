set_property (GLOBAL PROPERTY USE_FOLDERS ON)
set_property (GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "CMake")

set (CMAKE_CXX_STANDARD 23)
set (CMAKE_DISABLE_IN_SOURCE_BUILD ON)
set (CMAKE_DISABLE_SOURCE_CHANGES ON)

set (DEBUG_FLAG_LOGGING 	1)
set (DEBUG_FLAG_MEMORY 		2)
set (DEBUG_FLAG_CLOCKS 		4)
set (DEBUG_FLAG_POSTLOGGING 5)

# --- Turn on reloading for XAML code on hot for MSVC compilators it it's available.
if (POLICY CMP0141)
	cmake_policy (SET CMP0141 NEW)
	set (CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif ()
