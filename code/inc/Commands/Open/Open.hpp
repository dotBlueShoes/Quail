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

	// Pretty much WINDOWS ONLY stuff here !

	getter constexpr FILE* GetFile (
		IN	const charFilePath* const filePath
	) {
		FILE* fileConfiguration = nullptr;

		// IF file pointer return NULL - EXIT THE PROGRAM
		if ((fileConfiguration = _wfopen(filePath, L"r")) == NULL) {
			printf ("%s\n", strings::STRING_FAILURE_NO_CONFIG_FILE);
			exit (ExitCode::FAILURE_NO_CONFIG_FILE);
		}

		return fileConfiguration;
	}


	block ReadAndCloseStream (
		IN	FILE* const fileConfiguration,
		IN	Commands::Open::Stages::StageParams& stage
	) {
		do {
			stage.current = fgetc (fileConfiguration);
			Commands::Open::Stages::Current(stage);
		} while (stage.current != EOF);

		fclose(fileConfiguration);
	}


	block GetMainConfigData () {
		
		FILE* const fileConfiguration = GetFile ( FILE_PROJECTS.Pointer() );

		namespace COS = Commands::Open::Stages;

		COS::StageParams stage { EOF, 0 };
		COS::Current = COS::MainFile;
		COS::Initialize ();

		ReadAndCloseStream (fileConfiguration, stage);

	};


	block GetProjectConfigData (
		IN	const charFilePath* const filePath
	) {
		
		FILE* const fileConfiguration = GetFile ( filePath );

		namespace COS = Commands::Open::Stages;

		COS::StageParams stage { EOF, 0 };
		COS::Current = COS::MainFile;
		COS::Reset();

		ReadAndCloseStream (fileConfiguration, stage);

	};


	block GetConfigData ( 
		IN	const charFilePath* const filePath
	) {

		FILE* const fileConfiguration = GetFile ( filePath );

		namespace COS = Commands::Open::Stages;
			
		COS::StageParams stage { EOF, 0 };
		COS::Current = COS::ProjectFile;
		COS::AddImport();
			
		ReadAndCloseStream (fileConfiguration, stage);

	}

}