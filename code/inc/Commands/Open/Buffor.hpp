#pragma once
#include "lib/Framework.hpp"

#include "Buffors.hpp"

namespace Commands::Open {

	// Limits
	const uint8 CONSTANTS_LIMIT 			= 16;
	const uint8 SPACE_SIZE_CONSTANTS_INDEX	= 4;

	extern uint8 directoryPath;

	// Space in bytes.
	const uint8 SPACE_SIZE_FILES_COUNT		= 1;
	const uint8 SPACE_SIZE_DIRECTORY_LENGTH = 1;
	// - after directory_length stored is directory string.
	const uint8 SPACE_SIZE_CONSTANTS_COUNT	= 1;
	const uint8 SPACE_SIZE_IMPORTS_COUNT	= 1;
	const uint8 SPACE_SIZE_COMMANDS_COUNT	= 1;
	const uint8 SPACE_SIZE_QUEUES_COUNT		= 1;
	// - after queues count stored are name & cnx strings.
	const uint8 SPACE_SIZE_CONTEXT			= 1;
	const uint8 SPACE_SIZE_NAME				= 1;

	const uint8 INDEX_FILES_COUNT			= 0;

	getter constexpr uint8 GetHeaderIndex (const uint8& i) {
		switch (i) {
			case 0: // CONSTANTS
				return 0;
				break;
			case 1: // IMPORTS
				return 0 + SPACE_SIZE_CONSTANTS_COUNT;
				break;
			case 2: // COMMANDS
				return 0 + SPACE_SIZE_CONSTANTS_COUNT + SPACE_SIZE_IMPORTS_COUNT;
				break;
			case 3: // QUEUES
				return 0 + SPACE_SIZE_CONSTANTS_COUNT + SPACE_SIZE_IMPORTS_COUNT + SPACE_SIZE_COMMANDS_COUNT;
				break;
			case 4: // WHOLE
				return 0 + SPACE_SIZE_CONSTANTS_COUNT + SPACE_SIZE_IMPORTS_COUNT + SPACE_SIZE_COMMANDS_COUNT + SPACE_SIZE_QUEUES_COUNT;
				break;
			default:
				printf("ERROR IMPOSSIBLE INDEX!");
				return 0;
		}
	}

	getter uint8 GetDirectoryOffset () {
		return SPACE_SIZE_DIRECTORY_LENGTH + directoryPath;
	}

	getter uint8 GetHeaderOffset () {
		return SPACE_SIZE_FILES_COUNT + GetDirectoryOffset () + GetHeaderIndex (4);
	}

	// Project File
	// 1. read config file.
	// 2. open project file based on config file data.
	// 3. read project file by overwritting config file fully
	
	// STRUCTURE AS OF RIGHT NOW
	// { IMPORT_COUNT = 1 }
	// IMPORT_1 { 
	//		{ COUNTS }
	//		{ CONSTANTS SIZES & DATA }
	//		{ IMPORTS SIZES & DATA }
	//		{ COMMANDS SIZES & DATA }
	//		{ QUEUES SIZES & DATA }
	// }
	
	// 4. read imports.
	
	// STRUCTURE AS OF RIGHT NOW
	// { IMPORT_COUNT = 2 }
	// IMPORT_1 { 
	//		{ COUNTS }
	//		{ CONSTANTS SIZES & DATA }
	//		{ IMPORTS SIZES & DATA }
	//		{ COMMANDS SIZES & DATA }
	//		{ QUEUES SIZES & DATA }
	// }
	// IMPORT_2 { 
	//		{ COUNTS }
	//		{ CONSTANTS SIZES & DATA }
	//		{ IMPORTS SIZES & DATA }
	//		{ COMMANDS SIZES & DATA }
	//		{ QUEUES SIZES & DATA }
	// }

}