#pragma once
#include "lib/Framework.hpp"

#include "Buffors.hpp"
#include "Types.hpp"
#include "Buffor.hpp"

namespace Commands::Open::Parse {


	using StrType = array<const charFile, 2, uint8>;
	const StrType STR_TYPE_CONSTANT	= { " %" };
	const StrType STR_TYPE_IMPORT 	= { " !" };
	const StrType STR_TYPE_COMMAND	= { " >" };
	const StrType STR_TYPE_PIPE 	= { " |" };
	

	const array<const char, 16> PADDING { 
		' ', ' ', ' ', ' ', 
		' ', ' ', ' ', ' ', 
		' ', ' ', ' ', ' ', 
		' ', ' ', ' ', ' '
	};


	block Write (
		IN  					const StrType&		strType,
		IN 						const uint8& 		paddingLength,
		IN						const uint8& 		nameCount,
		INREADS	(nameCount)		const byte* const 	name,
		IN						const uint8& 		contextCount,
		INREADS	(contextCount)	const byte* const 	context
	) {
		fwrite(strType.Pointer(), sizeof(char), strType.Length(), stdout);
		fwrite(name, sizeof(char), nameCount, stdout);
		fwrite(PADDING.Pointer(), sizeof(char), paddingLength, stdout);
		fwrite(": ", sizeof(char), 2, stdout);
		fwrite(context, sizeof(char), contextCount, stdout);
		fwrite("\n", sizeof(char), 2, stdout);
	}


		block ConstructConstants (
			IN							const uint16& 				rawContextCount, 
			INREADS (rawContextCount)	const unsigned char* const	rawContext,
			IN							const uint16& 				constantsCount,
			IN							const uint8* const			startPositions,
			OUT 						uint16&						contextCount,
			OUT							uint8* const				context
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
					memoryBlockA.data, constantsCount, startPositions,
					SPACE_SIZE_CONSTATNS_INDEX
				);

				//printf("\nr:%i\n", resultIndex);

				// We need to get index stored in bytes.
				//const uint32 buf = ((uint64*)(startPositions + (resultIndex * SPACE_SIZE_CONSTATNS_INDEX)))[0];
				uint32 index = startPositions[resultIndex * SPACE_SIZE_CONSTATNS_INDEX + 0];
				index <<= 8;
				index += startPositions[resultIndex * SPACE_SIZE_CONSTATNS_INDEX + 1];
				index <<= 8;
				index += startPositions[resultIndex * SPACE_SIZE_CONSTATNS_INDEX + 2];
				index <<= 8;
				index += startPositions[resultIndex * SPACE_SIZE_CONSTATNS_INDEX + 3];

				//printf("\nh:%i\n", index);

				// To get context begin we get name position + it's length so we endup at context start.
				char* begin = (char *)memoryBlockA.data + 
					index +
					startPositions[(CONSTANTS_LIMIT * 4) + resultIndex];

				const size length = 
					startPositions[(CONSTANTS_LIMIT * 5) + resultIndex];

				//fwrite(begin, sizeof(charFile), length, stdout);
					
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

			//printf("\nend!\n");
		}


	block ReadThrough (
		OUT	size& 			nextIndex,
		IN	const uint8& 	valuesCount
	) {
		for (uint8 i = 0; i < valuesCount; ++i) {

			const uint8& nameCount = memoryBlockA.data[nextIndex];
			nextIndex += SPACE_SIZE_NAME;

			const uint8& rawContextCount = memoryBlockA.data[nextIndex];
			nextIndex += SPACE_SIZE_CONTEXT;

			nextIndex += nameCount + rawContextCount;
		}
	}


	block ReadThroughSave (
		OUT	size& 			nextIndex,
		OUT	uint8* const	values,
		IN	const uint8& 	valuesCount,
		IN	const size&		offset = 0
	) {

		for (uint8 i = 0; i < valuesCount; ++i) {

			const uint8& nameCount = memoryBlockA.data[nextIndex];
			nextIndex += SPACE_SIZE_NAME;

			const uint8& rawContextCount = memoryBlockA.data[nextIndex];
			nextIndex += SPACE_SIZE_CONTEXT;

			// ASSIGN START POSITION
			values[((i + offset) * SPACE_SIZE_CONSTATNS_INDEX) + 0] = nextIndex >> 24;
			values[((i + offset) * SPACE_SIZE_CONSTATNS_INDEX) + 1] = nextIndex >> 16;
			values[((i + offset) * SPACE_SIZE_CONSTATNS_INDEX) + 2] = nextIndex >>  8;
			values[((i + offset) * SPACE_SIZE_CONSTATNS_INDEX) + 3] = nextIndex >>  0;
			
			// SAVE 'nameCount' FOR LATER USE
			values[(CONSTANTS_LIMIT * SPACE_SIZE_CONSTATNS_INDEX) + (i + offset)] = nameCount;
			// SAVE 'contextCount' FOR LATER USE
			values[(CONSTANTS_LIMIT * (SPACE_SIZE_CONSTATNS_INDEX + 1)) + (i + offset)] = rawContextCount;

			nextIndex += nameCount + rawContextCount;
		}
	}


	block ReadThroughWrite (
		OUT	size& 			nextIndex,
		IN  const StrType&	strType,
		IN	const uint8& 	valuesCount
	) {
		for (uint16 i = 0; i < valuesCount; ++i) {
			
			const uint8& nameCount = memoryBlockA.data[nextIndex];
			nextIndex += SPACE_SIZE_NAME;
			const uint8& rawContextCount = memoryBlockA.data[nextIndex];
			nextIndex += SPACE_SIZE_CONTEXT;

			const uint8 paddingLength = (PADDING.Length() - nameCount) * (nameCount <= PADDING.Length());

			auto&& name = memoryBlockA.data + nextIndex;
			auto&& context = memoryBlockA.data + nextIndex + nameCount;

			Write (strType, paddingLength, nameCount, name, rawContextCount, context);

			nextIndex += nameCount + rawContextCount;
		}
	}


	block ReadThroughConstructWrite (
		OUT							size& 				nextIndex,
		IN  						const StrType&		strType,
		IN							const uint8& 		valuesCount,
		IN							const uint8& 		constantsCount,
		IN							const uint8* const 	constants
	) {
		uint16 contextCount = 0;

		for (uint16 i = 0; i < valuesCount; ++i) {
			
			const uint8& nameCount = memoryBlockA.data[nextIndex];
			nextIndex += SPACE_SIZE_NAME;
			const uint8& rawContextCount = memoryBlockA.data[nextIndex];
			nextIndex += SPACE_SIZE_CONTEXT;

			const uint8 paddingLength = (PADDING.Length() - nameCount) * (nameCount <= PADDING.Length());

			auto&& rawName = memoryBlockA.data + nextIndex;
			auto&& rawContext = memoryBlockA.data + nextIndex + nameCount;

			uint16 nameIndex = 0;
			for (; nameIndex < nameCount; ++nameIndex) {
				memoryBlockB.data[nameIndex] = rawName[nameIndex];
			}

			ConstructConstants(
				rawContextCount, rawContext, 
				constantsCount, constants, 
				contextCount, memoryBlockB.data + nameIndex
			);

			auto&& name = memoryBlockB.data;
			auto&& context = memoryBlockB.data + nameCount;

			Write (strType, paddingLength, nameCount, name, contextCount, context);

			nextIndex += nameCount + rawContextCount;
		}
	}

	// ReadThroughCommandMatchConstructExecute()
	// ReadThroughQueueMatchConstructExecute()
































	block DisplayFiles () {

		const uint8& constantsCount	= memoryBlockA.data[SPACE_SIZE_FILES_COUNT + GetHeaderIndex(0)];
		const uint8& importsCount	= memoryBlockA.data[SPACE_SIZE_FILES_COUNT + GetHeaderIndex(1)];
		const uint8& commandsCount	= memoryBlockA.data[SPACE_SIZE_FILES_COUNT + GetHeaderIndex(2)];
		const uint8& queuesCount	= memoryBlockA.data[SPACE_SIZE_FILES_COUNT + GetHeaderIndex(3)];

		size nextIndex = SPACE_SIZE_FILES_COUNT + GetHeaderIndex(4);

		auto&& constants = memoryBlockC.data;

		// DISPLAY IMPORTS
		fputc('\n', stdout);

		ReadThroughSave (nextIndex, constants, constantsCount);
		ReadThroughConstructWrite (nextIndex, STR_TYPE_IMPORT, importsCount, constantsCount, constants);
		ReadThroughWrite (nextIndex, STR_TYPE_COMMAND, commandsCount);
		ReadThroughWrite (nextIndex, STR_TYPE_PIPE, queuesCount);

		fputc('\n', stdout);
		
	}


	block DisplayProject () {

		const uint8& filesCount = memoryBlockA.data[INDEX_FILES_COUNT];
		uint8*& constants = memoryBlockC.data;

		size nextIndex = SPACE_SIZE_FILES_COUNT;
		size allConstantsCount = 0;

		// Read all to get constants.
		for (uint8 j = 0; j < filesCount + 1; ++j) {

			const uint8& constantsCount = memoryBlockA.data[nextIndex + GetHeaderIndex(0)];
			const uint8& importsCount 	= memoryBlockA.data[nextIndex + GetHeaderIndex(1)];
			const uint8& commandsCount 	= memoryBlockA.data[nextIndex + GetHeaderIndex(2)];
			const uint8& queuesCount 	= memoryBlockA.data[nextIndex + GetHeaderIndex(3)];

			nextIndex += INDEX_OFFSET;

			ReadThroughSave (nextIndex, constants, constantsCount, allConstantsCount);
			ReadThrough (nextIndex, importsCount);
			ReadThrough (nextIndex, commandsCount);
			ReadThrough (nextIndex, queuesCount);

			allConstantsCount += constantsCount;
		}

		// Reset buffor pointer.
		nextIndex = SPACE_SIZE_FILES_COUNT;


		fputc('\n', stdout);

		// Read again to write and apply constants.
		for (uint8 j = 0; j < filesCount + 1; ++j) {
			const uint8& constantsCount = memoryBlockA.data[nextIndex + GetHeaderIndex(0)];
			const uint8& importsCount 	= memoryBlockA.data[nextIndex + GetHeaderIndex(1)];
			const uint8& commandsCount 	= memoryBlockA.data[nextIndex + GetHeaderIndex(2)];
			const uint8& queuesCount 	= memoryBlockA.data[nextIndex + GetHeaderIndex(3)];
		
			nextIndex += INDEX_OFFSET;
		
			ReadThrough (nextIndex, constantsCount);
			ReadThrough (nextIndex, importsCount);
			ReadThroughConstructWrite (nextIndex, STR_TYPE_COMMAND, commandsCount, allConstantsCount, constants);
			ReadThroughWrite (nextIndex, STR_TYPE_PIPE, queuesCount);
		}

		fputc('\n', stdout);
		
	}

	// 2 fors, 
	//  1 - read through and get all constants from imports
	//  2 - write all commands and queues

	block ExecuteSubcommand (
		IN 						const uint8& 				subcmmdLength, 
		INREADS (projectLength) const charConsole* const 	subcmmdName
	) {

	}
	// 2 fors
	//  1 - read through and get all constants from imports
	//  2 - instead of ReadThrough(Write) do ReadThrough(MatchAndExecute)

}


		// TODO	

		// 1. Proper constatns startingPoint list (get all from whole buffor then after 1readthrough apply them and display them)
		// 2. Raw to Proccessed commands (talking about consts) not only imports
		// 3. Proper including ... 
		// 4. Execute commands
		// 5. Execute queues

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

/* IMPORT AND CONSTRUCT 
// DISPLAY IMPORTS
			//fwrite("\nIMPORTS\n", sizeof(char), 9, stdout);

			for (uint16 i = 0; i < importsCount; ++i) {

				// GET PROJECT_NAME
				nameCount = memoryBlockA.data[nextIndex];
				nextIndex += SPACE_SIZE_NAME;

				// GET PROJECT_PATH
				rawContextCount = memoryBlockA.data[nextIndex];
				nextIndex += SPACE_SIZE_CONTEXT;

				//auto&& rawName = memoryBlockA.data + nextIndex;
				//auto&& rawContext = memoryBlockA.data + nextIndex + nameCount;

				//uint16 nameIndex = 0;
				//for (; nameIndex < nameCount; ++nameIndex) {
				//	memoryBlockB.data[nameIndex] = rawName[nameIndex];
				//}

				//ConstructConstants(
				//	rawContextCount, rawContext, 
				//	constantsCount, constants, 
				//	contextCount, memoryBlockB.data + nameIndex
				//);

				//auto&& name = memoryBlockB.data;
				//auto&& context = memoryBlockB.data + nameCount;

				// DISPLAY FILES
				//fwrite(" ", sizeof(char), 2, stdout);
				//fwrite(name, sizeof(char), nameCount, stdout);

				//const uint8 paddingLength = (PADDING.Length() - nameCount) * (nameCount <= PADDING.Length());
				//fwrite(PADDING.Pointer(), sizeof(char), paddingLength, stdout);

				//fwrite(": ", sizeof(char), 2, stdout);
				//fwrite(context, sizeof(char), contextCount, stdout);
				//fwrite("\n", sizeof(char), 2, stdout);

				// MOVE TO THE BEGINING OF NEXT IMPORT
				nextIndex += nameCount + rawContextCount;

			}

			// DISPLAY COMMANDS
			//fputc('\n', stdout);
			//fwrite("\nCOMMANDS\n", sizeof(char), 10, stdout);
*/