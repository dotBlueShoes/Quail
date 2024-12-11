// Created 2024.11.13 by Matthew Strumiłło (dotBlueShoes)
//
#pragma once
#include "memory.hpp"
#include "log.hpp"

#include <Windows.h>
#ifdef ERROR
	#undef ERROR
#endif

#define ERRORWIN(text) { \
	MSGERROR (text); \
	exit (-1); \
}

#define ERROR(...) { \
	LOGERROR ("\n\t" __VA_ARGS__); \
	 \
	LOGMEMORY (); \
	DEBUG (DEBUG_FLAG_LOGGING) putc ('\n', stdout); \
	 \
	exit (-1); \
}

#define WERROR(...) { \
	LOGWERROR ("\n\t" __VA_ARGS__); \
	 \
	LOGMEMORY (); \
	DEBUG (DEBUG_FLAG_LOGGING) putc ('\n', stdout); \
	 \
	exit (-1); \
}
