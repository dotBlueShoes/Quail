include (FetchContent)

# --- CURL (STATIC LINKING ALL INCLUDING IT'S LIBRARIES)

if (NOT ${CURL_SHARED_LIBRARY})

    set (CURL_USE_LIBPSL OFF)
    set (CURL_USE_SCHANNEL ON)
    set (BUILD_SHARED_LIBS OFF)
    set (BUILD_STATIC_LIBS ON)
    set (CURL_STATICLIB ON)
    set (CURL_ZLIB OFF)

else ()

    set (CURL_USE_LIBPSL OFF)
    set (CURL_USE_SCHANNEL ON)
    set (BUILD_SHARED_LIBS ON)
    set (BUILD_STATIC_LIBS OFF)
    set (CURL_STATICLIB OFF)
    set (CURL_ZLIB OFF)

endif ()

FetchContent_Declare (
	curl
	GIT_REPOSITORY "https://github.com/curl/curl.git"
	GIT_TAG "curl-8_11_1"
	GIT_SHALLOW TRUE
)

FetchContent_MakeAvailable (curl)

# --- BLUELIB

if (NOT ${BLUELIB_LOCALLY})

	message (STATUS "DISABLED - Locally library BLUE")
	
	FetchContent_Declare (
		BLUELIB
		GIT_REPOSITORY https://github.com/dotBlueShoes/blue.git
		GIT_TAG master
	)
	
	FetchContent_MakeAvailable (BLUELIB)

else ()

	message (STATUS "ENABLED - Locally library BLUE")
	
	# --- Define 'bluelib'
	set (BLUELIB_INC_DIR ${CMAKE_CURRENT_LIST_DIR}/bluelib/include)
	add_library (BLUELIB INTERFACE)
	target_include_directories (BLUELIB INTERFACE ${BLUELIB_INC_DIR})

endif ()
