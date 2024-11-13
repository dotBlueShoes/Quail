// Created 2024.11.13 by Matthew Strumiłło (dotBlueShoes)
//
#pragma once
#include "types.hpp"

enum DEBUG_TYPE : u8 {
	DEBUG_FLAG_LOGGING		= 1,
	DEBUG_FLAG_MEMMORY		= 2,
	DEBUG_FLAG_CLOCKS		= 3,
	DEBUG_FLAG_POSTLOGGING	= 4,
};

const u8 DEBUG_TYPE = 1;

#define DEBUG(type) if constexpr (DEBUG_TYPE & type == type)
