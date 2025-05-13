// Created 2024.10.28 by Matthew Strumiłło (dotBlueShoes)
//  LICENSE: GNU GENERAL PUBLIC LICENSE Version 3, 29 June 2007
//

#include <global/logger/cli.hpp>
#include <global/windows/registry.hpp>
//
#include <blue/error.hpp>
//
#include "locale/error_codes.hpp"
#include "activities/match.hpp"


void CreateQuailConfigsFilePaths (
	INOUT 	u32& 	folderPathLength,
	INOUT 	c16*& 	folderPath,
	OUT 	u32& 	mConfigFilePathLength,
	OUT 	c16*& 	mConfigFilePath,
	OUT 	u32& 	gConfigFilePathLength,
	OUT 	c16*& 	gConfigFilePath
) {

	{ // Replace '\0' with '\\'. So that we can later create strings with it simpler.
		wmemset (folderPath + (folderPathLength / 2) - 1, L'\\', 1);
	}

	mConfigFilePathLength = folderPathLength + 1 + CONFIG::CONFIG_MAIN_LENGTH;
	ALLOCATE (c16, mConfigFilePath, mConfigFilePathLength);

	{ // CONSTRUCT
		memcpy (mConfigFilePath, folderPath, folderPathLength);
		memcpy (mConfigFilePath + (folderPathLength / 2), CONFIG::CONFIG_MAIN_W, CONFIG::CONFIG_MAIN_LENGTH);
	}

	gConfigFilePathLength = folderPathLength + 1 + CONFIG::CONFIG_GLOBAL_LENGTH;
	ALLOCATE (c16, gConfigFilePath, gConfigFilePathLength);

	{ // CONSTRUCT
		memcpy (gConfigFilePath, folderPath, folderPathLength);
		memcpy (gConfigFilePath + (folderPathLength / 2), CONFIG::CONFIG_GLOBAL_W, CONFIG::CONFIG_GLOBAL_LENGTH);
	}
}


s32 main (s32 argumentsCount, c8** arguments) {

	{ // Init Logging
		TIMESTAMP_BEGIN = TIMESTAMP::GetCurrent ();
		DEBUG (DEBUG_FLAG_LOGGING) putc ('\n', stdout); // Align fututre debug-logs
		LOGINFO ("Application Statred!\n");
	}

	{ // TODO. Optimize the key constant reopening.
		LOGWINFO ("[ REGISTRY ]\n");
		
		WINDOWS::REGISTRY::PROPERTY::GET::TopConfigsFolder (
			CONFIG::topConfigsFolderLength, 
			CONFIG::topConfigsFolder
		);

		WINDOWS::REGISTRY::PROPERTY::GET::IsWideCharacters (
			CONFIG::isWideCharacters
		);

		WINDOWS::REGISTRY::PROPERTY::GET::ListingLineSize (
			CONFIG::listingLineSize
		);

		CreateQuailConfigsFilePaths (
			CONFIG::topConfigsFolderLength, CONFIG::topConfigsFolder,
			CONFIG::configMainFilePathLength, CONFIG::configMainFilePath,
			CONFIG::configGlobalFilePathLength, CONFIG::configGlobalFilePath
		);
	}

	{ // Properly deallocate data if we hit ERROR.
		MEMORY::EXIT::PUSH (FREE, CONFIG::topConfigsFolder);
		MEMORY::EXIT::PUSH (FREE, CONFIG::configMainFilePath);
		MEMORY::EXIT::PUSH (FREE, CONFIG::configGlobalFilePath);
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
