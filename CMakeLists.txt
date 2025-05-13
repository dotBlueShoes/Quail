cmake_minimum_required (VERSION 3.28 FATAL_ERROR)
project (QuailAll VERSION 0.3.0 LANGUAGES C CXX)

message ("\n")

# --- Includes

list (APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
include (properties)


# --- Configurable Properties

option (CURL_SHARED_LIBRARY     "Link 'curl' as shared library"             OFF)
option (BLUELIB_LOCALLY         "Link 'bluelib' sources from local cache"   OFF)

# --- Dependencies

add_subdirectory (dependencies)

# --- Add 'defines' to all projects

add_compile_definitions (_CRT_SECURE_NO_WARNINGS)
add_compile_definitions (DEBUG_FLAG_LOGGING=${DEBUG_FLAG_LOGGING})
add_compile_definitions (DEBUG_FLAG_MEMORY=${DEBUG_FLAG_MEMORY})
add_compile_definitions (DEBUG_FLAG_CLOCKS=${DEBUG_FLAG_CLOCKS})
add_compile_definitions (DEBUG_FLAG_POSTLOGGING=${DEBUG_FLAG_POSTLOGGING})

# --- Project's sources

add_subdirectory (project/global)
add_subdirectory (project/quail)
add_subdirectory (project/myinstall)
add_subdirectory (project/myuninstall)
