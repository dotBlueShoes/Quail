// Created 2025.04.03 by Matthew Strumiłło (dotBlueShoes)
//  LICENSE: GNU GENERAL PUBLIC LICENSE Version 3, 29 June 2007
//
#pragma once
#include <blue/comparesearch.hpp>
#include <blue/error.hpp>
//
#include <global/windows/registry.hpp>
//
#include "activities/activities.hpp"
#include "locale/error_codes.hpp"
//
#include "property.hpp"
#include "display.hpp"

namespace ACTIVITIES::SETTINGS {

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

					auto&& settingName = SETTINGS[index];
					bool value;

					{ // Boolean is a String subtype.
						index = 0;
						PROPERTY::GetBool (index, settingValueLength, settingValue, settingName);
					}

					WINDOWS::REGISTRY::PROPERTY::SET::IsWideCharacters (index);

					{ // Display
						DISPLAY::Name (console, sizeof (SETTING_WC_DEFAULT), SETTING_WC_DEFAULT);
						DISPLAY::Bool (console, CONFIG::isWideCharacters, index);
					}

				} break;

				case ENUM_SETTING_LS:  {

					auto&& settingName = SETTINGS[index];
					u16 value;

					PROPERTY::GetUnsigned<u16> (value, settingValueLength, settingValue, settingName);

					const bool isInvalidRange = value < CONFIG::LISTING_LINE_SIZE_MIN && value != 0;
					
					if (isInvalidRange) {

						{ // Range validation update.
							const c8 msg [] = "\n\tValue is in invalid range! Defaulting to \"NO_CAP\" (0).";

							SetConsoleTextAttribute (console, 4);
							fwrite (msg, sizeof (c8), sizeof (msg), stdout);
							SetConsoleTextAttribute (console, 7);

							value = 0;
						}

						WINDOWS::REGISTRY::PROPERTY::SET::ListingLineSize (value);

						{ // Display
							DISPLAY::Name (console, sizeof (SETTING_LS_DEFAULT), SETTING_LS_DEFAULT);
							DISPLAY::U32 (console, CONFIG::listingLineSize, 2, "0");
						}

					} else {

						WINDOWS::REGISTRY::PROPERTY::SET::ListingLineSize (value);

						{ // Display
							DISPLAY::Name (console, sizeof (SETTING_LS_DEFAULT), SETTING_LS_DEFAULT);
							DISPLAY::U32 (console, CONFIG::listingLineSize, settingValueLength, settingValue);
						}

					}

				} break;

				default: { ERROR (LOCALE_ERROR_INVALID_SETTING_NAME, settingKey); } 
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

			default: { ERROR (LOCALE_ERROR_NO_SETTING); }
		}

	}

}
