#pragma once
#include "lib/Framework.hpp"

#include "../Buffor.hpp"
#include "../Parse.hpp"

#include <cstdlib>

namespace Commands::Open::Projects {


	block GetConfigFilePath (
		OUT						charFilePath* const		filePath, 
		IN						const uint8& 			projectLength,
		INREADS (projectLength)	const charFile* const 	projectName,
		IN 						const uint8& 			importsCount, 
		INREADS (importsCount)	const uint8* const 		imports,
		IN 						const uint8& 			constantsCount,
		INREADS (constants) 	const uint8* const 		constants
	) {

		auto onNoMatchFound = []() { 
			printf ("\n%s\n\n", Search::Array::STRING_SEARCH_BYPFME_ERROR);
			exit (ExitCode::FAILURE_INVALID_ARGUMENT);
		};

		uint16 resultIndex (0);

		//printf("1");

		Search::Buffor::ByPFM<charFile, uint16> (
			onNoMatchFound, resultIndex,
			projectLength, projectName,
			memoryBlockA.data, importsCount, imports,
			SPACE_SIZE_CONSTATNS_INDEX
		);

		uint32 index = imports[resultIndex * SPACE_SIZE_CONSTATNS_INDEX + 0];
		index <<= 8;
		index += imports[resultIndex * SPACE_SIZE_CONSTATNS_INDEX + 1];
		index <<= 8;
		index += imports[resultIndex * SPACE_SIZE_CONSTATNS_INDEX + 2];
		index <<= 8;
		index += imports[resultIndex * SPACE_SIZE_CONSTATNS_INDEX + 3];

		//printf("\nwhy:%i, %i\n", resultIndex, index);

		// Get RAW string
		const auto&& rawContext = memoryBlockA.data + 
			index + 
			imports[(CONSTANTS_LIMIT * 4) + resultIndex];

		const uint8& rawContextCount = 
			imports[(CONSTANTS_LIMIT * 5) + resultIndex];

		//printf("c:%i\n", imports[(CONSTANTS_LIMIT * 4) + resultIndex]);
		//printf("c:%i\n", rawContextCount);
		//fwrite(rawContext, sizeof(char), rawContextCount, stdout);
		
		auto&& context = memoryBlockB.data;
		uint16 contextCount (0);

		Parse::ConstructConstants(
			rawContextCount, rawContext, 
			constantsCount, constants, 
			contextCount, context
		);

		//printf("\nc:%i\n", contextCount);
		//fwrite(context, sizeof(char), contextCount, stdout);
		//printf("\n\n");

		// RETURN RESULT IN WCHAR* FORMAT
		std::mbstowcs(filePath, (char *)context, contextCount);
	}

	// Go THROUGHT buffor to:
	// 1. Get all constants positions
	// 2. Get all current file imports
	// 3. match argument_project_name with first import 
	// 4. Construct project_path with said constants
	// 5. Change path in char* to wchar*
	// 6. return wchar* path


	block ValidateProjectArgument (
		OUT						charFilePath* const		filePath, 
		IN						const uint8& 			projectLength,
		INREADS (projectLength)	const charFile* const 	projectName
	) {
		const uint8& constantsCount	= memoryBlockA.data[SPACE_SIZE_FILES_COUNT + GetHeaderIndex(0)];
		const uint8& importsCount	= memoryBlockA.data[SPACE_SIZE_FILES_COUNT + GetHeaderIndex(1)];

		// Declare and iterator to iterate through our main buffor.
		size nextIndex = SPACE_SIZE_FILES_COUNT + INDEX_OFFSET;

		// Prepere buffor for LOOK_UP_POSITIONS for CONSTANTS.
		auto&& constants = memoryBlockC.data;
		// Prepere buffor for LOOK_UP_POSITIONS for IMPORTS.
		auto&& imports = memoryBlockB.data;

		Parse::ReadThroughSave (nextIndex, constants, constantsCount);
		Parse::ReadThroughSave (nextIndex, imports, importsCount);

		
		//printf("\n1:%i", imports[3]);
		//printf("\n2:%i", imports[(CONSTANTS_LIMIT * 4) + 0]);
		//printf("\n3:%i\n", imports[(CONSTANTS_LIMIT * 5) + 0]);

		// Construct IMPORTS context with CONSTANTS, match and retrive.
		GetConfigFilePath (
			filePath, projectLength, projectName,
			importsCount, imports, 
			constantsCount, constants
		);
	}

	// FILESCOUNT
	// { PROJECT FILE
	//	 - constantsCount
	//	 - importsCount
	//	 - commandsCount
	//	 - queuesCount
	//	 - data {name, context}
	// }
	// { IMPORT FILE
	//	 - constantsCount
	//	 - importsCount
	//	 - commandsCount
	//	 - queuesCount
	//	 - data {name, context}
	// }

	block RestructureBufforForProject() {

	}


	block AddImport (
		OUT						uint8& 					filePathsCount, 
		OUT						charFilePath* const		filePaths, 
		IN						const uint8& 			projectLength,
		INREADS (projectLength)	const charFile* const 	projectName
	) {

		//const uint16& constantsCount	 = memoryBlockA.data[SPACE_SIZE_FILES_COUNT + GetHeaderIndex(0)];
		//const uint16& importsCount		 = memoryBlockA.data[SPACE_SIZE_FILES_COUNT + GetHeaderIndex(1)];
		//const uint16& commandsCount		 = memoryBlockA.data[SPACE_SIZE_FILES_COUNT + GetHeaderIndex(2)];
		//const uint16& queuesCount		 = memoryBlockA.data[SPACE_SIZE_FILES_COUNT + GetHeaderIndex(3)];
		//
		//size nextIndex = SPACE_SIZE_FILES_COUNT + INDEX_OFFSET;

	}


}