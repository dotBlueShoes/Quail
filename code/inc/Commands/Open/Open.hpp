#pragma once
#include "lib/Framework.hpp"

#include "Buffors.hpp"

#include "Stages.hpp"
#include "Parse.hpp"
#include "IO.hpp"

#include "Projects/Validate.hpp"


namespace Commands::Open::Subcommands {

	// Predefined subcommands for common use.
	// We will define them in config instead !!!!
	//const array<const charConsole, 6> config = "config";
	//const array<const charConsole, 4> list = "list";

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

			namespace COS = Commands::Open::Stages;

			COS::StageParams stageParams { EOF, 0 };
			COS::Current = COS::MainFile;
			COS::Initialize();

			do { // The actuall read operation.
				stageParams.current = fgetc (fileConfiguration);
				COS::Current(stageParams);
			} while (stageParams.current != EOF);

			// CLOSE OPENNED FILES
			fclose(fileConfiguration);
		}

	};

	/// READS the linked project config file.
	block GetConfigData ( 
		IN	const charFilePath* const filePath
	) {

		{ /* WINDOWS ONLY */
			FILE* fileConfiguration = nullptr;

			// IF file pointer return NULL - EXIT THE PROGRAM
			if ((fileConfiguration = _wfopen (filePath, L"r")) == NULL) {
				printf ("%s\n", strings::STRING_FAILURE_NO_CONFIG_FILE);
				exit (ExitCode::FAILURE_NO_CONFIG_FILE);
			}

			printf("\nReading Project File\n\n");

			namespace COS = Commands::Open::Stages;
			
			COS::StageParams stageParams { EOF, 0 };
			COS::Current = COS::ProjectFile;
			COS::ImportReset();
			
			do { // The actuall read operation.
				stageParams.current = fgetc (fileConfiguration);
				COS::Current(stageParams);
			} while (stageParams.current != EOF);

			// CLOSE OPENNED FILES
			fclose(fileConfiguration);
		}

	}

}