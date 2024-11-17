# --- Add header files
file (GLOB_RECURSE HEADER_FILES
	${PROJECT_SOURCE_DIR}/inc/*.h
	${PROJECT_SOURCE_DIR}/inc/*.hpp
)

add_compile_definitions (_CRT_SECURE_NO_WARNINGS)
add_compile_definitions (DEBUG_FLAG_LOGGING=${DEBUG_FLAG_LOGGING})
add_compile_definitions (DEBUG_FLAG_MEMORY=${DEBUG_FLAG_MEMORY})
add_compile_definitions (DEBUG_FLAG_CLOCKS=${DEBUG_FLAG_CLOCKS})
add_compile_definitions (DEBUG_FLAG_POSTLOGGING=${DEBUG_FLAG_POSTLOGGING})

if ( ${PROFILE} EQUAL 0 ) 

	message ("Debug Compilation!")
	add_compile_definitions (DEBUG_TYPE=${DEBUG_FLAG_LOGGING}+${DEBUG_FLAG_MEMORY})
	
elseif ( ${PROFILE} EQUAL 1 )

	message ("Release Compilation!")
	add_compile_definitions (DEBUG_TYPE=0)
	
endif ()

# WinAPI EXE
#add_executable (
#	${PROJECT_NAME} WIN32 ${HEADER_FILES} 
#	src/main.cpp
#)

# --- CONSOLE EXE
add_executable (
	${PROJECT_NAME} ${HEADER_FILES} 
	src/main.cpp
)

target_include_directories (
	${PROJECT_NAME} PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/inc
)

# --- Creation of a symlink to res inside build folder.
add_custom_command (
	TARGET ${PROJECT_NAME} POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E create_symlink
	${CMAKE_SOURCE_DIR}/project/res
	${CMAKE_CURRENT_BINARY_DIR}/res
)