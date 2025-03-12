// Created 2024.11.05 by Matthew Strumiłło (dotBlueShoes)
//  LICENSE: GNU GENERAL PUBLIC LICENSE Version 3, 29 June 2007
//
#pragma once
#include <blue/types.hpp>
#include <blue/io.hpp>
//
#include "interpreter.hpp"
#include "display.hpp"
#include "data.hpp"

//  ABOUT
// 1st read.
//  a) We're reading for modules (projects) & includes for "GLOBAL" space
//   > Data that is available globally across all projects.
//  b) We're reading for modules (projects) & includes for "MAIN" space
//   > It's the ROOT of all our projects definitions. Also contains commands and other.
//  c) We're then reading all cascading includes of both global and main space.
//  d) Based on the 'depth' parameter we're either:
//    1a. '0'  We're only listing the main config.
//    1b. '1>' We're either listing a module or running a command with GLOBAL data.
// 2nd read.

namespace OPEN {

	void ReadFile (
		IN  	FILE* const& 				file,
		IN  	INTERPRETER::Interpreter& 	interpreter
	) {

		//  LOGIC
		// 'INTERPRETER::parsingstage' is a jump table like function pointer
		//  This concept helps us define a starting stage for each read thing
		//  meaning we can easily disqualify certain strings if they're invalid.
		// 

		while (interpreter.current != EOF) {
			interpreter.current = getc (file);
			INTERPRETER::parsingstage (interpreter);
		}
	}


	void ReadIncludes (
		IN  	INTERPRETER::Interpreter& 	interpreter,
		INOUT 	u32& 						includesCounter,
		IN  	FILE*& 						parentFile
	) {

		INTERPRETER::parsingstage = INTERPRETER::MAIN::GetAllFiles;
		ReadFile (parentFile, interpreter);

		//  LOGIC
		// Within the first include read there might be links to other includes.
		// 'includes.size()' grows which each include detection.
		// Therefore we keep a 'includesCounter' variable to match every read
		// if all the includes in the tree have been read or not.
		//

		while (includesCounter != includes.size ()) {

			auto&& string = (c16*)(includes[includesCounter]);

			FILE* config = nullptr;
			IO::Read (string, config);
			files.push_back (config);

			INTERPRETER::parsingstage = INTERPRETER::MAIN::GetAllFiles;
			interpreter.current = 0;
			interpreter.special = 0;

			ReadFile (config, interpreter);

			LOGWINFO (" >> IncludeFile [%d]:`%s` read successfully\n", includesCounter, string);

			++includesCounter;
		}

	}


	auto FindProject (
		IN 		const c8* const& 	command,
		IN 		const u16& 			commandLength
	) {
		u32 index = 0;

		COMPARESEARCH::ArrayPartFirstMatchVectorLowCase ( 
			command, commandLength, sizeof (c8),
			index, 
			projects.keys.size () - projectsOffset,
			(void**)(projects.keys.data () + projectsOffset)
		);

		index += projectsOffset; // Add what we removed. We offsetted it before to search it the right way.
		projectsOffset = projects.keys.size ();

		return index;
	}
	

	void ReadProject (
		IN 		INTERPRETER::Interpreter& 	interpreter,
		INOUT 	u32& 						includesCounter,
		IN 		const u32& 					index
	) {

		const auto& configLength = projects.capes[index].configLength;
		const auto& pathLength = projects.capes[index].pathLength;

		const u32 configFilePathLength = (pathLength + configLength) / 2;
		const auto&& configFilePath = (c16*) (projects.configs[index]);
				
		LOGINFO (" > ProjectFile [%d]:`%ls` read successfully\n", index, configFilePath);

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

			ReadIncludes (interpreter, includesCounter, config);

			{ // Store the amount of include files a project has.
				projects.capes[index].special.includesCount = includesCounter - projectIncludesCounter;
			}

		}

	}


	void Open (
		IN 		const u32& 	depth,
		IN 		c8** const& actions
	) {

		INTERPRETER::Interpreter interpreter;

		u8 openType 		= OPEN_TYPE_LISTING;
		FILE* globalConfig 	= nullptr;
		FILE* mainConfig 	= nullptr;
		u32 includesCounter = 0;

		{ // Read Quail Configs.

			// GLOBAL config is not put into "files" list because we want to
			//  always read it first.
			IO::Read (CONFIG::configGlobalFilePath, globalConfig);
			IO::Read (CONFIG::configMainFilePath, mainConfig);

		}

		const u32 lastDepth = depth - 1;
		auto&& lastAction = actions[lastDepth];
		u16 lastActionLength = 0; 

		u32 globalFiles = 0;	// Point how many global files are read.
		u32 spaceFiles = 0; 	// Point how many files are from selected project.

		// ISSUE. This is not perfect i believe as it's vectors underneath.
		MEMORY::EXIT::PUSH (nullptr, OPEN::Destroy);

		{ // 1st READ

			LOGINFO ("[ 1ST READ ]\n");

			// At first we read config files from within Quail directory. -> main, global and their includes.
			currentConfigFolderLength = CONFIG::topConfigsFolderLength;
			currentConfigFolder = CONFIG::topConfigsFolder;

			{ // GLOBAL CONFIG
				interpreter.current = 0;
				interpreter.special = 0;

				ReadIncludes (interpreter, includesCounter, globalConfig);

				globalFiles = includesCounter;
			}

			{ // MAIN CONFIG
				interpreter.current = 0;
				interpreter.special = 0;

				files.push_back (mainConfig);
				ReadIncludes (interpreter, includesCounter, mainConfig);

				spaceFiles = includesCounter;
			}

			if (depth != 0) {

				for (u32 iDepth = 0; iDepth < lastDepth; ++iDepth) { // PROJECTS / SUBPROJECTS ONLY.
					auto&& command = actions[iDepth];

					u16 commandLength = 0; for (; command[commandLength] != TYPE_EOS; ++commandLength);
					ToLowCase (command, commandLength); // Conversion

					projectId = FindProject (command, commandLength); // Store information about what module we're in.

					
					if (projectId == projects.keys.size ()) {
						// Triggers when 'quail -o 'invalid' anything_else...'.
						ERROR ("Could not match string with any project." ERROR_NEW_LINE);
					} else { 
						ReadProject (interpreter, includesCounter, projectId); 
						spaceFiles = projects.capes[projectId].special.includesCount; // Store information about current module files amount.
					}
				}

				{ // Has to be a either a project / subproject / command / queue.
					for (; lastAction[lastActionLength] != TYPE_EOS; ++lastActionLength);
					ToLowCase (lastAction, lastActionLength); // Conversion
					u32 index = FindProject (lastAction, lastActionLength);

					if (index == projects.keys.size ()) {

						LOGINFO (" > Not a project - Looking for a command not a listing.\n");
						openType = OPEN_TYPE_COMMAND;

					} else { 
						projectId = index; 												// Store information about what module we're in.
						ReadProject (interpreter, includesCounter, index); 			// Get module's include files and read it's modules.
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

		{ // 2ND READ

			LOGINFO ("[ 2ND READ ]\n");

			//  ABOUT
			// This read is inverse it goes from the last file to the first (if necessary).
			//

			//  CHECK
			// 'global' files are not available for main config).
			if (projectId != PROJECT_MAIN_ID) { 

				//  LOGIC
				// Iterate through GLOBAL INCLUDES are read them.
				for (u32 iFile = globalFiles; iFile > 0; --iFile) {

					if (iFile == 0) exit (1);
					const auto& config = files[iFile - 1];
					
					if (config == mainConfig) {
						LOGINFO ("OHH GOD!!!\n");
					} else {
						rewind (config);
					
						INTERPRETER::parsingstage = INTERPRETER::MAIN::Main;
						interpreter.current = 0;
						interpreter.special = 0;

						ReadFile (config, interpreter);
					}
				}

				//  LOGIC
				// Finally read the global file itself.
				{
					rewind (globalConfig);

					INTERPRETER::parsingstage = INTERPRETER::MAIN::Main;
					interpreter.current = 0;
					interpreter.special = 0;

					ReadFile (globalConfig, interpreter);
				}
				
			}

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

				ReadFile (config, interpreter);

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

			// Currently I am loading all constants, variables, secrets
			//  and that might not be very smart. Because doing so I am loading a lot of things that are not necessary needed.
			//  It would be much smarter to only load strings that are being used by a specific command or a queue of commands.
			//   - Its a different approach completely. We would look for a command then it's constants, variables, secrets
			//    and we would assemble the whole command part by part. However `Queues` and `Lists` would be much more difficult.
			//    It also does not seem to be a big optimalization too.

		}

		HANDLE console = GetStdHandle (STD_OUTPUT_HANDLE);

		if (openType) { LOGINFO (" > execution...\n");

			{ // Find Command in Commands

				u32 index = 0; // Match for command.
				COMPARESEARCH::ArrayPartFirstMatchVector ( 
					lastAction, lastActionLength, sizeof (c8),
					index, 
					commands.keys.size (),
					(void**)(commands.keys.data ())
				);

				if (index == commands.keys.size ()) { // Not a command

					index = 0; // Match for queue.
					COMPARESEARCH::ArrayPartFirstMatchVector ( 
						lastAction, lastActionLength, sizeof (c8),
						index, 
						queues.keys.size (),
						(void**)(queues.keys.data ())
					);

					if (index == queues.keys.size ()) { // Not a queue either

						// Triggers when 'quail -o 'invalid''.
						// Triggers when 'quail -o 'valid' 'invalid''.
						ERROR ("Could not match string with a command nor a queue." ERROR_NEW_LINE);

					} else {

						const auto&& queueValue = (c8*) queues.values[index];
						const auto&& queueKey = (c8*) queues.keys[index];

						// 1. calculate amount of ',' characters
						// 2. add + 1 -> this is the number of commands we're be calling
						// 3. reserve an a array to hold ',' positions
						// 4. for each command do execution.

						auto&& buffer = (u32*) temporary;
						u32 substrings = 1, queueLength = 0;

						for (; queueValue[queueLength] != TYPE_EOS; ++queueLength) {
							if (queueValue[queueLength] == TYPE_SEPARATOR) {
								buffer[substrings] = queueLength + 1;
								++substrings;
							}
						}

						buffer[substrings] = queueLength + 1;
						buffer[0] = 0;

						for (u32 i = 0; i < substrings; ++i) {

							auto& start = buffer[i];
							auto&& command = queueValue + start;
							auto length = buffer[i + 1] - start - 1;

							index = 0;

							COMPARESEARCH::ArrayPartFirstMatchVector ( 
								command, length, sizeof (c8),
								index, 
								commands.keys.size (),
								(void**)(commands.keys.data ())
							);

							if (index == commands.keys.size ()) { 

								// Invalid command-key as queue-value.
								ERROR ("Command '%s' inside Queue '%s' does not exist." ERROR_NEW_LINE, 
									command, queueKey
								);

							} else {

								const auto&& comm = (c16*) commands.values[index];
								DISPLAY::QueueProgress (console, (c8*) commands.keys[index], TYPE_COMMAND, 11);
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

		} else { LOGINFO (" > listing ...\n");

			SetConsoleTextAttribute (console, 15);
			putc ('\n', stdout);

			for (u32 i = projectsOffset; i < projects.keys.size (); ++i) {
				const auto&& value = (c16*) projects.paths[i];
				const auto&& key = (c8*) projects.keys[i];

				DISPLAY::KeyValue<c8, c16> (
					console, key, strlen (key), value, wcslen (value), TYPE_PROJECT, 14
				);
			}

			for (u32 i = 0; i < commands.keys.size (); ++i) {
				const auto&& value = (c16*) commands.values[i];
				const auto&& key = (c8*) commands.keys[i];

				DISPLAY::KeyValue<c8, c16> (
					console, key, strlen (key), value, wcslen (value), TYPE_COMMAND, 11
				);
			}

			for (u32 i = 0; i < queues.keys.size (); ++i) {
				const auto&& value = (c8*) queues.values[i];
				const auto&& key = (c8*) queues.keys[i];

				DISPLAY::KeyValue<c8, c8>  (
					console, key, strlen (key), value, strlen (value), TYPE_QUEUE, 12
				);
			}

			SetConsoleTextAttribute (console, 7);
			putc ('\n', stdout);

		}

		OPEN::Destroy (nullptr);

	}

}
