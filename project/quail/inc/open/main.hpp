// Created 2024.11.05 by Matthew Strumiłło (dotBlueShoes)
//  LICENSE: GNU GENERAL PUBLIC LICENSE Version 3, 29 June 2007
//
#pragma once
#include <blue/types.hpp>
#include <blue/io.hpp>

#include "interpreter/main.hpp"
#include "data.hpp"

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


	//

	void ReadFile (
		FILE* const& file,
		INTERPRETER::Interpreter& interpreter
	) {
		while (interpreter.current != EOF) { // READ
			interpreter.current = getc (file);
			INTERPRETER::parsingstage (interpreter);
		}
	}

	void GetIncludes (
		INTERPRETER::Interpreter& interpreter,
		u32& includesCounter,
		FILE*& mainFile
	) {
		INTERPRETER::parsingstage = INTERPRETER::MAIN::GetAllFiles;

		ReadFile (mainFile, interpreter);

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

				ReadFile (config, interpreter);

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

		COMPARESEARCH::ArrayPartFirstMatchVectorLowCase ( 
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

	// Adds "_", "_path" and "_name" constants for use in config.
	void AddHiddenConstants ( 
		const u32& pathLength,
		const c16* const& path,
		const u32& nameLength,
		const c8* const& name
	) {

		{ // "_"

			{ // - key
				const u8 temp[] { "_" }; // Const defintion.
				const u32 tempLength = sizeof (temp);

				// Cpy -> We can easly deallocate it with others later.
				u8* constantName; ALLOCATE (u8, constantName, tempLength);
				memcpy (constantName, temp, tempLength);

				constants.keys.push_back (constantName);
			}

			{ // - value
				const u8 temp[] { "%" }; // Const defintion.
				const u32 tempLength = sizeof (temp);

				u8* constantValue; ALLOCATE (u8, constantValue, tempLength);
				memcpy (constantValue, temp, tempLength);

				constants.values.push_back (constantValue);
				constants.valueLengths.push_back (tempLength);
			}

		}

		{ // "_name"

			{ // - key
				const u8 temp[] { "_name" }; // Const defintion.
				const u32 tempLength = sizeof (temp);

				// Cpy -> We can easly deallocate it with others later.
				u8* constantName; ALLOCATE (u8, constantName, tempLength);
				memcpy (constantName, temp, tempLength);

				constants.keys.push_back (constantName);
			}

			{ // - value

				{ // Convertion c8* to c16*
					SetFirstTempW (name[0]);
					for (u32 i = 1; i < nameLength; ++i) {
						AddTempW (name[i]);
					}
				}

				u8* constantValue; ALLOCATE (u8, constantValue, temporaryLength);
				memcpy (constantValue, temporary, temporaryLength);

				constants.values.push_back (constantValue);
				constants.valueLengths.push_back (temporaryLength - 2); // minus EOF
			}

		}
		
		{ // "_path"

			{ // - key
				const u8 temp[] { "_path" }; // Const defintion.
				const u32 tempLength = sizeof (temp);

				// Cpy -> We can easly deallocate it with others later.
				u8* constantName; ALLOCATE (u8, constantName, tempLength);
				memcpy (constantName, temp, tempLength);

				constants.keys.push_back (constantName);
			}

			{ // value (copy but with removal of '/' sign at the end)
				u8* constantValue; ALLOCATE (u8, constantValue, pathLength); // should alloc one less char.
				memcpy (constantValue, path, pathLength - 2);

				constantValue[pathLength - 2] = 0;
				constantValue[pathLength - 1] = 0;

				constants.values.push_back (constantValue);
				constants.valueLengths.push_back (pathLength - 2); // minus EOF
			}

		}
		
	}


	void DisplayQueueCmd (
		const HANDLE& console,
		const c8* const& key,
		const c8& symbol,
		const u8& color
	) {
		const c8 LP[] = "    ";

		fwrite (LP, 	 sizeof (c8),	strlen (LP),	stdout);

		printf ("...executing [%c", symbol);
		SetConsoleTextAttribute (console, color);
		fwrite (key, 	 sizeof (c8), 	strlen (key), 	stdout);
		SetConsoleTextAttribute (console, 7);

		printf ("]\n");
	}

	// Prints the quail-actions in a customized manner with said color.
	void Display (
		const HANDLE& console,
		const c16* const& value,
		const c8* const& key,
		const c8& symbol,
		const u8& color
	) {
		// Name Padding, Line Padding
		const c8 NP[] = { ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ' };
		const c8 LP[] = "    ";

		const u16 NAME_PADDING_SIZE = 18;
		const u16 LINE_PADDING_SIZE = 120;

		// When padding is negative change it to equal 0.
		s16 nps = NAME_PADDING_SIZE - strlen (key); nps *= (nps > 0);
		s16 lps = LINE_PADDING_SIZE - 2 - nps - strlen (key) - 1 - strlen (LP) - 3; 

		s16 valueSize = wcslen (value);
		s16 lineEndSize = lps - valueSize;

		if (lineEndSize < 0) { valueSize = lps; lineEndSize = 4;
		} else { lineEndSize = 0; }

		{ // Writes
			fwrite (LP, 	 sizeof (c8),	strlen (LP),	stdout);
			putc   (symbol, stdout);

			SetConsoleTextAttribute (console, color);
			fwrite (key, 	 sizeof (c8), 	strlen (key), 	stdout);
			SetConsoleTextAttribute (console, 15);

			fwrite (NP, 	 sizeof (c8), 	nps, 			stdout);
			fwrite (": ", 	 sizeof (c8), 	2, 				stdout);
			fwrite (value, 	 sizeof (c16), 	valueSize, 		stdout);
			fwrite ("...\0", sizeof (c16), 	lineEndSize, 	stdout); // Shouldn't '\0' be after '\n' ?
			putc   ('\n', stdout);
		}
		
	}

	void Display (
		const HANDLE& console,
		const c8* const& value,
		const c8* const& key,
		const c8& symbol,
		const u8& color
	) {
		// Name Padding, Line Padding
		const c8 NP[] = { ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ' };
		const c8 LP[] = "    ";

		const u16 NAME_PADDING_SIZE = 18;
		const u16 LINE_PADDING_SIZE = 120;

		// When padding is negative change it to equal 0.
		s16 nps = NAME_PADDING_SIZE - strlen (key); nps *= (nps > 0);
		s16 lps = LINE_PADDING_SIZE - 2 - nps - strlen (key) - 1 - strlen (LP) - 3; 

		s16 valueSize = strlen (value);
		s16 lineEndSize = lps - valueSize;

		if (lineEndSize < 0) { valueSize = lps; lineEndSize = 4;
		} else { lineEndSize = 0; }

		{ // Writes
			fwrite (LP, 	 sizeof (c8),	strlen (LP),	stdout);
			putc   (symbol, stdout);

			SetConsoleTextAttribute (console, color);
			fwrite (key, 	 sizeof (c8), 	strlen (key), 	stdout);
			SetConsoleTextAttribute (console, 15);

			fwrite (NP, 	 sizeof (c8), 	nps, 			stdout);
			fwrite (": ", 	 sizeof (c8), 	2, 				stdout);
			fwrite (value, 	 sizeof (c8), 	valueSize, 		stdout);
			fwrite ("...\0", sizeof (c16), 	lineEndSize, 	stdout); // Shouldn't '\0' be after '\n' ?
			putc   ('\n', stdout);
		}
		
	}


	void Open (
		const u32& depth,
		c8** const& actions
	) {

		FILE* mainConfig = nullptr;
		INTERPRETER::Interpreter interpreter { 0 };
		u32 includesCounter = 0;
		u8 openType = OPEN_TYPE_LISTING;

		IO::Read (WINDOWS::REGISTRY::mainConfigFilePath, mainConfig);
		files.push_back (mainConfig);

		const u32 lastDepth = depth - 1;
		auto&& lastAction = actions[lastDepth];
		u16 lastActionLength = 0; 
		u32 spaceFiles = 0;

		{ // 1st READ

			{ // GLOBAL CONFIG
				// TODO: global.txt
				// We will be alwayas reading this file 1st at 1st read and 1st at second read.
				// and we want discard it's contents. Making it a global-data container that can be used across multiple modules
				// For example we can define all the "unity_dir", "vsc_dir", "godot_dir" and so on inside this file
				// and then only leave definitions inside the project-quail file itself.
			}

			{// MAIN CONFIG
				currentConfigFolderLength = WINDOWS::REGISTRY::topConfigsFolderPathLength;
				currentConfigFolder = WINDOWS::REGISTRY::mainConfigFilePath;

				GetIncludes (interpreter, includesCounter, mainConfig);
				spaceFiles = includesCounter; // Store information about current module files amount.
			}

			if (depth != 0) {

				for (u32 iDepth = 0; iDepth < lastDepth; ++iDepth) { // PROJECTS / SUBPROJECTS ONLY.
					auto&& command = actions[iDepth];

					u16 commandLength = 0; for (; command[commandLength] != TYPE_EOS; ++commandLength);
					ToLowCase (command, commandLength); // Conversion

					projectId = FindProject (command, commandLength); // Store information about what module we're in.

					if (projectId == projects.keys.size ()) {
						ERROR ("Could not match with any project.\n\n");
					} else { 	
						GetProjects (interpreter, includesCounter, projectId); 
						spaceFiles = projects.capes[projectId].special.includesCount; // Store information about current module files amount.
					}
				}

				{ // Has to be a either a project / subproject / command / queue.
					for (; lastAction[lastActionLength] != TYPE_EOS; ++lastActionLength);
					ToLowCase (lastAction, lastActionLength); // Conversion
					u32 index = FindProject (lastAction, lastActionLength);

					if (index == projects.keys.size ()) {

						LOGWARN ("We are looking for a command not a listing.\n\n");
						openType = OPEN_TYPE_COMMAND;

					} else { 
						projectId = index; 												// Store information about what module we're in.
						GetProjects (interpreter, includesCounter, index); 				// Get module's include files and read it's modules.
						spaceFiles = projects.capes[projectId].special.includesCount; 	// Store information about current module files amount.
					}
				}

				if (projectId != -1) { 

					auto&& pathLength = projects.capes[projectId].pathLength;
					auto&& path = (c16*) projects.paths[projectId];
					auto&& name = (c8*) projects.keys[projectId];
					u32 nameLength = 0; for (; name[nameLength] != '\0'; ++nameLength); ++nameLength; // + termination sign

					AddHiddenConstants (pathLength, path, nameLength, name);
				}

			}

		}

		{ // 2nd Read

			LOGINFO ("2nd Read\n");

			// By calculating so we only 2-read those files instead of the whole tree.
			//  Going from the last included file up to the sub/project file.
			//  -1 stands for the project file.
			const u32 projectFiles = files.size () - spaceFiles - 1;

			for (u32 iFile = files.size (); iFile > projectFiles; --iFile) {

				const auto& config = files[iFile - 1];
				rewind (config);

				INTERPRETER::parsingstage = INTERPRETER::MAIN::Main;
				interpreter.current = 0;
				interpreter.special = 0;

				ReadFile(config, interpreter);
				//while (interpreter.current != EOF) { // READ
				//	interpreter.current = getc (config);
				//	INTERPRETER::parsingstage (interpreter);
				//}

			}

			// DISPLAYS ALL CONSTANTS
			// for (s32 iConstant = 0; iConstant < constants.keys.size(); ++iConstant) {
			// 	const auto&& value = (c16*) constants.values[iConstant];
			// 	const auto&& key = (c8*) constants.keys[iConstant];
			// 	LOGINFO ("Constant: %s: %ls\n", key, value);
			// }

			// DISPLAYS ALL COMMANDS
			//LOGINFO ("WUT: %zd, %d\n", commands.keys.size(), spaceFiles - 1);
			//for (s32 iCommand = 0; iCommand < commands.keys.size(); ++iCommand) {
			//	const auto&& value = (c16*) commands.values[iCommand];
			//	const auto&& key = (c8*) commands.keys[iCommand];
			//	LOGINFO ("Command: %s: %ls\n", key, value);
			//}

			// Currently I am loading all constants, varaibles, secrets
			//  and that might not be very smart. Because doing so I am loading a lot of things that are not necessery needed.
			//  It would be much smarter to only load strings that are being used by a specific command or a queue of commands.
			//   - Its a different approach completly. We would look for a command then it's constants, varaibles, secrets
			//    and we would assemble the whole command part by part. However `Queues` and `Lists` would be much more difficult.
			//    It also does not seem to be a big optimalization too.

		}

		HANDLE console = GetStdHandle (STD_OUTPUT_HANDLE);

		if (openType) { LOGINFO ("Execution\n");

			u32 index = 0;

			{ // Find Command in Commands
				COMPARESEARCH::ArrayPartFirstMatchVector ( 
					lastAction, lastActionLength, sizeof (c8),
					index, 
					commands.keys.size (),
					(void**)(commands.keys.data ())
				);

				if (index == commands.keys.size ()) { // Not a command

					index = 0;

					COMPARESEARCH::ArrayPartFirstMatchVector ( 
						lastAction, lastActionLength, sizeof (c8),
						index, 
						queues.keys.size (),
						(void**)(queues.keys.data ())
					);

					if (index == queues.keys.size ()) { // Not a queue
						ERROR ("Not a valid command or queue!\n");
					} else {
						const auto&& queue = (c8*) queues.values[index];
						LOGINFO ("queue-id: [%d]: %s\n", index, queue);

						// 1. calculate amout of ',' characters
						// 2. add + 1 -> this is the number of commands we're be calling
						// 3. reserve an a array to hold ',' positions
						// 4. for each command do execution.

						auto&& buffor = (u32*) temporary;
						u32 substrings = 1, queueLength = 0;

						for (; queue[queueLength] != TYPE_EOS; ++queueLength) {
							if (queue[queueLength] == TYPE_SEPARATOR) {
								buffor[substrings] = queueLength + 1;
								++substrings;
							}
						}

						buffor[substrings] = queueLength + 1;
						buffor[0] = 0;

						for (u32 i = 0; i < substrings; ++i) {

							auto& start = buffor[i];
							auto&& command = queue + start;
							auto length = buffor[i + 1] - start - 1;

							LOGINFO ("queue-comamnd: [%d-%d]: %.*s.\n", i, length, length, command);

							index = 0;

							COMPARESEARCH::ArrayPartFirstMatchVector ( 
								command, length, sizeof (c8),
								index, 
								commands.keys.size (),
								(void**)(commands.keys.data ())
							);

							LOGINFO ("index: %d, %zd\n", index, commands.keys.size ());

							if (index == commands.keys.size ()) { // Not a queue
								LOGINFO ("Not a valid command or queue!\n");
							} else {
								const auto&& comm = (c16*) commands.values[index];
								LOGINFO ("id: [%d]: %ls\n", index, comm);
								DisplayQueueCmd (console, (c8*) commands.keys[index], TYPE_COMMAND, 11);
								_wsystem (comm);
							}
						}

					}

				} else {
					const auto&& command = (c16*) commands.values[index];
					LOGINFO ("id: [%d]: %ls\n", index, command);
					_wsystem (command);
				}
			}

		} else { LOGINFO ("Listing\n");

			// TODO
			// 2. List queues key and value.
			// 3. save previous color using. then restore. https://stackoverflow.com/questions/17125440/c-win32-console-color

			SetConsoleTextAttribute (console, 15);
			putc ('\n', stdout);

			// Display only subprojects of a project
			for (u32 i = projectsOffset; i < projects.keys.size (); ++i) {
				const auto&& value = (c16*) projects.paths[i];
				const auto&& key = (c8*) projects.keys[i];
				Display (console, value, key, TYPE_PROJECT, 14);
			}

			for (u32 i = 0; i < commands.keys.size (); ++i) {
				const auto&& value = (c16*) commands.values[i];
				const auto&& key = (c8*) commands.keys[i];
				Display (console, value, key, TYPE_COMMAND, 11);
			}

			for (u32 i = 0; i < queues.keys.size (); ++i) {
				const auto&& value = (c8*) queues.values[i];
				const auto&& key = (c8*) queues.keys[i];
				Display (console, value, key, TYPE_QUEUE, 12);
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

		for (s32 i = 0; i < queues.keys.size(); ++i) {
			FREE (queues.values[i]);
			FREE (queues.keys[i]);
		}

		for (u32 iFile = 0; iFile < files.size(); ++iFile) {
			IO::Close (files[iFile]);
		}

		// ? That should be somewhere else ?
		FREE (WINDOWS::REGISTRY::mainConfigFilePath);
	}

}
