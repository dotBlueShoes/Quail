// Created 2024.11.13 by Matthew Strumiłło (dotBlueShoes)
//  LICENSE: GNU GENERAL PUBLIC LICENSE Version 3, 29 June 2007
//
#pragma once
#include "types.hpp"

// IN CASE OF AN EMPTY BUILD HAVE ALL ON BY DEFAULT.
#ifndef DEBUG_TYPE
	#define DEBUG_FLAG_LOGGING 		1
	#define DEBUG_FLAG_MEMORY 		2
	#define DEBUG_FLAG_CLOCKS 		4
	#define DEBUG_FLAG_POSTLOGGING 	8
	#define DEBUG_TYPE 				15 // ALL ON
#endif

#define DDEBUG(type) DEBUG_TYPE & type
#define DEBUG(type) if constexpr (DEBUG_TYPE & type == type)
