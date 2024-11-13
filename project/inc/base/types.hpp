// Created 2024.10.28 by Matthew Strumiłło (dotBlueShoes)
//
#pragma once
#include <stdint.h>
#include <wchar.h>
#include <array>

using c8  = char;
using c16 = wchar_t;

using u8  = uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;

using s8  = int8_t;
using s16 = int16_t;
using s32 = int32_t;
using s64 = int64_t;

using r32 = float;
using r64 = double;

template<class T, class S, S length>
using array_t = std::array<T, length>;

template<class T, u8 length>
using arr8  = array_t<T, u8, length>;

template<class T, u16 length>
using arr16 = array_t<T, u16, length>;

template<class T, u32 length>
using arr32 = array_t<T, u32, length>;

template<class T, u32 length>
using arr64 = array_t<T, u32, length>;


// Equality String Test Called `3` with its u8 and u16 version.
//  ...

void IsEqualS3_8 (
	/* OUT */ u8& condition,
	/* IN  */ const u8* const& a,
	/* IN  */ const u8* const& b
) {
	for (u32 i = 0; condition == 0; ++i) {
		condition += (a[i]	!= b[i]) << 2;  // 4
		condition += a[i]	== '\0';		// 1
		condition += b[i]	== '\0';		// 1
	}
}

void IsEqualS3_16 (
	/* OUT */ u8& condition,
	/* IN  */ const u16* const& a,
	/* IN  */ const u16* const& b
) {
	for (u32 i = 0; condition == 0; ++i) {
		condition += (a[i]	!= b[i]) << 2;  // 4
		condition += a[i]	== '\0';		// 1
		condition += b[i]	== '\0';		// 1
	}
}
