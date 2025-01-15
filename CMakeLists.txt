cmake_minimum_required (VERSION 3.28 FATAL_ERROR)
project (QuailAll VERSION 0.3.0 LANGUAGES C CXX)

message ("\n")

# --- Includes

list (APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
include (properties)

# --- Dependencies

add_subdirectory (dependencies)

# --- Add a DUMMY variable that will force refresh every build.
# ISSUE (CMAKE does not like edititng source files.)
#
#set (DUMMY_OUTPUT "${CMAKE_BINARY_DIR}/.dummy_output")
#
#add_custom_command (
#    OUTPUT ${DUMMY_OUTPUT} 
#    COMMAND powershell
#	${CMAKE_SOURCE_DIR}/.quail/incrementBuildVersion.ps1 -filepath "${CMAKE_SOURCE_DIR}"
#    COMMENT "Executing PowerShell script before building target"
#    VERBATIM
#)
#
#add_custom_target (BuildCallback ALL DEPENDS ${DUMMY_OUTPUT})

# --- Add 'defines' to all projects

add_compile_definitions (_CRT_SECURE_NO_WARNINGS)
add_compile_definitions (DEBUG_FLAG_LOGGING=${DEBUG_FLAG_LOGGING})
add_compile_definitions (DEBUG_FLAG_MEMORY=${DEBUG_FLAG_MEMORY})
add_compile_definitions (DEBUG_FLAG_CLOCKS=${DEBUG_FLAG_CLOCKS})
add_compile_definitions (DEBUG_FLAG_POSTLOGGING=${DEBUG_FLAG_POSTLOGGING})

# --- Project's sources

add_subdirectory (project/bluelib)
add_subdirectory (project/global)
add_subdirectory (project/quail)
add_subdirectory (project/installer)
add_subdirectory (project/uninstaller)
