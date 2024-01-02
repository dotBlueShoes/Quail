#pragma once
#include "lib/Framework.hpp"

#include "Buffors.hpp"
#include "Types.hpp"
#include "Buffor.hpp"

namespace Commands::Open::Parse {

	block DisplayFiles() {

		const uint8& filesCount = memoryBlockA.data[INDEX_FILES_COUNT];

		uint8 nextIndex = SPACE_SIZE_FILES_COUNT;
		uint8 nameCount, contextCount;

		// DISPLAY
		fputc('\n', stdout);

		for (uint8 i = 0; i < filesCount; ++i) {

			// GET PROJECT_NAME
			nameCount = memoryBlockA.data[nextIndex];
			nextIndex += SPACE_SIZE_NAME;

			// GET PROJECT_PATH
			contextCount = memoryBlockA.data[nextIndex];
			nextIndex += SPACE_SIZE_CONTEXT;

			// DISPLAY FILES
			fwrite(" ", sizeof(char), 2, stdout);
			fwrite(memoryBlockA.data + nextIndex, sizeof(char), nameCount, stdout);
			fwrite("\t: ", sizeof(char), 3, stdout);
			fwrite(memoryBlockA.data + nextIndex + nameCount, sizeof(char), contextCount, stdout);
			fwrite("\n", sizeof(char), 2, stdout);

			// MOVE TO THE BEGINING OF NEXT PROJECT
			nextIndex += nameCount + contextCount;

		}

		fputc('\n', stdout);
		
	}

}