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
		COS::Reset ();

		ReadAndCloseStream (fileConfiguration, stage);

	};


	block GetConfigData ( 
		IN	const charFilePath* const filePath
	) {

		FILE* const fileConfiguration = GetFile ( filePath );

		namespace COS = Commands::Open::Stages;
			
		COS::StageParams stage { EOF, 0 };
		COS::Current = COS::ProjectFile;
		COS::AddImport ();
			
		ReadAndCloseStream (fileConfiguration, stage);

	}

}