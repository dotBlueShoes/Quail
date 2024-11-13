// Created 2024.10.28 by Matthew Strumiłło (dotBlueShoes)
//
#pragma once
#include "types.hpp"
#include "io_types.hpp"
#include "debug.hpp"


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


#include <Windows.h>
#ifdef ERROR
	#undef ERROR
#endif


#define ERROR(...) { \
	LOGERROR ("\n\t" __VA_ARGS__); \
	exit (-1); \
}

#define WERROR(...) { \
	LOGWERROR ("\n\t" __VA_ARGS__); \
	exit (-1); \
}
