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

	// jysk

	const u16 STRING_SIZE		= 5;
	// + here could add other String values (enum types).
	const c8  STRING_FALSE  	[] ("false" ""  );
	const c8  STRING_TRUE   	[] ("true" "\0" );
	const c8  STRING_ERROR   	[] ("\0\0\0\0\0");

	const arr8<const c8*, 3> STRINGS {
		STRING_FALSE, STRING_TRUE, STRING_ERROR
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
		OUT		u32& value,
		IN 		const u32& stringCount,
		IN	 	c8* const& string
	) {

		//  ABOUT
		// Before calling this function set the value to 0!
		// This is because value is also treated as index.
		// In C/CPP false is 0, true is 1 and we abuse it.

		// Conversion
		ToLowCase (string, stringCount); 

		// Try match with possible strings (bool section only)
		COMPARESEARCH::ArrayPartFirstMatch ( 
			string, stringCount, sizeof (c8),
			value, 
			STRINGS.size (),
			STRINGS.data ()
		);

		if (value > 1) ERROR (LOCALE_ERROR_INVALID_SETTING_VALUE);
	}


	//  ABOUT, TODO
	// The valid types are < u8, u16, u32, u64 >.
	//
	template <typename T>
	void GetUnsigned (
		OUT 	T& value,
		IN 		const T& stringCount,
		IN 		const c8* const& string
	) {

		//  ABOUT
		// Custom atoi implementation. This will ERROR instead of returning 0
		//  when error encountered.

		T isNotValidNumber = 0;
		T i = 0;
		
		value = 0;
		for (; i < stringCount; ++i) {
			isNotValidNumber += (string[i] - '0') > 10;
			value = value * 10 + (string[i] - '0');
		}

		if (isNotValidNumber) ERROR (LOCALE_ERROR_INVALID_SETTING_VALUE);
	}


	void DisplayName (
		IN		const HANDLE& console,
		IN		const u32& settingNameLength,
		IN		const c8* const& settingName
	) {
		const c8 A [] = "\n\tSuccessfully set \"";
		const c8 B [] = "\" from ";

		fwrite (A, sizeof (c8), sizeof (A), stdout);
		SetConsoleTextAttribute (console, 14);
		fwrite (settingName, sizeof (c8), settingNameLength, stdout);
		SetConsoleTextAttribute (console, 7);
		fwrite (B, sizeof (c8), sizeof (B), stdout);
	}


	void DisplayBool (
		IN		const HANDLE& console,
		IN		const bool& previousValue,
		IN		const bool& value
	) {
		const c8 A [] = " to ";

		SetConsoleTextAttribute (console, 11);
		fwrite (STRINGS[previousValue], sizeof (c8), STRING_SIZE, stdout);
		SetConsoleTextAttribute (console, 7);

		fwrite (A, sizeof (c8), sizeof (A), stdout);

		SetConsoleTextAttribute (console, 11);
		fwrite (STRINGS[value], sizeof (c8), STRING_SIZE, stdout);
		SetConsoleTextAttribute (console, 7);
	}


	void DisplayU32 (
		IN		const HANDLE& console,
		IN		const u32& previousValue,
		IN		const u32& settingValueLength,
		IN		const c8* const& settingValue
	) {
		const c8 A [] = " to ";

		SetConsoleTextAttribute (console, 11);
		fprintf  (stdout, "%d", previousValue);
		SetConsoleTextAttribute (console, 7);

		fwrite (A, sizeof (c8), sizeof (A), stdout);

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

					{ // Boolean is a String subtype.
						index = 0;
						GetBool (index, settingValueLength, settingValue);
					}

					WINDOWS::REGISTRY::SetPropertyIsWideCharacters (index);

					{ // Display
						DisplayName (console, sizeof (SETTING_WC_DEFAULT), SETTING_WC_DEFAULT);
						DisplayBool (console, CONFIG::isWideCharacters, index);
					}

				} break;

				case ENUM_SETTING_LS:  {

					u16 value;
					GetUnsigned<u16> (value, settingValueLength, settingValue);

					const bool isInvalidRange = value < CONFIG::LISTING_LINE_SIZE_MIN && value != 0;
					
					if (isInvalidRange) {

						{ // Range validation update.
							const c8 msg [] = "\n\tValue is in invalid range! Defaulting to \"NO_CAP\" (0).";

							SetConsoleTextAttribute (console, 4);
							fwrite (msg, sizeof (c8), sizeof (msg), stdout);
							SetConsoleTextAttribute (console, 7);

							value = 0;
						}

						WINDOWS::REGISTRY::SetPropertyListingLineSize (value);

						{ // Display
							DisplayName (console, sizeof (SETTING_LS_DEFAULT), SETTING_LS_DEFAULT);
							DisplayU32 (console, CONFIG::listingLineSize, 2, "0");
						}

					} else {

						WINDOWS::REGISTRY::SetPropertyListingLineSize (value);

						{ // Display
							DisplayName (console, sizeof (SETTING_LS_DEFAULT), SETTING_LS_DEFAULT);
							DisplayU32 (console, CONFIG::listingLineSize, settingValueLength, settingValue);
						}

					}

				} break;

				default: { ERROR (LOCALE_ERROR_INVALID_SETTING_NAME); } 
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
