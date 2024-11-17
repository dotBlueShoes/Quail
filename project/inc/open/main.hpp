// Created 2024.11.05 by Matthew Strumiłło (dotBlueShoes)
//
#pragma once
#include "base/types.hpp"
#include "base/io.hpp"

#include "interpreter/main.hpp"
#include "data.hpp"
#include "comparesearch.hpp"

namespace OPEN {

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

		//LOGINFO ("ConfigFile read successfully\n");

		{ // Main Config
			while (includesCounter != includes.size()) {

				auto&& string = (c16*)(includes[includesCounter]);

				//printf ("INFO: PATH: %ls\n", string);

				FILE* config = nullptr;
				IO::Read (string, config);
				files.push_back (config);

				INTERPRETER::parsingstage = INTERPRETER::MAIN::GetAllFiles;
				interpreter.current = 0;
				interpreter.special = 0;

				while (interpreter.current != EOF) { // READ
					interpreter.current = getc (config);
					INTERPRETER::parsingstage (interpreter);
				}

				LOGWINFO (">> IncludeFile [%d]:`%s` read successfully\n", includesCounter, string);

				++includesCounter;
			}
		}
	}


	auto FindProject (
		const c8* const& command,
		const u16& commandLength
	) {
		u32 index = 0;

		COMPARESEARCH::ArrayPartFirstMatchVector ( 
			command, commandLength, sizeof (c8),
			index, 
			projects.keys.size () - projectsOffset,
			(void**)(projects.keys.data () + projectsOffset)
		);

		index += projectsOffset; // Add what we removed. We offseted it before to search it the right way.
		projectsOffset = projects.keys.size ();

		return index;
	}
	

	void GetProjects (
		INTERPRETER::Interpreter& interpreter,
		u32& includesCounter,
		const u32& index
	) {

		const auto& configLength = projects.capes[index].configLength;
		const auto& pathLength = projects.capes[index].pathLength;

		const u32 configFilePathLength = (pathLength + configLength) / 2;
		const auto&& configFilePath = (c16*) (projects.configs[index]);
				
		LOGINFO ("> ProjectFile [%d]:`%ls` read successfully\n", index, configFilePath);

		// And another one...
		FILE* config = nullptr;
		IO::Read (configFilePath, config);
		files.push_back (config);

		{

			u32 directoryPathLength = configFilePathLength;
			u32 projectIncludesCounter = includesCounter;

			{ // Get directoryPath from filePath
				for (; configFilePath[directoryPathLength] != '\\'; --directoryPathLength);
				++directoryPathLength;
			}

			currentConfigFolderLength = directoryPathLength * 2;
			currentConfigFolder = configFilePath;

			interpreter.current = 0;
			interpreter.special = index;

			GetIncludes (interpreter, includesCounter, config);

			{ // Store the amount of include files a project has.
				projects.capes[index].special.includesCount = includesCounter - projectIncludesCounter;
			}

		}

	}


	void Open (
		const u32& depth,
		const c8* const* const& actions
	) {

		FILE* mainConfig = nullptr;
		INTERPRETER::Interpreter interpreter { 0 };
		u32 includesCounter = 0;
		u8 openType = OPEN_TYPE_LISTING;

		IO::Read (mainConfigFilePath, mainConfig);
		files.push_back (mainConfig);

		const u32 lastDepth = depth - 1;
		const auto& lastAction = actions[lastDepth];
		u16 lastActionLength = 0; 

		{ // 1st READ

			{// MAIN CONFIG
				currentConfigFolderLength = mainConfigFolderPathLength;
				currentConfigFolder = mainConfigFilePath;

				GetIncludes (interpreter, includesCounter, mainConfig);
			}

			if (depth != 0) {

				for (u32 iDepth = 0; iDepth < lastDepth; ++iDepth) { // PROJECTS / SUBPROJECTS ONLY.
					const auto& command = actions[iDepth];
					u16 commandLength = 0; for (; command[commandLength] != TYPE_EOS; ++commandLength);

					u32 index = FindProject (command, commandLength);

					if (index == projects.keys.size ()) {
						ERROR ("Could not match with any project.\n\n");
					} else { GetProjects (interpreter, includesCounter, index); }
				}

				{ // Has to be a either a project / subproject / command / queue.
					for (; lastAction[lastActionLength] != TYPE_EOS; ++lastActionLength);
					u32 index = FindProject (lastAction, lastActionLength);

					if (index == projects.keys.size ()) {

						LOGWARN ("We are looking for a command not a listing.\n\n");
						openType = OPEN_TYPE_COMMAND;

					} else { GetProjects (interpreter, includesCounter, index); }
				}

			}

		}

		{ // 2nd Read

			LOGINFO ("2nd Read\n");

			for (u32 iFile = files.size (); iFile > 0; --iFile) {

				//LOGINFO ("CALL!\n");
				const auto& config = files[iFile - 1];
				rewind (config);

				INTERPRETER::parsingstage = INTERPRETER::MAIN::Main;
				interpreter.current = 0;
				interpreter.special = 0;

				while (interpreter.current != EOF) { // READ
					interpreter.current = getc (config);
					INTERPRETER::parsingstage (interpreter);
				}

			}

			// DISPLAYS ALL CONSTANTS
			// for (s32 iConstant = 0; iConstant < constants.keys.size(); ++iConstant) {
			// 	const auto&& value = (c16*) constants.values[iConstant];
			// 	const auto&& key = (c8*) constants.keys[iConstant];
			// 	LOGINFO ("Constant: %s: %ls\n", key, value);
			// }

			// // DISPLAYS ALL COMMANDS
			// for (s32 iCommand = 0; iCommand < commands.keys.size(); ++iCommand) {
			// 	const auto&& value = (c16*) commands.values[iCommand];
			// 	const auto&& key = (c8*) commands.keys[iCommand];
			// 	LOGINFO ("Command: %s: %ls\n", key, value);
			// }

			// Currently I am loading all constants, varaibles, secrets
			//  and that might not be very smart. Because doing so I am loading a lot of things that are not necessery needed.
			//  It would be much smarter to only load strings that are being used by a specific command or a queue of commands.
			//   - Its a different approach completly. We would look for a command then it's constants, varaibles, secrets
			//    and we would assemble the whole command part by part. However `Queues` and `Lists` would be much more difficult.
			//    It also does not seem to be a big optimalization too.

		}

		if (openType) { // EXECUTE
			LOGINFO ("Execute.\n");

			u32 index = 0;

			{ // Find Command in Commands
				COMPARESEARCH::ArrayPartFirstMatchVector ( 
					lastAction, lastActionLength, sizeof (c8),
					index, 
					commands.keys.size (),
					(void**)(commands.keys.data ())
				);

				if (index == commands.keys.size ()) {
					ERROR ("Not a valid command\n");
				} else {

					const auto&& command = (c16*) commands.values[index];
					LOGINFO ("id: [%d]: %ls\n", index, command);
					_wsystem (command);

				}
			}

		} else { 		// LISTING
			LOGINFO ("LISTING.\n");

			// TODO
			//  2. List queues key and value.

			putc ('\n', stdout);

			HANDLE console = GetStdHandle (STD_OUTPUT_HANDLE);

			// save previous color using. then restore. https://stackoverflow.com/questions/17125440/c-win32-console-color
			SetConsoleTextAttribute (console, 15);

			// Display only subprojects of a project
			for (u32 i = projectsOffset; i < projects.keys.size (); ++i) {
				const auto&& value = (c16* )projects.paths[i];
				const auto&& key = (c8* )projects.keys[i];
				printf ("    %c%s: %ls\n", TYPE_PROJECT, key, value);
			}

			for (u32 i = 0; i < commands.keys.size (); ++i) {
				const auto&& value = (c16* )commands.values[i];
				const auto&& key = (c8* )commands.keys[i];
				printf ("    %c%s: %ls\n", TYPE_COMMAND, key, value);
			}

			SetConsoleTextAttribute (console, 7);

			putc ('\n', stdout);

		}

		for (s32 i = 0; i < projects.keys.size(); ++i) {
			FREE (projects.configs[i]);
			FREE (projects.paths[i]);
			FREE (projects.keys[i]);
		}

		for (s32 i = 0; i < includes.size(); ++i) {
			FREE (includes[i]);
		}

		for (s32 i = 0; i < constants.keys.size(); ++i) {
			FREE (constants.values[i]);
			FREE (constants.keys[i]);
		}

		for (s32 i = 0; i < commands.keys.size(); ++i) {
			FREE (commands.values[i]);
			FREE (commands.keys[i]);
		}

		for (u32 iFile = 0; iFile < files.size(); ++iFile) {
			IO::Close (files[iFile]);
		}

		// ? That should be somewhere else ?
		FREE (mainConfigFilePath);
	}

}
