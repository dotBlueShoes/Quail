# --- Define a 'library' with it's 'headers'
set (GLOBAL_INC_DIR ${CMAKE_CURRENT_LIST_DIR}/inc)
add_library (GLOBAL INTERFACE)
target_include_directories (GLOBAL INTERFACE ${GLOBAL_INC_DIR})
