# --- Define a 'library' with it's 'headers'
set (BLUELIB_INC_DIR ${CMAKE_CURRENT_LIST_DIR}/inc)
add_library (BLUELIB INTERFACE)
target_include_directories (BLUELIB INTERFACE ${BLUELIB_INC_DIR})
