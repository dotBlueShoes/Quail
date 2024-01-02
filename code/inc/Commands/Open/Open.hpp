#pragma once
#include "lib/Framework.hpp"

#include "Buffors.hpp"

#include "Stages.hpp"
#include "Parse.hpp"
#include "IO.hpp"

#include "Projects/Validate.hpp"


namespace Commands::Open::Subcommands {

	// Predefined subcommands for common use.
	const array<const charConsole, 6> config = "config";
	const array<const charConsole, 4> list = "list";

}

namespace Commands::Open {

	/// READS the main projects config file. 
	block GetMainConfigData () {
		
		{ /* WINDOWS ONLY */
			FILE* fileConfiguration = nullptr;
			//size readLength = 0;

			// IF file pointer return NULL - EXIT THE PROGRAM
			if ((fileConfiguration = _wfopen(FILE_PROJECTS.Pointer(), L"r")) == NULL) {
				printf ("%s\n", strings::STRING_FAILURE_NO_CONFIG_FILE);
				exit (ExitCode::FAILURE_NO_CONFIG_FILE);
			}

			// THIS NEEDS TO BE RETHINKED !
			// // Obtain file size and check with buffor.
  			// fseek (fileConfiguration ,0 ,SEEK_END);
  			// readLength = ftell (fileConfiguration);
  			// rewind (fileConfiguration);
			// 
			// 
			// // Check if our memory buffor doing fine.
			// if (readLength > memoryBlockA.length) {
			// 	printf ("%s\n", strings::STRING_FAILURE_BUFFOR_TO_SMALL);
			// 	exit (ExitCode::FAILURE_BUFFOR_TO_SMALL);
			// }

			namespace COS = Commands::Open::Stages;

			//printf("%s", "START\n");

			COS::StageParams stageParams { EOF, 0 };
			COS::Current = COS::MainBeginStage;
			COS::Initialize();

			do { // The actuall read operation.
				stageParams.current = fgetc (fileConfiguration);
				// Call to stages extended
				COS::Current(stageParams);
				//printf("%c", stageParams.current);
				//printf("%c", '\n');
			} while (stageParams.current != EOF);

			// CLOSE OPENNED FILES
			fclose(fileConfiguration);
		}

	};

	/// READS the linked project config file.
	block GetConfigData ( 
		IN	const charFilePath* const filePath
	) {

		//printf("here");
		wprintf(L"\n%ls\n", filePath);

		//
		//for (size i = 0; i < other.Length(); ++i) {
		//	if (other[i] != filePath[i]) {
		//		printf("%i\n", i);
		//	}
		//}
		//
		//printf("%c", filePath[35]);

		//wprintf(L"\n%ls\n", L"lol");

		{ /* WINDOWS ONLY */
			FILE* fileConfiguration = nullptr;

			// IF file pointer return NULL - EXIT THE PROGRAM
			if ((fileConfiguration = _wfopen (filePath, L"r")) == NULL) {
				printf ("%s\n", strings::STRING_FAILURE_NO_CONFIG_FILE);
				exit (ExitCode::FAILURE_NO_CONFIG_FILE);
			}

			printf("2");

			//namespace COS = Commands::Open::Stages;
			
			//COS::StageParams stageParams { EOF, 0 };
			//COS::Current = COS::BeginStage;
			//COS::FileReset();

			
			
			//do { // The actuall read operation.
			//	stageParams.current = fgetc (fileConfiguration);
			//	// Call to stages
			//	COS::Current(stageParams);
			//} while (stageParams.current != EOF);
			//
			//printf("end");

			// CLOSE OPENNED FILES
			fclose(fileConfiguration);
		}

	}

}