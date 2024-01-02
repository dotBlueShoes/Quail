#pragma once
#include "lib/Framework.hpp"

#include "Buffors.hpp"
#include "Types.hpp"

namespace IO::Projects::Parse {

	block DisplayFiles() {

		//const array<char, 19> LABEL { "\nProject Filepaths\n" };

		const uint8 SPACE_SIZE_FILES_COUNT = 1;
		const uint8 SPACE_SIZE_CONTEXT = 1;
		const uint8 SPACE_SIZE_NAME = 1;


		const uint8& filesCount = memoryBlockA.data[0];
		

		uint8 nextIndex = SPACE_SIZE_FILES_COUNT;
		uint8 nameCount, contextCount;


		// DISPLAY LABEL
		//fwrite(LABEL.Pointer(), sizeof(char), LABEL.Length(), stdout);
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