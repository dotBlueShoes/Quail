#pragma once
#include "lib/Framework.hpp"

#include "Buffors.hpp"
#include "Types.hpp"
#include "Buffor.hpp"

namespace Commands::Open::Parse {

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

			{ // CONSTRUCT CONSTANTS

				uint16 nameIndex = 0;
				for (; nameIndex < nameCount; ++nameIndex) {
					memoryBlockB.data[nameIndex] = rawName[nameIndex];
				}
				

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
					auto&& destination = memoryBlockB.data + nameIndex + contextCount;
					auto&& constantName = (char *)rawContext + constantIndex;
					//printf(":%i\n", constantLength);

					//fwrite(constantName, sizeof(char), constantLength, stdout);

					//size nextConstantIndex = SPACE_SIZE_COUNTS_OFFSET;
					//uint16 constantNameCount, constantContextCount;
					//
					//constantNameCount = memoryBlockA.data[nextConstantIndex];
					//nextConstantIndex += SPACE_SIZE_NAME;
					//constantContextCount = memoryBlockA.data[nextConstantIndex];
					//nextConstantIndex += SPACE_SIZE_CONTEXT;
					//
					//auto&& constantContext = memoryBlockA.data + nextConstantIndex + constantNameCount;

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

					//fwrite(begin, sizeof(char), length, stdout);
					//printf(":%i\n", resultIndex);
					
					// COPY
					std::memcpy(destination, begin, length * sizeof(char));
					//for (uint16 k = 0; k < constantContextCount; ++k) {
					//	memoryBlockB.data[nameIndex + contextCount + k] = constantContext[k];
					//}
					contextCount += length;
					

				}

	copy:		for (; contextIndex < rawContextCount; ++contextIndex, ++contextCount) {
					auto&& current = rawContext[contextIndex];
					
					if (current == Stages::REFERENCE) { 
						++contextIndex; // We have to do it manually here.
						constantIndex = contextIndex;
						goto constant; 
					};

					memoryBlockB.data[nameIndex + contextCount] = current;
				}

				// FOR NOW ONLY!!!
				//++contextCount;
				//memoryBlockB.data[nameIndex + contextCount] = '\0';


				// 1. inside name string look for REFERENCE sign.
				// 2. read till we find another REFERENCE sign.
				// if found
				// 	3. look for that string in constants section inside buffor using ByPFM.
				//  if match
				//   4. copy that part to memoryBlockB buffor then copy the rest if still needded
				//  else
				//	 4. ill-formed exit(1); 
				// else
				// 	3. ill-formed exit(1);

			}

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