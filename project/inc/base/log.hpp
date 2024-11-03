// Created 2024.10.28 by Matthew Strumiłło (dotBlueShoes)
//
#pragma once
#include "types.hpp"
#include "io_types.hpp"

#include <Windows.h>

#ifdef ERROR
	#undef ERROR
#endif

#define ERROR(...) { \
	printf (__VA_ARGS__); \
	exit (-1); \
}

#define WERROR(...) { \
	wprintf (__VA_ARGS__); \
	exit (-1); \
}
