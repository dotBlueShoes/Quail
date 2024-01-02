#pragma once
#include "lib/Framework.hpp"

#include "Buffors.hpp"

#include "Stages.hpp"
#include "Parse.hpp"
#include "IO.hpp"


namespace Commands::Open::Subcommands {

	// Predefined subcommands for common use.
	const array<const charConsole, 6> config = "config";
	const array<const charConsole, 4> list = "list";

}

namespace Commands::Open {

	/// READS the main projects config file. 
	block GetProjectsData () {
		
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

			//printf("%s", "DISPLAY\n");

			// DISPLAY COLLECTED DATA.
			namespace COP = Commands::Open::Parse;
			COP::DisplayFiles();

			//printf("%s", "END\n");

		}

	};

	/// READS the linked project config file.
	block GetProjectData ( 
		IN	wchar* filePath
	) {

		{ /* WINDOWS ONLY */
			FILE* fileConfiguration = nullptr;

			// IF file pointer return NULL - EXIT THE PROGRAM
			if ((fileConfiguration = _wfopen (filePath, L"r")) == NULL) {
				exit (ExitCode::FAILURE_NO_CONFIG_FILE);
			}

			namespace COS = Commands::Open::Stages;

			COS::StageParams stageParams { EOF, 0 };
			COS::Current = COS::BeginStage;
			COS::Initialize();

			do { // The actuall read operation.
				stageParams.current = fgetc (fileConfiguration);
				// Call to stages
				COS::Current(stageParams);
			} while (stageParams.current != EOF);
		}

	}

}