// Created 2025.04.02 by Matthew Strumiłło (dotBlueShoes)
//  LICENSE: GNU GENERAL PUBLIC LICENSE Version 3, 29 June 2007
//
#pragma once
#include "../config.hpp"
#include "logger.hpp"
//
#if DEBUG_TYPE == 0

	#define LOGERROR(...) { \
		SetConsoleTextAttribute (GetStdHandle (STD_OUTPUT_HANDLE), CNS_CLR_ERR); \
		fprintf (stdout, "\n%sQuail-Error", CONFIG::QUAIL_LINE_OFFSET); \
		SetConsoleTextAttribute (GetStdHandle (STD_OUTPUT_HANDLE), CNS_CLR_DEF); \
		fprintf (stdout, ": " __VA_ARGS__); \
	}

	#define ERROR_NEW_LINE "\n\n"

#endif
