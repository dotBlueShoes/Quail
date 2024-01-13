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

		const uint16& constantsCount	 = memoryBlockA.data[INDEX_INITIAL_CONSTANTS_COUNT];
		const uint16& importsCount		 = memoryBlockA.data[INDEX_INITIAL_IMPORTS_COUNT];
		const uint16& commandsCount		 = memoryBlockA.data[INDEX_INITIAL_COMMANDS_COUNT];
		const uint16& queuesCount		 = memoryBlockA.data[INDEX_INITIAL_QUEUES_COUNT];

		size nextIndex = SPACE_SIZE_FILES_COUNT + SPACE_SIZE_FILE_OFFSET;
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

		// DISPLAY IMPORTS
		fputc('\n', stdout);

		for (uint16 i = 0; i < importsCount; ++i) {

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

		// DISPLAY COMMANDS
		fputc('\n', stdout);

		for (uint16 i = 0; i < commandsCount; ++i) {
			nameCount = memoryBlockA.data[nextIndex];
			nextIndex += SPACE_SIZE_NAME;
			rawContextCount = memoryBlockA.data[nextIndex];
			nextIndex += SPACE_SIZE_CONTEXT;

			auto&& rawName = memoryBlockA.data + nextIndex;
			auto&& rawContext = memoryBlockA.data + nextIndex + nameCount;

			fwrite(" ", sizeof(char), 2, stdout);
			fwrite(rawName, sizeof(char), nameCount, stdout);
			fwrite("\t: ", sizeof(char), 3, stdout);
			fwrite(rawContext, sizeof(char), rawContextCount, stdout);
			fwrite("\n", sizeof(char), 2, stdout);

			nextIndex += nameCount + rawContextCount;
		}

		// DISPLAY QUEUES
		fputc('\n', stdout);

		for (uint16 i = 0; i < queuesCount; ++i) {
			nameCount = memoryBlockA.data[nextIndex];
			nextIndex += SPACE_SIZE_NAME;
			rawContextCount = memoryBlockA.data[nextIndex];
			nextIndex += SPACE_SIZE_CONTEXT;

			auto&& rawName = memoryBlockA.data + nextIndex;
			auto&& rawContext = memoryBlockA.data + nextIndex + nameCount;

			fwrite(" ", sizeof(char), 2, stdout);
			fwrite(rawName, sizeof(char), nameCount, stdout);
			fwrite("\t: ", sizeof(char), 3, stdout);
			fwrite(rawContext, sizeof(char), rawContextCount, stdout);
			fwrite("\n", sizeof(char), 2, stdout);

			nextIndex += nameCount + rawContextCount;
		}

		fputc('\n', stdout);
		
	}


	block DisplayProject() {

		// TODO
		// Refactor it so that 
		//  For each file we need to read constants and proccess raw imports into proper imports
		//  then we reed these imports as files so we end up in a loop ???

		// MAIN_CONFIG
		// -> PROJECT_CONFIG
		//	  -> LINKED_CONFIG_1
		//	  -> LINKED_CONFIG_2

		// We have 2 important methods (display & validate)

		// Validate runs for each new nested import
		//  so it runs once when we read from MAIN_CONFIG to PROJECT_CONFIG
		//  and it runs once when we read PROJECT_CONFIG to LINKED_CONFIG_1 + LINKED_CONFIG_2

		// Validate gives us paths that we feed back to read and place in buffor.
		//  Therefore YES it adds up but it's not an innfinite loop we just need to offset ourself and
		//  point at right values each import.

		// DisplayProject happends when the buffor is fully constructed.
		//  All project imports are imported and we're safe to display here commands, queues
		//  in contrast to DisplayFiles where we only display all defined there projects and their directiories!

		// FOCUS ON Validate method!
		// - diffrentiate between ValidateProject and ValidateImports
		// - ValidateImports would process on number of imports rather then just one. just so we dont offset so much.

		const uint16& filesCount = memoryBlockA.data[INDEX_FILES_COUNT];

		size nextIndex = 1;
		uint16 nameCount, rawContextCount, contextCount;

		auto&& startPositions = memoryBlockC.data;

		//fputc('\n', stdout);
		//printf(":%i\n", filesCount);
		

		for (uint8 j = 0; j < filesCount + 1; ++j) {

			const uint16& constantsCount 	= memoryBlockA.data[nextIndex + 0];
			const uint16& importsCount 		= memoryBlockA.data[nextIndex + 1];
			const uint16& commandsCount 	= memoryBlockA.data[nextIndex + 2];
			const uint16& queuesCount 		= memoryBlockA.data[nextIndex + 3];

			nextIndex += INDEX_OFFSET;

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

			// DISPLAY IMPORTS
			fwrite("\nIMPORTS\n", sizeof(char), 9, stdout);

			for (uint16 i = 0; i < importsCount; ++i) {

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

				// MOVE TO THE BEGINING OF NEXT IMPORT
				nextIndex += nameCount + rawContextCount;

			}

			// DISPLAY COMMANDS
			//fputc('\n', stdout);
			fwrite("\nCOMMANDS\n", sizeof(char), 10, stdout);

			for (uint16 i = 0; i < commandsCount; ++i) {
				nameCount = memoryBlockA.data[nextIndex];
				nextIndex += SPACE_SIZE_NAME;
				rawContextCount = memoryBlockA.data[nextIndex];
				nextIndex += SPACE_SIZE_CONTEXT;

				auto&& rawName = memoryBlockA.data + nextIndex;
				auto&& rawContext = memoryBlockA.data + nextIndex + nameCount;

				fwrite(" ", sizeof(char), 2, stdout);
				fwrite(rawName, sizeof(char), nameCount, stdout);
				fwrite("\t: ", sizeof(char), 3, stdout);
				fwrite(rawContext, sizeof(char), rawContextCount, stdout);
				fwrite("\n", sizeof(char), 2, stdout);

				nextIndex += nameCount + rawContextCount;
			}

			// DISPLAY QUEUES
			//fputc('\n', stdout);
			fwrite("\nQUEUES\n", sizeof(char), 8, stdout);

			for (uint16 i = 0; i < queuesCount; ++i) {
				nameCount = memoryBlockA.data[nextIndex];
				nextIndex += SPACE_SIZE_NAME;
				rawContextCount = memoryBlockA.data[nextIndex];
				nextIndex += SPACE_SIZE_CONTEXT;

				auto&& rawName = memoryBlockA.data + nextIndex;
				auto&& rawContext = memoryBlockA.data + nextIndex + nameCount;

				fwrite(" ", sizeof(char), 2, stdout);
				fwrite(rawName, sizeof(char), nameCount, stdout);
				fwrite("\t: ", sizeof(char), 3, stdout);
				fwrite(rawContext, sizeof(char), rawContextCount, stdout);
				fwrite("\n", sizeof(char), 2, stdout);

				nextIndex += nameCount + rawContextCount;
			}

		}

		fputc('\n', stdout);
		
	}

}