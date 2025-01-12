// Created 2024.10.28 by Matthew Strumiłło (dotBlueShoes)
//  LICENSE: GNU GENERAL PUBLIC LICENSE Version 3, 29 June 2007
//
#pragma once
#include "types.hpp"
#include "io_types.hpp"
#include "debug.hpp"

#include "windows/types.hpp"

#define MSGINFO(message) { \
	DEBUG (DEBUG_FLAG_LOGGING) \
	MessageBoxA (NULL, message, "INFO", MB_OK); \
}

#define MSGWARN(message) { \
	DEBUG (DEBUG_FLAG_LOGGING) \
	MessageBoxA (NULL, message, "WARN", MB_OK); \
}

#define MSGERROR(message) { \
	DEBUG (DEBUG_FLAG_LOGGING) \
	MessageBoxA (NULL, message, "ERROR", MB_OK); \
}

#define LOGINFO(...) { \
	DEBUG (DEBUG_FLAG_LOGGING) \
	printf ("INFO: " __VA_ARGS__); \
}

#define LOGWINFO(...) { \
	DEBUG (DEBUG_FLAG_LOGGING) \
	wprintf (L"INFO: " __VA_ARGS__); \
}

#define LOGWARN(...) { \
	DEBUG (DEBUG_FLAG_LOGGING) \
	printf ("WARN: " __VA_ARGS__); \
}

#define LOGWWARN(...) { \
	DEBUG (DEBUG_FLAG_LOGGING) \
	wprintf (L"WARN: " __VA_ARGS__); \
}

#define LOGERROR(...) { \
	printf ("ERRR: " __VA_ARGS__); \
}

#define LOGWERROR(...) { \
	wprintf (L"ERRR: " __VA_ARGS__); \
}
