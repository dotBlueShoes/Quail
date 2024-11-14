// Created 2024.11.05 by Matthew Strumiłło (dotBlueShoes)
//
#pragma once
#include "base/types.hpp"
#include "base/io.hpp"

#include "interpreter/main.hpp"
#include "data.hpp"
#include "comparesearch.hpp"

namespace OPEN {

	// TODO
	//  Add comments so we can comment out things and test things easier.
	//  issue with -> ./run -o Example sub
	//   1. relative vs full paths. Right now we're using fill paths we will need to implement relative paths
	//    therefore a way to identify them is needed maybe like `&+`, `&-` and `^+`, `^-` so both includes and projects can work this way.
	//    ISSUE! -> to psuje konwencje robiąc z 2 bufforów 4 sprawimy że nie będziemy mogli zrozumieć kolejności przy 2 czytaniu.
	//   2. I need to possess the project DIRECTORY without it's quail directory. Therefore maybe project should have 2 contents
	//    somethink along the lines `^project 'project-dir' `relative-path-to-.quail.txt``



	// Do funkcji `Open` trafiają teraz argumenty, które zmieniają działanie tej funkcji.
	// 1. Otwieramy MainConfig
	// 2. Jeśli depth jest równy 0 to znaczy że czytamy tylko includy bez projects i chcemy docelowo tylko 
	// wylistować projekty i komendy i kolejki
	// 3. Jeśli depth jest równy 1 to znaczy że albo chcemy 
	//	a) wylistować zawartosć projektu, którego ścieżka jest w mainConfig
	//    Jeśli tak to trzeba też zincludować pliki które może wskazać config projektu
	//  b) uruchomić komendę z mainConfig
	//  c) uruchomić kolejkę z mainConfig
	// 4. Jeśli depth jest równy więcej niż 1 to znaczy że mamy do czynienia z subprojektami
	// 

	void GetIncludes (
		INTERPRETER::Interpreter& interpreter,
		u32& includesCounter,
		FILE*& mainFile
	) {
		INTERPRETER::parsingstage = INTERPRETER::MAIN::GetAllFiles;

		while (interpreter.current != EOF) { // READ
			interpreter.current = getc (mainFile);
			INTERPRETER::parsingstage (interpreter);
		}

		LOGINFO ("ConfigFile read successfully\n");

		{ // Main Config
			while (includesCounter != includes.size()) {

				auto&& string = (c16*)(includes[includesCounter]);

				//printf ("INFO: PATH: %ls\n", string);

				FILE* config = nullptr;
				IO::Read (string, config);

				INTERPRETER::parsingstage = INTERPRETER::MAIN::GetAllFiles;
				interpreter.current = 0;
				interpreter.special = 0;

				while (interpreter.current != EOF) { // READ
					interpreter.current = getc (config);
					INTERPRETER::parsingstage (interpreter);
				}

				LOGWINFO ("IncludeFile [%d]:`%s` read successfully\n", includesCounter, string);
				IO::Close (config);

				++includesCounter;
			}
		}

		//printf ("INFO: A\n");
	}


	void GetProjects (
		INTERPRETER::Interpreter& interpreter,
		u32& includesCounter,
		const c8* const& command
	) {
		
		u16 commandLength = 0; for (; command[commandLength] != TYPE_EOS; ++commandLength);
		u32 index = 0;

		COMPARESEARCH::ArrayPartFirstMatchVector ( 
			command, commandLength, sizeof (c8),
			index, 
			projects.keys.size () - projectsOffset,
			(void**)(projects.keys.data () + projectsOffset)
		);

		index += projectsOffset; // Add what we removed. We offseted it before to search it the right way.
		projectsOffset = projects.keys.size ();

		if (index == projects.keys.size ()) {

			ERROR ("Could not match with any project.\n\n");

		} else {

			const auto& configLength = projects.capes[index].configLength;
			const auto& pathLength = projects.capes[index].pathLength;

			const u32 configFilePathLength = (pathLength + configLength) / 2;
			const auto&& configFilePath = (c16*) (projects.configs[index]);
				
			LOGINFO ("ProjectFile [%d]:`%ls`\n", index, configFilePath);

			// And another one...
			FILE* config = nullptr;
			IO::Read (configFilePath, config);

			{
				u32 directoryPathLength = configFilePathLength;

				{ // Get directoryPath from filePath
					for (; configFilePath[directoryPathLength] != '\\'; --directoryPathLength);
					++directoryPathLength;
				}

				currentConfigFolderLength = directoryPathLength * 2;
				currentConfigFolder = configFilePath;

				interpreter.current = 0;
				interpreter.special = index;

				GetIncludes (interpreter, includesCounter, config);
			}

			IO::Close (config);

		}
	}


	void Open (
		const u32& depth,
		const c8* const* const& commands
	) {

		FILE* mainConfig = nullptr;
		INTERPRETER::Interpreter interpreter { 0 };
		IO::Read (mainConfigFilePath, mainConfig);
		u32 includesCounter = 0;

		{ // 1st READ

			const u32 lastDepth = depth - 1;

			{// MAIN CONFIG
				currentConfigFolderLength = mainConfigFolderPathLength;
				currentConfigFolder = mainConfigFilePath;

				GetIncludes (interpreter, includesCounter, mainConfig);
			}

			for (u32 iDepth = 0; iDepth < lastDepth; ++iDepth) { // PROJECTS / SUBPROJECTS ONLY.
				const auto& command = commands[iDepth];
				GetProjects (interpreter, includesCounter, command);
			}

			{ // Has to be a either a project / subproject / command / queue.
				const auto& command = commands[lastDepth];
				GetProjects (interpreter, includesCounter, command);
			}

		}

		{ // 2nd Read

			// We're reading from back to begin. This gives as more control over what we read
			//  during this proces. We're able to create cascading CONSTANTS thanks to this method.

			for (s32 i = projects.keys.size(); i > 0; --i) {
				const auto index = i - 1;

				const auto&& value = (c16*) projects.configs[index];
				const auto&& key = (c8*) projects.keys[index];

				LOGINFO ("ProjectFile [%d]:`%s` (%ls) read successfully\n", index, key, value);

				// TODO
				// we need to store the amount of includes a project has 
				// to do so i created `projects.capes[i].special.includesCount` but it has to be populated first.

				// 1. Go thorugh each project-include
				// 2. Go through each project
				// 3. Read them using INTERPRETER::MAIN::Main
				// Where we will read
				// Constants, Secrets, Varaibles, Commands, Queues
			}

		}

		//printf ("INFO: C\n");

		for (s32 i = 0; i < projects.keys.size(); ++i) {

			//{
			//	auto&& value = (c16*) projects.configs[i];
			//	auto&& key = (c8*) projects.keys[i];
			//	LOGINFO ("ProjectFile [%d]:`%s` (%ls) read successfully\n", i, key, value);
			//}

			FREE (projects.configs[i]);
			FREE (projects.paths[i]);
			FREE (projects.keys[i]);
		}

		//printf ("INFO: D\n");

		for (s32 i = 0; i < includes.size(); ++i) {
			FREE (includes[i]);
		}

		//printf ("INFO: E\n");

		// SHOULD BE MOVED OUTSIDE THAT !!!!
		IO::Close (mainConfig);
		FREE (mainConfigFilePath);

		//printf ("INFO: F\n");
	}

}
