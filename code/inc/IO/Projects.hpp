#pragma once
#include "lib/Framework.hpp"

#include "Constants.hpp"
#include "Buffors.hpp"

#include "Projects/Stages.hpp"
#include "Projects/Parse.hpp"

namespace IO {

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

			namespace IPS = IO::Projects::Stages;

			printf("%s", "START\n");

			IPS::StageParams stageParams { EOF, 0 };
			IPS::Current = IPS::MainBeginStage;
			IPS::Initialize();

			do { // The actuall read operation.
				stageParams.current = fgetc (fileConfiguration);
				// Call to stages extended
				IPS::Current(stageParams);
				//printf("%c", stageParams.current);
				//printf("%c", '\n');
			} while (stageParams.current != EOF);

			printf("%s", "DISPLAY\n");

			// DISPLAY COLLECTED DATA.
			namespace IPP = IO::Projects::Parse;
			IPP::DisplayFiles();

			printf("%s", "END\n");

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

			namespace IPS = IO::Projects::Stages;

			IPS::StageParams stageParams { EOF, 0 };
			IPS::Current = IPS::BeginStage;
			IPS::Initialize();

			do { // The actuall read operation.
				stageParams.current = fgetc (fileConfiguration);
				// Call to stages
				IPS::Current(stageParams);
			} while (stageParams.current != EOF);
		}

	}

}