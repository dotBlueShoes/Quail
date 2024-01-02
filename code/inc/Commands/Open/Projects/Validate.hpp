#pragma once
#include "lib/Framework.hpp"

#include "../Buffor.hpp"

#include <cstdlib>

namespace Commands::Open::Projects {

	// IN BUFFOR
	// READ FROM IT:
	// - PROJECTS COUNT
	// - PROJECT LENGTH
	// - PROJECT NAME
	// RETURN ASSOCIATED WITH IT PATH

	block Validate (
		OUT						charFilePath* filePath, 
		IN						const uint8& projectLength,
		INREADS (projectLength)	const charFile* const projectName
	) {

		const uint8& filesCount = memoryBlockA.data[INDEX_FILES_COUNT];
		uint8 nextIndex = SPACE_SIZE_FILES_COUNT;
		uint8 nameCount, contextCount;

		// PREPERE WHERE TO LOOK FOR POSITIONS
		
		auto&& startPositions = memoryBlockB.data;

		for (uint8 i = 0; i < filesCount; ++i) {

			// GET PROJECT_NAME
			nameCount = memoryBlockA.data[nextIndex];
			nextIndex += SPACE_SIZE_NAME;

			// GET PROJECT_PATH
			contextCount = memoryBlockA.data[nextIndex];
			nextIndex += SPACE_SIZE_CONTEXT;

			// ASSIGN START POSITION
			startPositions[i] = nextIndex;
			// SAVE nameCount FOR LATER USE
			startPositions[filesCount + i] = nameCount;
			// SAVE contextCount FOR LATER USE
			startPositions[(filesCount * 2) + i] = contextCount;

			// MOVE TO THE BEGINING OF NEXT PROJECT
			nextIndex += nameCount + contextCount;

		}

		// MATCH

		auto onNoMatchFound = []() { 
			printf ("\n%s\n\n", Search::Array::STRING_SEARCH_BYPFME_ERROR);
			exit (ExitCode::FAILURE_INVALID_ARGUMENT);
		};

		uint16 resultIndex (0);

		Search::Buffor::ByPFM<charFile, uint16> (
			onNoMatchFound, resultIndex,
			projectLength, projectName,
			memoryBlockA.data, filesCount, startPositions
		);

		// RETURN RESULT IN CORRECT FORMAT

		

		char* begin = (char *)memoryBlockA.data + startPositions[resultIndex] + startPositions[filesCount + resultIndex];
		const size length = startPositions[(2 *filesCount) + resultIndex];

		// null-terminate the end
		//begin[length] = '\0';

		fwrite(begin, sizeof(char), length, stdout);
		//printf("c:%c\n", begin[0]);
		//printf("c:%llu\n", length);

		std::mbstowcs(filePath, begin, length);

		//wprintf(L"\n%ls\n", filePath);

		//filePath = L"0";
		
	}


}