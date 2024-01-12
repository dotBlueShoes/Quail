#pragma once
#include "lib/Framework.hpp"

#include "../Buffor.hpp"
#include "../Parse.hpp"

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

		const uint16& constantsCount = memoryBlockA.data[INDEX_INITIAL_CONSTANTS_COUNT];
		const uint8& filesCount = memoryBlockA.data[INDEX_INITIAL_IMPORTS_COUNT];
		uint8 nextIndex = SPACE_SIZE_COUNTS_OFFSET;
		uint8 nameCount, rawContextCount;

		// PREPERE LOOK_UP_POSITIONS FOR CONSTANTS
		auto&& constantsStartPositions = memoryBlockC.data;

		// Skip constants in buffor to point at imports instead and create LOOK_UP_POSITIONS.
		for (uint8 i = 0; i < constantsCount; ++i) {
			nameCount = memoryBlockA.data[nextIndex];
			nextIndex += SPACE_SIZE_NAME;
			rawContextCount = memoryBlockA.data[nextIndex];
			nextIndex += SPACE_SIZE_CONTEXT;

			// ASSIGN START POSITION
			constantsStartPositions[i] = nextIndex;
			// SAVE nameCount FOR LATER USE
			constantsStartPositions[constantsCount + i] = nameCount;
			// SAVE contextCount FOR LATER USE
			constantsStartPositions[(constantsCount * 2) + i] = rawContextCount;

			nextIndex += nameCount + rawContextCount;
		}


		// PREPERE LOOK_UP_POSITIONS FOR INCLUDES
		auto&& includesStartPositions = memoryBlockB.data;

		for (uint8 i = 0; i < filesCount; ++i) {

			// GET PROJECT_NAME
			nameCount = memoryBlockA.data[nextIndex];
			nextIndex += SPACE_SIZE_NAME;

			// GET PROJECT_PATH
			rawContextCount = memoryBlockA.data[nextIndex];
			nextIndex += SPACE_SIZE_CONTEXT;

			// ASSIGN START POSITION
			includesStartPositions[i] = nextIndex;
			// SAVE nameCount FOR LATER USE
			includesStartPositions[filesCount + i] = nameCount;
			// SAVE rawContextCount FOR LATER USE
			includesStartPositions[(filesCount * 2) + i] = rawContextCount;

			// MOVE TO THE BEGINING OF NEXT PROJECT
			nextIndex += nameCount + rawContextCount;

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
			memoryBlockA.data, filesCount, includesStartPositions
		);

		// Get RAW string
		auto&& rawContext = memoryBlockA.data + includesStartPositions[resultIndex] + includesStartPositions[filesCount + resultIndex];
		rawContextCount = includesStartPositions[(2 * filesCount) + resultIndex];
		
		auto&& context = memoryBlockB.data;
		uint16 contextCount (0);

		//fwrite(rawContext, sizeof(char), rawContextCount, stdout);

		Parse::ConstructConstants(
			rawContextCount, rawContext, 
			constantsCount, constantsStartPositions, 
			contextCount, context
		);

		//fwrite(context, sizeof(char), contextCount, stdout);

		// RETURN RESULT IN CORRECT FORMAT
		std::mbstowcs(filePath, (char *)context, contextCount);
		
	}


}