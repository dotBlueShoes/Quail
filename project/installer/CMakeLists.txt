# --- Define a 'subproject'
project (Installer VERSION 0.3.0 LANGUAGES C CXX)

# --- Add 'headers' to a variable.
file (GLOB_RECURSE HEADER_FILES
	${PROJECT_SOURCE_DIR}/inc/*.h
	${PROJECT_SOURCE_DIR}/inc/*.hpp
)

# WinAPI EXE _winmain - starting point
add_executable (
	${PROJECT_NAME} WIN32 ${HEADER_FILES} 
	src/main.cpp
)

# --- Add library 'headers' for use to said project
target_link_libraries (${PROJECT_NAME} BLUELIB)
target_link_libraries (${PROJECT_NAME} CURL)

# --- Copy .dlls inside project build directory.
add_custom_command ( TARGET ${PROJECT_NAME} POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_if_different
	$<TARGET_FILE:CURL> $<TARGET_FILE_DIR:${PROJECT_NAME}>
)
