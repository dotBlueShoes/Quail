// Created 2025.04.15 by Matthew Strumiłło (dotBlueShoes)
//  LICENSE: GNU GENERAL PUBLIC LICENSE Version 3, 29 June 2007
//
#pragma once
#include "compiler_log.hpp"
//
#if _HAS_CXX20 == 0
	COMPILERLOGERRO ("CXX20 is required! Either build or intelliSense is running an older version!")
#endif
//
#include <concepts>

template <class T> concept typeunsigned = std::unsigned_integral<T>;
template <class T> concept typesigned = std::signed_integral<T>;
