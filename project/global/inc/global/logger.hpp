// Created 2025.03.09 by Matthew Strumiłło (dotBlueShoes)
//  LICENSE: GNU GENERAL PUBLIC LICENSE Version 3, 29 June 2007
//
#pragma once
#define CONSOLE_COLOR_ENABLED
#define LOGGER_TIME_FORMAT "%f"
//
#if DEBUG_TYPE == 0

	#define LOGERROR(...) { \
		SetConsoleTextAttribute (GetStdHandle (STD_OUTPUT_HANDLE), CNS_CLR_ERR); \
		fprintf (stdout, "\n\tQuail-Error"); \
		SetConsoleTextAttribute (GetStdHandle (STD_OUTPUT_HANDLE), CNS_CLR_DEF); \
		fprintf (stdout, ": " __VA_ARGS__); \
	}

	#define ERROR_NEW_LINE "\n\n"

#endif
