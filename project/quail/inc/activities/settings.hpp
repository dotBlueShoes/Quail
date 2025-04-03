// Created 2025.04.03 by Matthew Strumiłło (dotBlueShoes)
//  LICENSE: GNU GENERAL PUBLIC LICENSE Version 3, 29 June 2007
//
#pragma once
#include "blue/comparesearch.hpp"
#include <blue/error.hpp>
//
#include "activities.hpp"
#include "../windows/registry.hpp"

namespace ACTIVITIES {

	// + here could add other String values (enum types).
	const c8 STRING_FALSE  	[] ("false" "" );
	const c8 STRING_TRUE   	[] ("true" "\0");

	const arr8<const c8*, 3> STRINGS {
		STRING_FALSE, STRING_TRUE, ERROR_TYPE
	};

	const c8 SETTING_WC [] ("iswidecharacters" 	""		);
	const c8 SETTING_LS [] ("listinglinesize" 	"\0"	);

	const arr8<const c8*, 3> SETTINGS {
		SETTING_WC, SETTING_LS, ERROR_TYPE
	};

	enum ENUM_SETTING: u8 {
		ENUM_SETTING_WC 	= 0,
		ENUM_SETTING_LS 	= 1,
		ENUM_SETTING_ERROR 	= 2,
	};


	void GetBool (
		OUT 	bool& value,
		IN 		const u32& stringCount,
		INOUT 	c8* const& string
	) {
		u32 index = 0;

		// Conversion
		ToLowCase (string, stringCount); 

		// Try match with MAXs
		COMPARESEARCH::ArrayPartFirstMatch ( 
			string, stringCount, sizeof (c8),
			index, 
			STRINGS.size (),
			STRINGS.data ()
		);

		switch (index) {
			case 0:  { value = false; } return;
			case 1:  { value = true;  } return;
			default: { ERROR ("Incorrect setting value!\n"); }
		}
	}


	void GetUnsigned (
		OUT 	u32& value,
		IN 		const u32& stringCount,
		INOUT 	c8* const& string
	) {
		value = CONFIG::listingLineSize;
		//  TODO
		// 1. Validate that each character in string is a number
		// 40 to 49 codes ?
		// 2. Transform chars to int.
	}


	void MatchSettings (
		IN 		const u32& 	stringsCount,
		IN 		c8** const& strings
	) {
		auto keysCount = stringsCount / 2;

		for (u32 i = 0; i < keysCount; ++i) {
			
			auto&& settingValue = strings[(i * 2) + 1];
			auto&& settingKey = strings[(i * 2) + 0];

			u32 settingValueLength = 0; for (; settingValue[settingValueLength] != '\0'; ++settingValueLength);
			u32 settingKeyLength = 0; for (; settingKey[settingKeyLength] != '\0'; ++settingKeyLength);

			u32 index = 0;

			// Conversion
			ToLowCase (settingKey, settingKeyLength); 

			// Try match with MAXs
			COMPARESEARCH::ArrayPartFirstMatch ( 
				settingKey, settingKeyLength, sizeof (c8),
				index, 
				SETTINGS.size (),
				SETTINGS.data ()
			);

			switch (index) {
				case ENUM_SETTING_WC:  {

					bool value;
					GetBool (value, settingValueLength, settingValue);

					if (value) { LOGINFO ("%s, true\n", settingKey); }
					else { LOGINFO ("%s, false\n", settingKey); }

					WINDOWS::REGISTRY::SetPropertyIsForceC8Display (value);

				} break;

				//case ENUM_SETTING_LS:  {
				//	LOGINFO ("%s, %s!\n", settingKey, settingValue);
				//} break;

				default: {
					ERROR ("Incorrect setting name!\n");
				} break;
			}
		}
	}


	void Settings (
		IN 		const u32& 	stringsCount,
		IN 		c8** const& strings
	) {

		u8 condition;
		condition  = ((stringsCount % 2) == 1) << 0;
		condition += (stringsCount == 0      ) << 1;

		switch (condition) {
			case 0:  { MatchSettings (stringsCount, strings); } break;
			case 1:  { ERROR ("missing string\n"); } break;

			default: { ERROR ("no strings at all\n"); } break;
		}

	}

}
