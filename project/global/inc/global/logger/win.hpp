// Created 2025.04.02 by Matthew Strumiłło (dotBlueShoes)
//  LICENSE: GNU GENERAL PUBLIC LICENSE Version 3, 29 June 2007
//
#pragma once
#include "logger.hpp"
//
#if DEBUG_TYPE == 0

	#define LOGERROR(...) { \
		c8 error[256]; \
		sprintf (error, __VA_ARGS__); \
		MSGERROR (error); \
	}

#elif DEBUG_TYPE & DEBUG_FLAG_LOGGING

	#define ERROR_EXIT_SLEEP() { \
		LOGWARN ("Console will close in %d milliseconds...\n", 5000) \
		Sleep (5000); \
	}

#endif
