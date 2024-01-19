#pragma once
#include "lib/Framework.hpp"

#include "Buffors.hpp"

namespace Commands::Open {

	// Limits
	const uint8 CONSTANTS_LIMIT = 16;
	const uint16 SPACE_SIZE_CONSTATNS_INDEX = 4;

	// Space in bytes.
	const uint8 SPACE_SIZE_FILES_COUNT = 1;
	const uint8 SPACE_SIZE_CONSTANTS_COUNT = 1;
	const uint8 SPACE_SIZE_IMPORTS_COUNT = 1;
	const uint8 SPACE_SIZE_COMMANDS_COUNT = 1;
	const uint8 SPACE_SIZE_QUEUES_COUNT = 1;

	getter constexpr uint8 GetHeaderIndex(const uint8& i) {
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

	const size INDEX_FILES_COUNT	= SPACE_SIZE_FILES_COUNT - 1;
	const size INDEX_OFFSET			= SPACE_SIZE_FILES_COUNT - 1 + GetHeaderIndex(4);

	// OTHER

	const uint8 SPACE_SIZE_CONTEXT = 1;
	const uint8 SPACE_SIZE_NAME = 1;

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