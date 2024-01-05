#pragma once
#include "lib/Framework.hpp"

#include "Buffors.hpp"
#include "Types.hpp"
#include "Buffor.hpp"

namespace Commands::Open::Parse {


		block ConstructConstants (
			IN							const uint16& rawContextCount, 
			INREADS (rawContextCount)	const unsigned char* rawContext,
			IN							const uint16& constantsCount,
			IN							const uint8* startPositions,
			OUT 						uint16& contextCount,
			OUT							uint8* context
		) {
			uint16 contextIndex = 0, constantIndex, constantLength;

			contextCount = 0;
			goto copy;
				
constant:	for (; contextIndex < rawContextCount; ++contextIndex) {
				auto&& current = rawContext[contextIndex];
				if (current == Stages::REFERENCE) {
					constantLength = contextIndex - constantIndex;
					++contextIndex; // We have to do it manually here.
					goto replace;
				};
			}

			// THAT DETECTION SHOULD BE HAPPENING BEFORE THO !
			// Closing REFERENCE was abscent!
			printf("%s%s", "ERROR: ", "FILE_IS_ILL-FORMED!\n");
			exit(1);

replace:	{ 
				auto&& destination = context + contextCount;
				auto&& constantName = (char *)rawContext + constantIndex;

				// MATCH

				auto onNoMatchFound = []() { 
					printf ("\n%s\n\n", Search::Array::STRING_SEARCH_BYPFME_ERROR);
					exit (ExitCode::FAILURE_INVALID_ARGUMENT);
				};

				uint16 resultIndex (0);

				Search::Buffor::ByPFM<charFile, uint16> (
					onNoMatchFound, resultIndex,
					constantLength, constantName,
					memoryBlockA.data, constantsCount, startPositions
				);

				char* begin = (char *)memoryBlockA.data + startPositions[resultIndex] + startPositions[constantsCount + resultIndex];
				const size length = startPositions[(2 * constantsCount) + resultIndex];
					
				// COPY
				std::memcpy(destination, begin, length * sizeof(char));
				contextCount += length;	

			}

copy:		for (; contextIndex < rawContextCount; ++contextIndex, ++contextCount) {
				auto&& current = rawContext[contextIndex];
					
				if (current == Stages::REFERENCE) { 
					++contextIndex; // We have to do it manually here.
					constantIndex = contextIndex;
					goto constant; 
				};

				context[contextCount] = current;
			}
		}


	block DisplayFiles() {

		const uint16& constantsCount = memoryBlockA.data[INDEX_CONSTANTS_COUNT];
		const uint16& filesCount = memoryBlockA.data[INDEX_FILES_COUNT];

		size nextIndex = SPACE_SIZE_COUNTS_OFFSET;
		uint16 nameCount, rawContextCount, contextCount;

		auto&& startPositions = memoryBlockC.data;

		// Skip constants in buffor to point at imports instead and create LOOK_UP_POSITIONS.
		for (uint8 i = 0; i < constantsCount; ++i) {
			nameCount = memoryBlockA.data[nextIndex];
			nextIndex += SPACE_SIZE_NAME;
			rawContextCount = memoryBlockA.data[nextIndex];
			nextIndex += SPACE_SIZE_CONTEXT;

			// ASSIGN START POSITION
			startPositions[i] = nextIndex;
			// SAVE nameCount FOR LATER USE
			startPositions[constantsCount + i] = nameCount;
			// SAVE contextCount FOR LATER USE
			startPositions[(constantsCount * 2) + i] = rawContextCount;

			nextIndex += nameCount + rawContextCount;
		}

		// DISPLAY
		fputc('\n', stdout);

		for (uint16 i = 0; i < filesCount; ++i) {

			// GET PROJECT_NAME
			nameCount = memoryBlockA.data[nextIndex];
			nextIndex += SPACE_SIZE_NAME;

			// GET PROJECT_PATH
			rawContextCount = memoryBlockA.data[nextIndex];
			nextIndex += SPACE_SIZE_CONTEXT;

			auto&& rawName = memoryBlockA.data + nextIndex;
			auto&& rawContext = memoryBlockA.data + nextIndex + nameCount;

			uint16 nameIndex = 0;
			for (; nameIndex < nameCount; ++nameIndex) {
				memoryBlockB.data[nameIndex] = rawName[nameIndex];
			}

			ConstructConstants(
				rawContextCount, rawContext, 
				constantsCount, startPositions, 
				contextCount, memoryBlockB.data + nameIndex
			);

			auto&& name = memoryBlockB.data;
			auto&& context = memoryBlockB.data + nameCount;

			// DISPLAY FILES
			fwrite(" ", sizeof(char), 2, stdout);
			fwrite(name, sizeof(char), nameCount, stdout);
			fwrite("\t: ", sizeof(char), 3, stdout);
			fwrite(context, sizeof(char), contextCount, stdout);
			fwrite("\n", sizeof(char), 2, stdout);

			// MOVE TO THE BEGINING OF NEXT PROJECT
			nextIndex += nameCount + rawContextCount;

		}

		fputc('\n', stdout);
		
	}

}