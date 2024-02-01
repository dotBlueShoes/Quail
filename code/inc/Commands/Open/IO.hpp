#pragma once
#include "lib/Framework.hpp"

namespace Commands::Open {

	//const array<const char, 42> FOLDER_CONFIGS (R"(D:\ProgramFiles\dotBlueShoes\quail\configs)");
	//const array<const char, 12> FILE_PROJECTS (R"(projects.txt)");

	const array<const wchar, 56> FILE_PROJECTS (LR"(D:\ProgramFiles\dotBlueShoes\quail\configs\projects.txt)");


	#pragma region [ WINDOWS ONLY ]

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

		fclose (fileConfiguration);
	}

	#pragma endregion [ WINDOWS ONLY ]

}

