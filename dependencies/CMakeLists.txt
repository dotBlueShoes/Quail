include (FetchContent)

# --- CURL (STATIC LINKING ALL INCLUDING IT'S LIBRARIES)

set (CURL_USE_SCHANNEL ON)
set (BUILD_SHARED_LIBS OFF)
set (BUILD_STATIC_LIBS ON)
set (CURL_STATICLIB ON)
set (CURL_ZLIB OFF)

FetchContent_Declare (
	curl
	GIT_REPOSITORY "https://github.com/curl/curl.git"
	GIT_TAG "curl-8_11_1"
	GIT_SHALLOW TRUE
)

FetchContent_MakeAvailable (curl)
