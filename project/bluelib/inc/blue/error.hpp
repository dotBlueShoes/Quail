// Created 2024.11.13 by Matthew Strumiłło (dotBlueShoes)
//  LICENSE: GNU GENERAL PUBLIC LICENSE Version 3, 29 June 2007
//
#pragma once
#include "memory.hpp"
#include "log.hpp"
#include "windows/types.hpp"

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
	getchar (); \
	exit (-1); \
}

#define WERROR(...) { \
	LOGWERROR ("\n\t" __VA_ARGS__); \
	 \
	LOGMEMORY (); \
	DEBUG (DEBUG_FLAG_LOGGING) putc ('\n', stdout); \
	 \
	getchar (); \
	exit (-1); \
}
