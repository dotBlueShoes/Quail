cmake_minimum_required (VERSION 3.28 FATAL_ERROR)
project (Quail VERSION 0.3.0 LANGUAGES C CXX)

# --- Includes
list (APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
include (properties)

# --- Project's sources
add_subdirectory (project)