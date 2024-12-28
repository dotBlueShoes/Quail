# --- Define a 'subproject'
project (Installer VERSION 0.3.0 LANGUAGES C CXX)

# --- Add 'headers' to a variable.
file (GLOB_RECURSE HEADER_FILES
	${PROJECT_SOURCE_DIR}/inc/*.h
	${PROJECT_SOURCE_DIR}/inc/*.hpp
)

# --- Profile-based actions
if ( ${PROFILE} EQUAL 0 ) 

	message ("Debug Installer Compilation!")
	add_compile_definitions (DEBUG_TYPE=${DEBUG_FLAG_LOGGING}+${DEBUG_FLAG_MEMORY})
	
elseif ( ${PROFILE} EQUAL 1 )

	message ("Release Installer Compilation!")
	add_compile_definitions (DEBUG_TYPE=0)
	
endif ()

set (WIN_RESOURCES ${PROJECT_SOURCE_DIR}/res/res_main.rc)
add_executable ( # WinAPI EXE _winmain - starting point
	${PROJECT_NAME} WIN32 ${HEADER_FILES} 
	src/main.cpp
	${WIN_RESOURCES}
)

# --- Add project 'headers' for use to said project
target_include_directories (
	${PROJECT_NAME} PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/inc
)

# --- Add library 'headers' for use to said project
target_link_libraries (${PROJECT_NAME} kernel32 Comctl32)
target_link_libraries (${PROJECT_NAME} BLUELIB)
#target_link_libraries (${PROJECT_NAME} CURL)
#target_link_libraries (${PROJECT_NAME} CURL::libcurl_shared)
target_link_libraries (${PROJECT_NAME} CURL::libcurl_static)


#find_package (CURL REQUIRED)
#target_link_libraries (${PROJECT_NAME} CURL::libcurl)

# --- Copy .dlls as 'Folder CPY' inside project build directory.
#add_custom_command ( 
#	TARGET ${PROJECT_NAME} POST_BUILD
#    COMMAND ${CMAKE_COMMAND} -E copy_directory
#		$<TARGET_FILE:CURL> 
#        $<TARGET_FILE_DIR:${PROJECT_NAME}>
#)
