# --- Define a 'subproject'
project (Quail VERSION 0.3.0 LANGUAGES C CXX)

# --- Add 'headers' to a variable.
file (GLOB_RECURSE HEADER_FILES
	${PROJECT_SOURCE_DIR}/inc/*.h
	${PROJECT_SOURCE_DIR}/inc/*.hpp
)

# --- Profile-based actions
if ( ${PROFILE} EQUAL 0 ) 

	message ("Debug Program Compilation!")
	add_compile_definitions (DEBUG_TYPE=${DEBUG_FLAG_LOGGING}+${DEBUG_FLAG_MEMORY})
	
elseif ( ${PROFILE} EQUAL 1 )

	message ("Release Program Compilation!")
	add_compile_definitions (DEBUG_TYPE=0)
	
endif ()

# --- Define as 'console only executable'
add_executable (
	${PROJECT_NAME} ${HEADER_FILES} 

	# --- Add 'sources'
	src/main.cpp
)

# --- Add project 'headers' for use to said project
target_include_directories (
	${PROJECT_NAME} PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/inc
)

# --- Add library 'headers' for use to said project
target_link_libraries (${PROJECT_NAME} BLUELIB)

# --- Creation of a symlink to res inside build folder.
#add_custom_command (
#	TARGET ${PROJECT_NAME} POST_BUILD
#	COMMAND ${CMAKE_COMMAND} -E create_symlink
#	${CMAKE_SOURCE_DIR}/project/res
#	${CMAKE_CURRENT_BINARY_DIR}/res
#)