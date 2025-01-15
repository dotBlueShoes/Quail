// Created 2024.11.05 by Matthew Strumiłło (dotBlueShoes)
//  LICENSE: GNU GENERAL PUBLIC LICENSE Version 3, 29 June 2007
//
#pragma once

#include <global/version.h>

#include <blue/windows/registry.hpp>
#include <blue/memory.hpp>
#include <blue/types.hpp>
#include <blue/log.hpp>

#include "configuration.hpp"
#include "resources.hpp"

namespace WINDOWS::REGISTRY {

	const c16 KEY_PATH_UNINSTALL_W[] 				= L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\Quail";

	const c16 PROPERTY_UNINSTALL_DISPLAY_ICON[]		= L"DisplayIcon";
	const c16 PROPERTY_UNINSTALL_DISPLAY_NAME[]		= L"DisplayName";
	const c16 PROPERTY_UNINSTALL_DISPLAY_VERSION[]	= L"DisplayVersion";
	const c16 PROPERTY_UNINSTALL_ESTIMATED_SIZE[]	= L"EstimatedSize";
	const c16 PROPERTY_UNINSTALL_NO_REPAIR[]		= L"NoRepair";
	const c16 PROPERTY_UNINSTALL_NO_MODIFY[]		= L"NoModify";
	const c16 PROPERTY_UNINSTALL_HELP_LINK[]		= L"HelpLink";
	const c16 PROPERTY_UNINSTALL_INSTALL_LOCATION[]	= L"InstallLocation";
	const c16 PROPERTY_UNINSTALL_PUBLISHER[]		= L"Publisher";
	const c16 PROPERTY_UNINSTALL_UNINSTALL_STRING[]	= L"UninstallString";
	const c16 PROPERTY_UNINSTALL_URL_INFO_ABOUT[]	= L"URLInfoAbout";
	const c16 PROPERTY_UNINSTALL_URL_UPDATE_INFO[]	= L"URLUpdateInfo";

	const c16 VALUE_UNINSTALL_DISPLAY_NAME[]		= L"Quail";
	const s32 VALUE_UNINSTALL_ESTIMATED_SIZE		= DISK_SPACE_KB;
	const s32 VALUE_UNINSTALL_NO_REPAIR				= 1;
	const s32 VALUE_UNINSTALL_NO_MODIFY				= 1;
	const c16 VALUE_UNINSTALL_HELP_LINK[]			= L"0";
	const c16 VALUE_UNINSTALL_PUBLISHER[]			= L".BlueShoes";
	const c16 VALUE_UNINSTALL_URL_INFO_ABOUT[]		= L"0";
	const c16 VALUE_UNINSTALL_URL_UPDATE_INFO[]		= L"0";


	const c16 KEY_PATH_W[]							= L"SOFTWARE\\dotBlueShoes\\Quail";
	const c16 PROPERTY_QUAIL_FILEPATH_W[]			= L"filepath";
	const c16 VALUE_DEFAULT_QUAIL_FOLDER_W[] 		= L"C:\\Program Files\\dotBlueShoes\\Quail";


	void CreateFiles (const u32& directoryPathLength, const c16* const& directoryPath) {
		c16* buffer;

		{ // Main Config
			{ // CONSTRUCT (Main Config Path)
				ALLOCATE (c16, buffer, directoryPathLength + CONFIGURATION::MAIN_CONFIG_LENGTH + 1);

				memcpy (buffer, directoryPath, directoryPathLength);
				buffer[(directoryPathLength / 2) - 1] = L'\\';
				memcpy (buffer + (directoryPathLength / 2), CONFIGURATION::MAIN_CONFIG_W, CONFIGURATION::MAIN_CONFIG_LENGTH);

				LOGINFO ("main config filepath: %ls\n", buffer);
			}

			if (!IO::IsExisting (buffer)) { IO::Create (buffer); }
			else { LOGWARN ("main config file arleady exists.\n"); }

			FREE (buffer);
		}

		{ // Global Config
			{ // CONSTRUCT (Global Config Path)
				ALLOCATE (c16, buffer, directoryPathLength + CONFIGURATION::GLOBAL_CONFIG_LENGTH + 1);

				memcpy (buffer, directoryPath, directoryPathLength);
				buffer[(directoryPathLength / 2) - 1] = L'\\';
				memcpy (buffer + (directoryPathLength / 2), CONFIGURATION::GLOBAL_CONFIG_W, CONFIGURATION::GLOBAL_CONFIG_LENGTH);

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
		
		const c16* keyPath = L"System\\CurrentControlSet\\Control\\Session Manager\\Environment";
		const c16* name = L"path";

		c16* env; ALLOCATE (c16, env, 2048);
		DWORD envSize = -1;
		
		HKEY key;

		// OPEN
    	auto error = RegOpenKeyExW (HKEY_LOCAL_MACHINE, keyPath, 0, KEY_ALL_ACCESS, &key);

    	if (error != ERROR_SUCCESS) {
			ERROR ("Could not open key at `%ls`\n", keyPath);
		}

		error = RegGetValueW (key, NULL, name, RRF_RT_ANY, NULL, env, &envSize); // GET

		if (error != ERROR_SUCCESS) {
			ERROR ("Could not get `%ls` value.\n", name);
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
    		error = RegSetValueExW (key, name, 0, REG_SZ, (LPBYTE)env, envSize);
			
			if (error != ERROR_SUCCESS) {
				ERROR ("Could not set `%ls` value with `%ls`\n", name, env);
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

		u32 uninstallerFilepathLength = filepathLength + CONFIGURATION::UNINSTALLER_LENGTH ;
		c16* uninstallerFilepath; ALLOCATE (c16, uninstallerFilepath, uninstallerFilepathLength);

		u32 quailFilepathLength = filepathLength + CONFIGURATION::QUAIL_LENGTH ;
		c16* quailFilepath; ALLOCATE (c16, quailFilepath, quailFilepathLength);

		{ // Construct UNINSTALLER
			memcpy (uninstallerFilepath, filepath, filepathLength - 2); 			// don't cpy '\0'
			wmemset (uninstallerFilepath + (filepathLength / 2) - 1, L'\\', 1); 	// emplace `\` sign
			memcpy (uninstallerFilepath + (filepathLength / 2), CONFIGURATION::UNINSTALLER_W, CONFIGURATION::UNINSTALLER_LENGTH);
			LOGINFO ("uninstaller filepath: %ls\n", uninstallerFilepath);
		}

		{ // Construct QUAIL
			memcpy (quailFilepath, filepath, filepathLength - 2); 					// don't cpy '\0'
			wmemset (quailFilepath + (filepathLength / 2) - 1, L'\\', 1); 			// emplace `\` sign
			memcpy (quailFilepath + (filepathLength / 2), CONFIGURATION::QUAIL_W, CONFIGURATION::QUAIL_LENGTH);
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
		}

	}

}