#pragma once
#include "lib/Framework.hpp"

namespace Commands::Open {

	// Space in bytes.
	const uint8 SPACE_SIZE_FILES_COUNT = 1;
	const uint8 SPACE_SIZE_CONSTANTS_COUNT = 1;
	const uint8 SPACE_SIZE_IMPORTS_COUNT = 1;
	const uint8 SPACE_SIZE_COMMANDS_COUNT = 1;
	const uint8 SPACE_SIZE_QUEUES_COUNT = 1;

	const uint8 SPACE_SIZE_COUNTS_OFFSET = 
		SPACE_SIZE_FILES_COUNT +
		SPACE_SIZE_CONSTANTS_COUNT + 
		SPACE_SIZE_IMPORTS_COUNT;

	const uint8 SPACE_SIZE_CONTEXT = 1;
	const uint8 SPACE_SIZE_NAME = 1;

	// AT WHAT INDECIES IS WHAT VALUE
	const size INDEX_FILES_COUNT = 0;
	const size INDEX_INITIAL_CONSTANTS_COUNT = 1;
	const size INDEX_INITIAL_IMPORTS_COUNT = 2;
	const size INDEX_OFFSET = INDEX_INITIAL_IMPORTS_COUNT;

	// running from config file
	// ! we don't access config import file: constants
	// - we access all constants, imports, commands, queues

	// running from project file
	// - we access config file: constants
	// - we access project import file: constants
	// - we access project import file import file: constants
	// - we access project file: constants, commands, queues

	// Project File
	// 1. read config file.
	// 2. open project file based on config file data.
	// 3. read project file by overwritting config file commands and queues.
	
	// STRUCTURE AS OF RIGHT NOW
	// { IMPORT_COUNT = 1 }
	// IMPORT_0 { 
	//		{ COUNTS }
	//		{ CONSTANTS SIZES & DATA }
	// }
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
	// IMPORT_0 { 
	//		{ COUNTS }
	//		{ CONSTANTS SIZES & DATA }
	// }
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
	
	// Maybe this could work...
	// Everything works on LOOK_UP_STARTS array...
	// Maybe i can point on CONSTANTS, IMPORTS, COMMANDS, QUEUES from other inports with ease ?
	// This would also mean that order wouldn't matter.

}