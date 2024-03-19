#pragma once
#include "lib/Framework.hpp"

#include "Buffors.hpp"
#include "Types.hpp"
#include "Buffor.hpp"

namespace Commands::Open::Parse {

	namespace ErrorMSG {
		const char INVALID_QUEUE_SUBCOMMAND[] = "Error: Invalid command in queue declaration!";
		const char INVALID_QUEUE_COMMAND[] = "Error: Command not matched!";
		const char INVALID_CONSTANT_REFERENCE[] = "Error: Invalid constant reference in command context!";
	}
	 


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


	block SaveNextIndex (
		OUT	uint8* const	values,
		IN	const size& 	nextIndex,
		IN	const size&		element,
		IN	const size&		offset
	) {
		values[((element + offset) * SPACE_SIZE_CONSTANTS_INDEX) + 0] = nextIndex >> 24;
		values[((element + offset) * SPACE_SIZE_CONSTANTS_INDEX) + 1] = nextIndex >> 16;
		values[((element + offset) * SPACE_SIZE_CONSTANTS_INDEX) + 2] = nextIndex >>  8;
		values[((element + offset) * SPACE_SIZE_CONSTANTS_INDEX) + 3] = nextIndex >>  0;
	}


	block LoadNextIndex (
		INOUT uint32& index,
		IN	const uint8* const values,
		IN	const uint16& element
	) {
		index += values[element * SPACE_SIZE_CONSTANTS_INDEX + 0];
		index <<= 8;
		index += values[element * SPACE_SIZE_CONSTANTS_INDEX + 1];
		index <<= 8;
		index += values[element * SPACE_SIZE_CONSTANTS_INDEX + 2];
		index <<= 8;
		index += values[element * SPACE_SIZE_CONSTANTS_INDEX + 3];
	}


	block Write (
		IN  					const StrType&		strType,
		IN 						const u16& 			consoleX,
		IN 						const uint8& 		paddingLength,
		IN						const uint8& 		nameCount,
		INREADS	(nameCount)		const byte* const 	name,
		IN						const uint8& 		contextCount,
		INREADS	(contextCount)	byte* 		context
	) {
		const uint16 maxConsoleX = consoleX - 4 - nameCount - paddingLength;

		uint16 condition = (3 + nameCount + paddingLength + contextCount) <= consoleX;
		uint16 trimmedContext = (condition * contextCount) + (!condition * maxConsoleX);

		// This makes it so next line when resized wont connect with this one.
		context[trimmedContext - 1] *= condition;

		// Thim makes a pretty "...".
		context[trimmedContext - 2] *= condition;
		context[trimmedContext - 2] += !condition * '.';
		context[trimmedContext - 3] *= condition;
		context[trimmedContext - 3] += !condition * '.';
		context[trimmedContext - 4] *= condition;
		context[trimmedContext - 4] += !condition * '.';

		fwrite (strType.Pointer(), sizeof (char), strType.Length (), stdout);
		fwrite (name, sizeof (char), nameCount, stdout);
		fwrite (PADDING.Pointer(), sizeof (char), paddingLength, stdout);
		fwrite (": ", sizeof (char), 2, stdout);
		fwrite (context, sizeof (char), trimmedContext, stdout);
		fwrite ("\n", sizeof (char), 1, stdout);
	}

	block WriteDirectory (
		IN							const uint8& 		filePathLength,
		INREADS	(filePathLength)	const uint8* const 	filePath
	) {
		const array<char, 3> dirLS { "\n [" };
		const array<char, 2> dirLE { "]\n" };

		fwrite (dirLS.Pointer(), sizeof (char), dirLS.Length(), stdout);
		fwrite (filePath, sizeof (wchar), filePathLength, stdout);
		fwrite (dirLE.Pointer(), sizeof (char), dirLE.Length(), stdout);
	}


		block ConstructConstants (
			// memoryBlockA.data
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
			printf (ErrorMSG::INVALID_CONSTANT_REFERENCE);
			exit (1);

replace:	{ 
				auto&& destination = context + contextCount;
				auto&& constantName = (char *)rawContext + constantIndex;

				// MATCH

				auto onNoMatchFound = [](uint16& resultIndex, const uint8 elementsCount) { 
					printf ("\n%s\n\n", Search::Array::STRING_SEARCH_BYPFME_ERROR);
					exit (ExitCode::FAILURE_INVALID_ARGUMENT);
				};

				uint16 resultIndex (0);

				Search::Buffor::ByPFM<charFile, uint16> (
					onNoMatchFound, resultIndex,
					constantLength, constantName,
					memoryBlockA.data, constantsCount, startPositions,
					SPACE_SIZE_CONSTANTS_INDEX
				);

				// We need to get index that is stored in bytes.
				uint32 index = 0;
				LoadNextIndex (index, startPositions, resultIndex);

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

			// SAVE START POSITIONS
			SaveNextIndex (values, nextIndex, i, offset);
			// SAVE 'nameCount' FOR LATER USE
			values[(CONSTANTS_LIMIT * SPACE_SIZE_CONSTANTS_INDEX) + (i + offset)] = nameCount;
			// SAVE 'contextCount' FOR LATER USE
			values[(CONSTANTS_LIMIT * (SPACE_SIZE_CONSTANTS_INDEX + 1)) + (i + offset)] = rawContextCount;

			nextIndex += nameCount + rawContextCount;
		}
	}


	block ReadThroughSaveEx (
		IN	uint8*& 		filesBuffor,
		OUT	size& 			nextIndex,
		IN	const uint8& 	valuesCount,
		OUT	uint8* const	values,
		IN	const size&		offset = 0
	) {
		for (uint8 i = 0; i < valuesCount; ++i) {

			const uint8& nameCount = filesBuffor[nextIndex];
			nextIndex += SPACE_SIZE_NAME;

			const uint8& rawContextCount = filesBuffor[nextIndex];
			nextIndex += SPACE_SIZE_CONTEXT;

			// SAVE START POSITIONS
			SaveNextIndex (values, nextIndex, i, offset);

			nextIndex += nameCount + rawContextCount;
		}
	}


	block ReadThroughWrite (
		OUT	size& 			nextIndex,
		IN	const u16& 		consoleX,
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

			Write (strType, consoleX, paddingLength, nameCount, name, rawContextCount, context);

			nextIndex += nameCount + rawContextCount;
		}
	}


	block ReadThroughConstructWrite (
		OUT	size& 				nextIndex,
		IN	const u16& 			consoleX,
		IN  const StrType&		strType,
		IN	const uint8& 		valuesCount,
		IN	const uint8& 		constantsCount,
		IN	const uint8* const 	constants
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

			ConstructConstants (
				rawContextCount, rawContext, 
				constantsCount, constants, 
				contextCount, memoryBlockB.data + nameIndex
			);

			auto&& name = memoryBlockB.data;
			auto&& context = memoryBlockB.data + nameCount;

			Write (strType, consoleX, paddingLength, nameCount, name, contextCount, context);

			nextIndex += nameCount + rawContextCount;
		}
	}

	block ReadThroughAllGetConstants (
		IN	const uint8& filesCount,
		OUT	size& allConstantsCount,
		OUT	uint8*& allConstants
	) {

		size nextIndex = SPACE_SIZE_FILES_COUNT + GetDirectoryOffset ();

		for (uint8 j = 0; j < filesCount + 1; ++j) {

			const uint8& constantsCount = memoryBlockA.data[nextIndex + GetHeaderIndex(0)];
			const uint8& importsCount 	= memoryBlockA.data[nextIndex + GetHeaderIndex(1)];
			const uint8& commandsCount 	= memoryBlockA.data[nextIndex + GetHeaderIndex(2)];
			const uint8& queuesCount 	= memoryBlockA.data[nextIndex + GetHeaderIndex(3)];

			if ((constantsCount + importsCount + commandsCount + queuesCount) == 0) {
				const array<const char, 26> errorMessage { "\nERROR: Empty linked file!" };
				fwrite (errorMessage.Pointer(), sizeof (char), errorMessage.Length(), stdout);
			}

			nextIndex += GetHeaderIndex(4);

			ReadThroughSave (nextIndex, allConstants, constantsCount, allConstantsCount);
			ReadThrough (nextIndex, importsCount);
			ReadThrough (nextIndex, commandsCount);
			ReadThrough (nextIndex, queuesCount);

			allConstantsCount += constantsCount;
		}
	}


	block ReadThroughCommandMatchConstructExecute (
		OUT						size& 						nextIndex,
		IN  					const StrType&				strType,
		IN						const uint8& 				valuesCount,
		IN						const uint8& 				constantsCount,
		IN						const uint8* const 			constants,
		IN 						const uint8& 				subcmmdLength, 
		INREADS (projectLength) const charConsole* const 	subcmmdName
	) {

		// 1. Get all names as search indecies.
		// 2. Parse those names into search function with our subcommand.
		// 3. Get rawContext and construct actual context.
		// 4. Null-terminate and execute

		auto onNoMatchFound = [](uint16& resultIndex, const uint8 elementsCount) { 
			resultIndex = elementsCount + 1;
		};

		auto&& filesBuffor = memoryBlockA.data;
		auto&& names = memoryBlockB.data;

		ReadThroughSaveEx (filesBuffor, nextIndex, valuesCount, names, 0);

		uint16 resultIndex (0);
		Search::Buffor::ByPFM<charFile, uint16> (
			onNoMatchFound, resultIndex,
			subcmmdLength, subcmmdName,
			filesBuffor, valuesCount, names,
			SPACE_SIZE_CONSTANTS_INDEX
		);

		if (resultIndex != valuesCount) {

			// This is bad.. i do the very same in Search::Buffor::ByPFM function...
			uint32 bufforIndex = 0;
			LoadNextIndex (bufforIndex, names, resultIndex);


			auto&& nameCount = filesBuffor[bufforIndex - 2];
			auto&& rawContextCount = filesBuffor[bufforIndex - 1];
			auto&& rawContext = filesBuffor + bufforIndex + nameCount;
			auto&& context = memoryBlockB.data;

			uint16 contextCount = 0;

			ConstructConstants (
				rawContextCount, rawContext, 
				constantsCount, constants, 
				contextCount, context
			);

			// null-terminate the string.
			context[contextCount] = '\0';

			std::system((const char *)(context));

			exit(0);
		}

	}

	block ReadThroughQueueMatchConstructExecute (
		OUT						size& 						nextIndex,
		IN  					const StrType&				strType,
		IN						const uint8& 				commandsCount,
		IN						const uint8& 				queuesCount,
		IN						const uint8& 				constantsCount,
		IN						const uint8* const 			constants,
		IN 						const uint8& 				subcmmdLength, 
		INREADS (projectLength) const charConsole* const 	subcmmdName
	) {

		// 1. Get all command names as search indecies.
		// 2. Get command_name's from queue context in an array.
		// FOR EACH SUBCOMMAND
		//	3. parse those names into search function with our subcommand.
		//	4. get rawContext and construct actual context.
		//	5. null-terminate and execute

		auto onNoMatchFound = [](uint16& resultIndex, const uint8 elementsCount) { 
			resultIndex = elementsCount + 1;
			printf (ErrorMSG::INVALID_QUEUE_COMMAND);
			exit (1);
		};

		auto&& filesBuffor = memoryBlockA.data;
		auto&& commandsBuffor = memoryBlockB.data;
		auto&& names = memoryBlockB.data + (commandsCount * SPACE_SIZE_CONSTANTS_INDEX);

		ReadThroughSaveEx (filesBuffor, nextIndex, queuesCount, names, 0);

		uint16 resultIndex (0);
		Search::Buffor::ByPFM<charFile, uint16> (
			onNoMatchFound, resultIndex,
			subcmmdLength, subcmmdName,
			filesBuffor, queuesCount, names,
			SPACE_SIZE_CONSTANTS_INDEX
		);

		if (resultIndex != queuesCount) {

			// This is bad.. i do the very same in Search::Buffor::ByPFM function...
			uint32 bufforIndex = 0;
			LoadNextIndex (bufforIndex, names, resultIndex);

			auto&& queueNameCount = filesBuffor[bufforIndex - 2];
			auto&& rawContextCount = filesBuffor[bufforIndex - 1];
			auto&& rawContext = filesBuffor + bufforIndex + queueNameCount;

			// 1. Save first position of context in names
			// For each "," save another position in place of names as that is no longer used.

			const auto&& divider = ',';
			uint8 elementLength = 0;
			uint8 substringsCount = 1;
			uint8 substringsTempCount = 0;
			uint8 collision = 0;

			// Prep substring start position

			// Get substring count
			for (size i = 0; i < rawContextCount; ++i) {
				collision = rawContext[i] == divider;
				substringsCount += collision;
			}

			std::memset(names, 0, (substringsCount * 1) + 1);

			// Save other substring start positions
			for (size i = 0; i < rawContextCount; ++i) {
				collision = rawContext[i] == divider;
				substringsTempCount += collision;

				// It will return to 0 at collision.
				elementLength = ++elementLength * !collision; 

				//names[(substringsTempCount * 2) + 0] += (bufforIndex + queueNameCount + i) * collision; // Start Index
				names[(substringsTempCount * 1) + 1] = elementLength; // Substring Count
			}


			uint8 prevCommandLength = 0;

			for (size i = 0; i < substringsCount; ++i) {
				const auto& commandNameLength = names[(i * 1) + 1];
				const auto&& commandName = (const charFile* const)(filesBuffor + bufforIndex + queueNameCount + prevCommandLength + i);
				prevCommandLength += commandNameLength;

				// Now each command have to be matched with the ones declared, constructed and then executed.
				//  Therefore we will copy-paste partly code from "ReadThroughCommandMatchConstructExecute"

				{
					auto onQueueCommandMatchNoFound = [](uint16& resultIndex, const uint8 elementsCount) { 
						printf (ErrorMSG::INVALID_QUEUE_SUBCOMMAND);
						exit (1);
					};

					resultIndex = 0;
					Search::Buffor::ByPFM<charFile, uint16> (
						onQueueCommandMatchNoFound, resultIndex,
						commandNameLength, commandName,
						filesBuffor, commandsCount, commandsBuffor,
						SPACE_SIZE_CONSTANTS_INDEX
					);

					if (resultIndex != commandsCount) {
						
						// This is bad.. i do the very same in Search::Buffor::ByPFM function...
						uint32 bufforIndex = 0;
						LoadNextIndex (bufforIndex, commandsBuffor, resultIndex);


						auto&& nameCount = filesBuffor[bufforIndex - 2];
						auto&& rawContextCount = filesBuffor[bufforIndex - 1];
						auto&& rawContext = filesBuffor + bufforIndex + nameCount;
						//auto&& context = memoryBlockB.data + (commandsCount * SPACE_SIZE_CONSTANTS_INDEX);
						uint8 context[256] { 0 }; 

						//fwrite (rawContext, sizeof (char), rawContextCount, stdout);
						//fputc('\n', stdout);

						uint16 contextCount = 0;

						ConstructConstants (
							rawContextCount, rawContext, 
							constantsCount, constants, 
							contextCount, context
						);

						// null-terminate the string.
						context[contextCount] = '\0';

						//fputc('\n', stdout);
						//fwrite (context, sizeof (char), contextCount, stdout);

						std::system((const char *)(context));
					}
				}
			}


			exit(0);
		}

	}




























	block GetConsoleWidth (u16& x) {
		CONSOLE_SCREEN_BUFFER_INFO csbi;
    	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    	x = csbi.srWindow.Right - csbi.srWindow.Left + 1;
	}



	block DisplayFiles () {
		const uint8& filePathLength	= memoryBlockA.data[SPACE_SIZE_FILES_COUNT + SPACE_SIZE_DIRECTORY_LENGTH];
		const uint8& constantsCount	= memoryBlockA.data[SPACE_SIZE_FILES_COUNT + GetDirectoryOffset () + GetHeaderIndex (0)];
		const uint8& importsCount	= memoryBlockA.data[SPACE_SIZE_FILES_COUNT + GetDirectoryOffset () + GetHeaderIndex (1)];
		const uint8& commandsCount	= memoryBlockA.data[SPACE_SIZE_FILES_COUNT + GetDirectoryOffset () + GetHeaderIndex (2)];
		const uint8& queuesCount	= memoryBlockA.data[SPACE_SIZE_FILES_COUNT + GetDirectoryOffset () + GetHeaderIndex (3)];

		size nextIndex = GetHeaderOffset ();

		u16 consoleSizeX = 0;
		GetConsoleWidth (consoleSizeX);

		auto&& constants = memoryBlockC.data;
		auto&& filePath = memoryBlockA.data + SPACE_SIZE_FILES_COUNT + SPACE_SIZE_DIRECTORY_LENGTH + 1;

		//printf("\nv: %hhu", memoryBlockA.data[SPACE_SIZE_FILES_COUNT + SPACE_SIZE_DIRECTORY_LENGTH + ((55 * 2) + 2)]);

		
		WriteDirectory (filePathLength, filePath);

		// DISPLAY IMPORTS
		ReadThroughSave (nextIndex, constants, constantsCount);
		ReadThroughConstructWrite (nextIndex, consoleSizeX, STR_TYPE_IMPORT, importsCount, constantsCount, constants);
		ReadThroughWrite (nextIndex, consoleSizeX, STR_TYPE_COMMAND, commandsCount);
		ReadThroughWrite (nextIndex, consoleSizeX, STR_TYPE_PIPE, queuesCount);

		fputc ('\n', stdout);
		
	}


	block DisplayProject () {
		const uint8& filesCount		= memoryBlockA.data[INDEX_FILES_COUNT];
		const uint8& filePathLength	= memoryBlockA.data[SPACE_SIZE_FILES_COUNT + SPACE_SIZE_DIRECTORY_LENGTH];

		auto&& filePath = memoryBlockA.data + SPACE_SIZE_FILES_COUNT + SPACE_SIZE_DIRECTORY_LENGTH + 1;
		uint8*& allConstants		= memoryBlockC.data;

		size allConstantsCount		= 0;
		size nextIndex				= SPACE_SIZE_FILES_COUNT + GetDirectoryOffset ();

		u16 consoleSizeX = 0;
		GetConsoleWidth (consoleSizeX);

		ReadThroughAllGetConstants (filesCount, allConstantsCount, allConstants);

		WriteDirectory (filePathLength, filePath);

		// Read again to write and apply constants.
		for (uint8 j = 0; j < filesCount + 1; ++j) {
			const uint8& constantsCount = memoryBlockA.data[nextIndex + GetHeaderIndex(0)];
			const uint8& importsCount 	= memoryBlockA.data[nextIndex + GetHeaderIndex(1)];
			const uint8& commandsCount 	= memoryBlockA.data[nextIndex + GetHeaderIndex(2)];
			const uint8& queuesCount 	= memoryBlockA.data[nextIndex + GetHeaderIndex(3)];
		
			nextIndex += GetHeaderIndex(4);
		
			ReadThrough (nextIndex, constantsCount);
			ReadThrough (nextIndex, importsCount);
			ReadThroughConstructWrite (nextIndex, consoleSizeX, STR_TYPE_COMMAND, commandsCount, allConstantsCount, allConstants);
			ReadThroughWrite (nextIndex, consoleSizeX, STR_TYPE_PIPE, queuesCount);
		}

		fputc ('\n', stdout);
	}

	// 2 fors, 
	//  1 - read through and get all constants from imports
	//  2 - write all commands and queues

	block ExecuteSubcommand (
		IN 						const uint8& 				subcmmdLength, 
		INREADS (projectLength) const charConsole* const 	subcmmdName
	) {
		const uint8& filesCount = memoryBlockA.data[INDEX_FILES_COUNT];
		
		uint8*& allConstants = memoryBlockC.data;
		size allConstantsCount = 0;

		size nextIndex = SPACE_SIZE_FILES_COUNT + GetDirectoryOffset ();

		//printf("0");

		ReadThroughAllGetConstants (filesCount, allConstantsCount, allConstants);

		//printf("1");

		for (uint8 j = 0; j < filesCount + 1; ++j) {
			const uint8& constantsCount = memoryBlockA.data[nextIndex + GetHeaderIndex(0)];
			const uint8& importsCount 	= memoryBlockA.data[nextIndex + GetHeaderIndex(1)];
			const uint8& commandsCount 	= memoryBlockA.data[nextIndex + GetHeaderIndex(2)];
			const uint8& queuesCount 	= memoryBlockA.data[nextIndex + GetHeaderIndex(3)];
		
			nextIndex += GetHeaderIndex(4);

			//printf ("2");
		
			ReadThrough (nextIndex, constantsCount);
			ReadThrough (nextIndex, importsCount);
			ReadThroughCommandMatchConstructExecute (nextIndex, STR_TYPE_COMMAND, commandsCount, allConstantsCount, allConstants, subcmmdLength, subcmmdName);
			ReadThroughQueueMatchConstructExecute (nextIndex, STR_TYPE_PIPE, commandsCount, queuesCount, allConstantsCount, allConstants, subcmmdLength, subcmmdName);
		}

	}

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