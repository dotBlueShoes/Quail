// Created 2024.11.13 by Matthew Strumiłło (dotBlueShoes)
//  LICENSE: GNU GENERAL PUBLIC LICENSE Version 3, 29 June 2007
//
#pragma once
#include "memory.hpp"
#include "log.hpp"
#include "windows/types.hpp"

#ifndef ERROR_EXIT_SLEEP

	//#define ERROR_EXIT_SLEEP() { \
	//	Sleep (2000); \
	//}

	#define ERROR_EXIT_SLEEP() // dummy

#endif

#define ERRORWIN(text) { \
	MSGERROR (text); \
	MEMORY::EXIT::ATEXIT (); \
	exit (-1); \
}

#define ERROR(...) { \
	LOGERROR (__VA_ARGS__); \
	 \
	MEMORY::EXIT::ATEXIT (); \
	LOGMEMORY (); \
	DEBUG (DEBUG_FLAG_LOGGING) putc ('\n', stdout); \
	 \
	ERROR_EXIT_SLEEP () \
	exit (-1); \
}

#define WERROR(...) { \
	LOGWERROR (__VA_ARGS__); \
	 \
	MEMORY::EXIT::ATEXIT (); \
	LOGMEMORY (); \
	DEBUG (DEBUG_FLAG_LOGGING) putc ('\n', stdout); \
	 \
	ERROR_EXIT_SLEEP () \
	exit (-1); \
}
