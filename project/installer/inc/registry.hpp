// Created 2024.11.05 by Matthew Strumiłło (dotBlueShoes)
//  LICENSE: GNU GENERAL PUBLIC LICENSE Version 3, 29 June 2007
//
#pragma once
#include <global/windows/registry.hpp>

#include <blue/windows/registry.hpp>
#include <blue/memory.hpp>
#include <blue/types.hpp>
#include <blue/log.hpp>

// TODO MOVE!
#include "resources.hpp"

namespace WINDOWS::REGISTRY {

	const c16 VALUE_UNINSTALL_DISPLAY_NAME			[] = L"Quail";
	const s32 VALUE_UNINSTALL_ESTIMATED_SIZE		= DISK_SPACE_KB;
	const s32 VALUE_UNINSTALL_NO_REPAIR				= 1;
	const s32 VALUE_UNINSTALL_NO_MODIFY				= 1;
	const c16 VALUE_UNINSTALL_HELP_LINK				[] = L"0";
	const c16 VALUE_UNINSTALL_PUBLISHER				[] = L".BlueShoes";
	const c16 VALUE_UNINSTALL_URL_INFO_ABOUT		[] = L"0";
	const c16 VALUE_UNINSTALL_URL_UPDATE_INFO		[] = L"0";


	void CreateFiles (const u32& directoryPathLength, const c16* const& directoryPath) {
		c16* buffer;

		{ // Main Config
			{ // CONSTRUCT (Main Config Path)
				ALLOCATE (c16, buffer, directoryPathLength + CONFIG::CONFIG_MAIN_LENGTH + 1);

				memcpy (buffer, directoryPath, directoryPathLength);
				buffer[(directoryPathLength / 2) - 1] = L'\\';
				memcpy (buffer + (directoryPathLength / 2), CONFIG::CONFIG_MAIN_W, CONFIG::CONFIG_MAIN_LENGTH);

				LOGINFO ("main config filepath: %ls\n", buffer);
			}

			if (!IO::IsExisting (buffer)) { IO::Create (buffer); }
			else { LOGWARN ("main config file arleady exists.\n"); }

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

			if (!IO::IsExisting (buffer)) { IO::Create (buffer); }
			else { LOGWARN ("global config file arleady exists.\n"); }

			FREE (buffer);
		}
	}


	void AddQuailToPath (
		const u16& quailLength,
		const c16* const& quail
	) {

		c16* env; ALLOCATE (c16, env, 2048);
		DWORD envSize = -1;
		
		HKEY key;

		// OPEN
    	auto error = RegOpenKeyExW (HKEY_LOCAL_MACHINE, KEY_ENVIROMENT_VARIABLES_W, 0, KEY_ALL_ACCESS, &key);

    	if (error != ERROR_SUCCESS) {
			ERROR ("Could not open key at `%ls`\n", KEY_ENVIROMENT_VARIABLES_W);
		}

		error = RegGetValueW (key, NULL, PROPERTY_PATH_W, RRF_RT_ANY, NULL, env, &envSize); // GET

		if (error != ERROR_SUCCESS) {
			ERROR ("Could not get `%ls` value.\n", PROPERTY_PATH_W);
		} 

		//LOGWINFO ("`Path:` [%d]: %s\n", envSize, env);

		if (wcsstr (env, quail) != NULL) {
			LOGWWARN ("Enviroment Variable `PATH` entry for Quail arleady exists.\n");
		} else {
			LOGWINFO ("Creating new entry in Enviroment Variable `PATH` for Quail.\n");

			{ // Creating proper entry string representing Quail.
				auto&& binary = ((u8*) env) + envSize; //642;
				auto&& begin = (c16*) binary - 2;
			
				wmemset (begin + 0, L';', 1);					// Replace `\0` with `;`.
				memcpy  (begin + 1, quail, quailLength);		// Add Quail
				wmemset (begin + (quailLength / 2), L'\0', 1); 	// Replace `\\` with `\0`.
			
				envSize = envSize + quailLength + 1;
			
				LOGWINFO ("`Path:` [%d]: %s\n", envSize, env);
			}
			
			// Adding the entry to the path variable.
    		error = RegSetValueExW (key, PROPERTY_PATH_W, 0, REG_SZ, (LPBYTE)env, envSize);
			
			if (error != ERROR_SUCCESS) {
				ERROR ("Could not set `%ls` value with `%ls`\n", PROPERTY_PATH_W, env);
			}
			
			// Update all other applications because we did just edited enviroment varaibles!
			SendMessageTimeoutW (HWND_BROADCAST, WM_SETTINGCHANGE, 0, (LPARAM)"Environment", SMTO_BLOCK, 100, NULL);
			LOGWINFO ("Successfully added Quail to `Path` enviroment varaible.\n");
		}

		// FREE
    	RegCloseKey (key);
		FREE (env);

	}


	void CreateKeys (const u32& filepathLength, const c16* const& filepath) {

		HKEY key; LSTATUS error; DWORD status;

		u32 uninstallerFilepathLength = filepathLength + CONFIG::UNINSTALLER_NAME_LENGTH ;
		c16* uninstallerFilepath; ALLOCATE (c16, uninstallerFilepath, uninstallerFilepathLength);

		u32 quailFilepathLength = filepathLength + CONFIG::EXECUTABLE_NAME_LENGTH ;
		c16* quailFilepath; ALLOCATE (c16, quailFilepath, quailFilepathLength);

		{ // Construct UNINSTALLER
			memcpy (uninstallerFilepath, filepath, filepathLength - 2); 			// don't cpy '\0'
			wmemset (uninstallerFilepath + (filepathLength / 2) - 1, L'\\', 1); 	// emplace `\` sign
			memcpy (uninstallerFilepath + (filepathLength / 2), CONFIG::UNINSTALLER_NAME, CONFIG::UNINSTALLER_NAME_LENGTH);
			LOGINFO ("uninstaller filepath: %ls\n", uninstallerFilepath);
		}

		{ // Construct QUAIL
			memcpy (quailFilepath, filepath, filepathLength - 2); 					// don't cpy '\0'
			wmemset (quailFilepath + (filepathLength / 2) - 1, L'\\', 1); 			// emplace `\` sign
			memcpy (quailFilepath + (filepathLength / 2), CONFIG::EXECUTABLE_NAME, CONFIG::EXECUTABLE_NAME_LENGTH);
			LOGINFO ("quail filepath: %ls\n", quailFilepath);
		}

		{ // Quail Key
			CreateKeyMachine (key, error, status, KEY_PATH_W);

			{ // STATUS
				if (error != ERROR_SUCCESS) 		ERROR 		("Creating key failed.\n\n");
				switch (status) {
					case REG_CREATED_NEW_KEY: 		LOGINFO 	("Key successfully created.\n"); 	break;
					case REG_OPENED_EXISTING_KEY: 	LOGWWARN 	("Key arleady exists.\n"); 			break;
					default: 						ERROR 		("Unknown key-status\n\n");
				}
			}

			CreatePropertyC16 (key, error, PROPERTY_QUAIL_FILEPATH_W, filepath, filepathLength);
			CHECK_PROPERTY (error, PROPERTY_QUAIL_FILEPATH_W);

			RegCloseKey (key);
		}

		{ // Uninstaller Key
			CreateKeyMachine (key, error, status, KEY_PATH_UNINSTALL_W);

			{ // STATUS
				if (error != ERROR_SUCCESS) 		ERROR 		("Creating key failed.\n\n");
				switch (status) {
					case REG_CREATED_NEW_KEY: 		LOGINFO 	("Key successfully created.\n"); 	break;
					case REG_OPENED_EXISTING_KEY: 	LOGWWARN 	("Key arleady exists.\n"); 			break;
					default: 						ERROR 		("Unknown key-status\n\n");
				}
			}

			CreatePropertyC16 (key, error, PROPERTY_UNINSTALL_DISPLAY_ICON, quailFilepath, quailFilepathLength);
			CHECK_PROPERTY (error, PROPERTY_UNINSTALL_DISPLAY_ICON);

			CreatePropertyC16 (key, error, PROPERTY_UNINSTALL_DISPLAY_NAME, VALUE_UNINSTALL_DISPLAY_NAME, sizeof (VALUE_UNINSTALL_DISPLAY_NAME));
			CHECK_PROPERTY (error, PROPERTY_UNINSTALL_DISPLAY_NAME);

			CreatePropertyC16 (key, error, PROPERTY_UNINSTALL_DISPLAY_VERSION, QUAIL_FULL_VERSION_W, sizeof (QUAIL_FULL_VERSION_W));
			CHECK_PROPERTY (error, PROPERTY_UNINSTALL_DISPLAY_VERSION);

			CreatePropertyS32 (key, error, PROPERTY_UNINSTALL_ESTIMATED_SIZE, DISK_SPACE_KB);
			CHECK_PROPERTY (error, PROPERTY_UNINSTALL_ESTIMATED_SIZE);

			CreatePropertyS32 (key, error, PROPERTY_UNINSTALL_NO_REPAIR, 1);
			CHECK_PROPERTY (error, PROPERTY_UNINSTALL_NO_REPAIR);

			CreatePropertyS32 (key, error, PROPERTY_UNINSTALL_NO_MODIFY, 1);
			CHECK_PROPERTY (error, PROPERTY_UNINSTALL_NO_MODIFY);

			CreatePropertyC16 (key, error, PROPERTY_UNINSTALL_HELP_LINK, VALUE_UNINSTALL_HELP_LINK, sizeof (VALUE_UNINSTALL_HELP_LINK));
			CHECK_PROPERTY (error, PROPERTY_UNINSTALL_HELP_LINK);

			CreatePropertyC16 (key, error, PROPERTY_UNINSTALL_INSTALL_LOCATION, filepath, filepathLength);
			CHECK_PROPERTY (error, PROPERTY_UNINSTALL_INSTALL_LOCATION);

			CreatePropertyC16 (key, error, PROPERTY_UNINSTALL_PUBLISHER, VALUE_UNINSTALL_PUBLISHER, sizeof (VALUE_UNINSTALL_PUBLISHER));
			CHECK_PROPERTY (error, PROPERTY_UNINSTALL_PUBLISHER);

			CreatePropertyC16 (key, error, PROPERTY_UNINSTALL_UNINSTALL_STRING, uninstallerFilepath, uninstallerFilepathLength);
			CHECK_PROPERTY (error, PROPERTY_UNINSTALL_UNINSTALL_STRING);

			CreatePropertyC16 (key, error, PROPERTY_UNINSTALL_URL_INFO_ABOUT, VALUE_UNINSTALL_URL_INFO_ABOUT, sizeof (VALUE_UNINSTALL_URL_INFO_ABOUT));
			CHECK_PROPERTY (error, PROPERTY_UNINSTALL_URL_INFO_ABOUT);

			CreatePropertyC16 (key, error, PROPERTY_UNINSTALL_URL_UPDATE_INFO, VALUE_UNINSTALL_URL_UPDATE_INFO, sizeof (VALUE_UNINSTALL_URL_UPDATE_INFO));
			CHECK_PROPERTY (error, PROPERTY_UNINSTALL_URL_UPDATE_INFO);

			RegCloseKey (key);

			FREE (uninstallerFilepath);
			FREE (quailFilepath);
		}

	}

}