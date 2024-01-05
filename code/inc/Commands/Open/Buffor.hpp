#pragma once
#include "lib/Framework.hpp"

namespace Commands::Open {

	// Space in bytes.
	const uint8 SPACE_SIZE_CONSTANTS_COUNT = 1;
	const uint8 SPACE_SIZE_FILES_COUNT = 1;
	const uint8 SPACE_SIZE_COUNTS_OFFSET = SPACE_SIZE_CONSTANTS_COUNT + SPACE_SIZE_FILES_COUNT;

	const uint8 SPACE_SIZE_CONTEXT = 1;
	const uint8 SPACE_SIZE_NAME = 1;

	// AT WHAT INDECIES IS WHAT VALUE
	const size INDEX_CONSTANTS_COUNT = 0;
	const size INDEX_FILES_COUNT = 1;
	const size INDEX_OFFSET = INDEX_FILES_COUNT;

}