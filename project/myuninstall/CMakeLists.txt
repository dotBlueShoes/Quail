# --- Define a 'subproject'
project (myuninstall VERSION 0.3.0 LANGUAGES C CXX)


# --- Add 'headers' to a variable.
file (GLOB_RECURSE HEADER_FILES
	${PROJECT_SOURCE_DIR}/inc/*.h
	${PROJECT_SOURCE_DIR}/inc/*.hpp
)


# --- Profile-based actions
if ( ${PROFILE} EQUAL 0 ) 

	message ("Debug myuninstall Compilation!")
	add_compile_definitions (DEBUG_TYPE=${DEBUG_FLAG_LOGGING}+${DEBUG_FLAG_MEMORY})
	
elseif ( ${PROFILE} EQUAL 1 )

	message ("Release myuninstall Compilation!")
	add_compile_definitions (DEBUG_TYPE=0)
	
endif ()


# --- Windows Resource Files
set (WIN_RESOURCES ${PROJECT_SOURCE_DIR}/res/res_main.rc)


# --- Build Type
add_executable (
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
target_link_libraries (${PROJECT_NAME} kernel32 Comctl32 dwmapi user32 shell32 ole32 Shlwapi)
target_link_libraries (${PROJECT_NAME} BLUELIB)
target_link_libraries (${PROJECT_NAME} GLOBAL)
