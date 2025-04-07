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

	const c8 SETTING_WC_DEFAULT [] ("isWideCharacters");
	const c8 SETTING_LS_DEFAULT [] ("listingLineSize");

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
			default: { ERROR (LOCALE_ERROR_INVALID_SETTING_VALUE); }
		}
	}


	void GetUnsigned (
		OUT 	u32& value,
		IN 		const u32& stringCount,
		INOUT 	c8* const& string
	) {

		//  ABOUT
		// Custom atoi implementation. This will ERROR instead of returning 0
		//  when error encountered.

		u32 isNotValidNumber = 0;
		u32 i = 0;
		
		value = 0;
		for (; i < stringCount; ++i) {
			isNotValidNumber += (string[i] - '0') > 10;
			value = value * 10 + (string[i] - '0');
		}

		if (isNotValidNumber) { ERROR (LOCALE_ERROR_INVALID_SETTING_VALUE); }

	}


	void DisplayName (
		IN		const HANDLE& console,
		IN		const u32& settingNameLength,
		IN		const c8* const& settingName
	) {
		const c8 A [] = "\n\tSuccessfully set \"";
		const c8 B [] = "\" to ";

		fwrite (A, sizeof (c8), sizeof (A), stdout);
		SetConsoleTextAttribute (console, 14);
		fwrite (settingName, sizeof (c8), settingNameLength, stdout);
		SetConsoleTextAttribute (console, 7);
		fwrite (B, sizeof (c8), sizeof (B), stdout);
	}


	void DisplayBool (
		IN		const HANDLE& console,
		IN		const bool& value
	) {
		SetConsoleTextAttribute (console, 11);
		if (value) { fwrite (STRING_TRUE, sizeof (c8), sizeof (STRING_TRUE), stdout); } 
		else { fwrite (STRING_FALSE, sizeof (c8), sizeof (STRING_FALSE), stdout); }
		SetConsoleTextAttribute (console, 7);
	}


	void DisplayU32 (
		IN		const HANDLE& console,
		IN		const u32& settingValueLength,
		IN		const c8* const& settingValue
	) {
		SetConsoleTextAttribute (console, 11);
		fwrite (settingValue, sizeof (c8), settingValueLength, stdout);
		SetConsoleTextAttribute (console, 7);
	}


	void MatchSettings (
		IN 		const u32& 	stringsCount,
		IN 		c8** const& strings
	) {

		HANDLE console = GetStdHandle (STD_OUTPUT_HANDLE);
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
					WINDOWS::REGISTRY::SetPropertyIsForceC8Display (value);

					{ // Display
						DisplayName (console, sizeof (SETTING_WC_DEFAULT), SETTING_WC_DEFAULT);
						DisplayBool (console, value);
					}

				} break;

				case ENUM_SETTING_LS:  {

					u32 value;
					GetUnsigned (value, settingValueLength, settingValue);
					WINDOWS::REGISTRY::SetPropertyListingLineSize (value);

					{ // Display
						DisplayName (console, sizeof (SETTING_LS_DEFAULT), SETTING_LS_DEFAULT);
						DisplayU32 (console, settingValueLength, settingValue);
					}

				} break;

				default: {
					ERROR (LOCALE_ERROR_INVALID_SETTING_NAME);
				} break;
			}
		}

		{ // Offset Display
			fwrite ("\n\n", sizeof (c8), sizeof ("\n\n"), stdout);
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
			case 1:  { ERROR (LOCALE_ERROR_INVALID_SETTING_QUEUE); } break;

			default: { ERROR (LOCALE_ERROR_NO_SETTING); } break;
		}

	}

}
