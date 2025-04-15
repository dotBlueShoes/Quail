// Created 2025.04.15 by Matthew Strumiłło (dotBlueShoes)
//  LICENSE: GNU GENERAL PUBLIC LICENSE Version 3, 29 June 2007
//
#pragma once
#include <blue/error.hpp>
//
#include "data.hpp"
//
#include "colors.hpp"

namespace ACTIVITIES::SETTINGS::DISPLAY {

	void Name (
		IN		const HANDLE& console,
		IN		const u32& settingNameLength,
		IN		const c8* const& settingName
	) {
		const c8 A [] = "\n\tSuccessfully set \"";
		const c8 B [] = "\" from ";

		fwrite (A, sizeof (c8), sizeof (A), stdout);
		SetConsoleTextAttribute (console, COLOR_YELLOW);
		fwrite (settingName, sizeof (c8), settingNameLength, stdout);
		SetConsoleTextAttribute (console, COLOR_DEFAULT);
		fwrite (B, sizeof (c8), sizeof (B), stdout);
	}


	void Bool (
		IN		const HANDLE& console,
		IN		const bool& previousValue,
		IN		const bool& value
	) {
		const c8 A [] = " to ";

		SetConsoleTextAttribute (console, COLOR_BLUE);
		fwrite (STRINGS[previousValue], sizeof (c8), STRING_SIZE, stdout);
		SetConsoleTextAttribute (console, COLOR_DEFAULT);

		fwrite (A, sizeof (c8), sizeof (A), stdout);

		SetConsoleTextAttribute (console, COLOR_BLUE);
		fwrite (STRINGS[value], sizeof (c8), STRING_SIZE, stdout);
		SetConsoleTextAttribute (console, COLOR_DEFAULT);
	}


	void U32 (
		IN		const HANDLE& console,
		IN		const u32& previousValue,
		IN		const u32& settingValueLength,
		IN		const c8* const& settingValue
	) {
		const c8 A [] = " to ";

		SetConsoleTextAttribute (console, COLOR_BLUE);
		fprintf  (stdout, "%d", previousValue);
		SetConsoleTextAttribute (console, COLOR_DEFAULT);

		fwrite (A, sizeof (c8), sizeof (A), stdout);

		SetConsoleTextAttribute (console, COLOR_BLUE);
		fwrite (settingValue, sizeof (c8), settingValueLength, stdout);
		SetConsoleTextAttribute (console, COLOR_DEFAULT);
	}

}