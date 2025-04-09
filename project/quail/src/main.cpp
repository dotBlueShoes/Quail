// Created 2024.10.28 by Matthew Strumiłło (dotBlueShoes)
//  LICENSE: GNU GENERAL PUBLIC LICENSE Version 3, 29 June 2007
//
#include <global/logger/cli.hpp>
//
#include <blue/error.hpp>
//
#include "locale/error_codes.hpp"
#include "activities/match.hpp"
#include "windows/registry.hpp"

s32 main (s32 argumentsCount, c8* arguments[]) {

	{ // Init Logging
		TIMESTAMP_BEGIN = TIMESTAMP::GetCurrent ();
		DEBUG (DEBUG_FLAG_LOGGING) putc ('\n', stdout); // Align fututre debug-logs
		LOGINFO ("Application Statred!\n");
	}

	{ // TODO. Optimize it.
		LOGWINFO ("[ REGISTRY ]\n");
		
		WINDOWS::REGISTRY::GetPropertyTopConfigsFolder (
			CONFIG::topConfigsFolderLength, 
			CONFIG::topConfigsFolder
		);

		WINDOWS::REGISTRY::GetPropertyIsWideCharacters (
			CONFIG::isWideCharacters
		);

		WINDOWS::REGISTRY::GetPropertyListingLineSize (
			CONFIG::listingLineSize
		);

		WINDOWS::REGISTRY::CreateQuailConfigsFilePaths (
			CONFIG::topConfigsFolderLength, CONFIG::topConfigsFolder,
			CONFIG::configMainFilePathLength, CONFIG::configMainFilePath,
			CONFIG::configGlobalFilePathLength, CONFIG::configGlobalFilePath
		);
	}

	{ // Properly deallocate data if we hit ERROR.
		MEMORY::EXIT::PUSH (CONFIG::topConfigsFolder, FREE);
		MEMORY::EXIT::PUSH (CONFIG::configMainFilePath, FREE);
		MEMORY::EXIT::PUSH (CONFIG::configGlobalFilePath, FREE);
	}

	switch (argumentsCount) {

		case 0:
		case 1: { printf ("\n%s%s\n\n", CONFIG::QUAIL_LINE_OFFSET, LOCALE_ERROR_NO_ACTIVITY); } break;

		default: {
			auto&& argument = arguments[1];
			auto depth = argumentsCount - 2;
			auto commands = arguments + 2;
		
			u32 argumentLength = 0; for (; argument[argumentLength] != 0; ++argumentLength);
			ACTIVITIES::MATCH::Activity (argumentLength, argument, depth, commands);
		}
		
	}

	// { // There's actually no need to do so. 
	// 	MEMORY::EXIT::POP ();
	// 	MEMORY::EXIT::POP ();
	// 	MEMORY::EXIT::POP ();
	// }

	FREE (CONFIG::topConfigsFolder);
	FREE (CONFIG::configMainFilePath);
	FREE (CONFIG::configGlobalFilePath);

	{ // Deinit Logging
		LOGMEMORY ();
		LOGINFO ("Finalized Execution\n");
		DEBUG (DEBUG_FLAG_LOGGING) putc ('\n', stdout); // Align debug-logs
	}

	return 0;
}
