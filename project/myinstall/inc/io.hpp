// Created 2024-01-15 by Matthew Strumiłło (dotBlueShoes)
//  LICENSE: GNU GENERAL PUBLIC LICENSE Version 3, 29 June 2007
//
#pragma once
#include <global/config.hpp>
#include <blue/error.hpp>
#include <blue/io.hpp>
//
namespace IO {

	void CreateFiles (
		const u32& directoryPathLength, 
		const c16* const& directoryPath,
		const bool& isBatch
	) {
		c16* buffer;

		{ // Main Config

			{ // CONSTRUCT (Main Config Path)
				ALLOCATE (c16, buffer, directoryPathLength + CONFIG::CONFIG_MAIN_LENGTH + 1);

				memcpy (buffer, directoryPath, directoryPathLength);
				buffer[(directoryPathLength / 2) - 1] = L'\\';
				memcpy (buffer + (directoryPathLength / 2), CONFIG::CONFIG_MAIN_W, CONFIG::CONFIG_MAIN_LENGTH);

				LOGINFO ("main config filepath: %ls\n", buffer);
			}

			BRANCH if (!IO::IsExisting (buffer)) {

				const u32 length = 
					CONFIG::DEFAULT_CONFIG_MAIN_1_LENGTH + 
					CONFIG::topConfigsFolderLength + 
					CONFIG::DEFAULT_CONFIG_MAIN_2_LENGTH
				;

				c16* contetnt; ALLOCATE (c16, contetnt, length);

				{ // CONSTRUCT
					memcpy (contetnt, DEFAULT_CONFIG_MAIN_1, CONFIG::DEFAULT_CONFIG_MAIN_1_LENGTH);
					memcpy (contetnt + (CONFIG::DEFAULT_CONFIG_MAIN_1_LENGTH / 2), CONFIG::topConfigsFolder, CONFIG::topConfigsFolderLength - 2);
					memcpy (contetnt + (CONFIG::DEFAULT_CONFIG_MAIN_1_LENGTH + CONFIG::topConfigsFolderLength - 2) / 2, DEFAULT_CONFIG_MAIN_2, CONFIG::DEFAULT_CONFIG_MAIN_2_LENGTH);
					//LOGINFO ("FINAL CONTENT: \n%ls\n\n", contetnt);
				}

				IO::CreateAdd (buffer, contetnt);

				FREE (contetnt);

			} else { LOGWARN ("Main config file already exists.\n"); }

			FREE (buffer);

		}

		{ // Global Config

			{ // CONSTRUCT (Global Config Path)
				ALLOCATE (c16, buffer, directoryPathLength + CONFIG::CONFIG_GLOBAL_LENGTH + 1);

				memcpy (buffer, directoryPath, directoryPathLength);
				buffer[(directoryPathLength / 2) - 1] = L'\\';
				memcpy (buffer + (directoryPathLength / 2), CONFIG::CONFIG_GLOBAL_W, CONFIG::CONFIG_GLOBAL_LENGTH);

				LOGINFO ("global config filepath: %ls\n", buffer);
			}

			BRANCH if (!IO::IsExisting (buffer)) { IO::CreateAdd (buffer, DEFAULT_CONFIG_GLOBAL); }
			else { LOGWARN ("Global config file already exists.\n"); }

			FREE (buffer);

		}

		if (isBatch) { // Executable Shortcut

			{ // CONSTRUCT (Global Config Path)
				ALLOCATE (c16, buffer, directoryPathLength + CONFIG::CONFIG_GLOBAL_LENGTH + 1);

				memcpy (buffer, directoryPath, directoryPathLength);
				buffer[(directoryPathLength / 2) - 1] = L'\\';
				memcpy (buffer + (directoryPathLength / 2), CONFIG::QUAIL_SHOURTCUT_NAME, CONFIG::QUAIL_SHOURTCUT_NAME_LENGTH);
				LOGINFO ("executable shortcut filepath: %ls\n", buffer);
				
			}

			BRANCH if (!IO::IsExisting (buffer)) { 

				IO::CreateAdd (buffer, CONFIG::QUAIL_SHOURTCUT_CONTENT);

			} else { LOGWARN ("Executable shortcut file already exists.\n"); }

			FREE (buffer);

		}
	}

}
