// Created 2025.04.15 by Matthew Strumiłło (dotBlueShoes)
//  LICENSE: GNU GENERAL PUBLIC LICENSE Version 3, 29 June 2007
//
#pragma once
#include <blue/comparesearch.hpp>
#include <blue/concepts.hpp>
#include <blue/error.hpp>
//
#include "locale/error_codes.hpp"
//
#include "data.hpp"

namespace ACTIVITIES::SETTINGS::PROPERTY {

	//  ABOUT
	// 1. Before calling this function set the value to 0!
	// This is because value is also treated as index.
	// In C/CPP false is 0, true is 1 and we abuse it.
	//
	// 2. ToLowCase requires non-const string.
	// Theres no meaning in creating a copy so we're using 
	// the exsisting one.
	//
	void GetBool (
		INOUT	u32& value,
		IN 		const u32& stringCount,
		INOUT	c8* const& string,
		IN		const c8* const& keyName
	) {
		// Conversion
		ToLowCase (string, stringCount); 

		// Try match with possible strings (bool section only)
		COMPARESEARCH::ArrayPartFirstMatch ( 
			string, stringCount, sizeof (c8),
			value, 
			STRINGS.size (),
			STRINGS.data ()
		);

		if (value > 1) ERROR (LOCALE_ERROR_INVALID_SETTING_VALUE, string, keyName);
	}


	//  ABOUT
	// Custom atoi implementation. This will ERROR instead of returning 0
	//  when error encountered.
	//
	template <typeunsigned T>
	void GetUnsigned (
		OUT 	T& value,
		IN 		const T& stringCount,
		IN 		const c8* const& string,
		IN		const c8* const& keyName
	) {
		T isNotValidNumber = 0;
		T i = 0;
		
		value = 0;
		for (; i < stringCount; ++i) {
			isNotValidNumber += (string[i] - '0') > 10;
			value = value * 10 + (string[i] - '0');
		}

		if (isNotValidNumber) ERROR (LOCALE_ERROR_INVALID_SETTING_VALUE, string, keyName);
	}

}