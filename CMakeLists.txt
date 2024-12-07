cmake_minimum_required (VERSION 3.28 FATAL_ERROR)
project (QuailAll VERSION 0.3.0 LANGUAGES C CXX)

# --- Includes

list (APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
include (properties)

# --- Dependencies

add_subdirectory (dependencies)

# --- Project's sources

add_subdirectory (project/bluelib)
add_subdirectory (project/quail)
add_subdirectory (project/installer)
