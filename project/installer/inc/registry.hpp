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


	//void AddContentToMainConfig (FILE*& file) {
	//	DWORD bytesWritten;
	//	BOOL result;
	//
    //	result = WriteFile (
    //	    file,                // Handle to the file
    //	    data,                 // Pointer to the data to write
    //	    (DWORD)strlen(data),  // Number of bytes to write
    //	    &bytesWritten,        // Number of bytes written
    //	    NULL                  // No overlapped structure
    //	);
	//
    //	if (!result) {
    //	    printf ("Failed to write to the file. Error code: %lu\n", GetLastError());
    //	    CloseHandle (file);
    //	    return;
    //	}
	//
    //	printf ("Successfully written to the file.\n");
	//
    //	// Close the file handle
    //	CloseHandle (file);
	//}

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

			if (!IO::IsExisting (buffer)) { 

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
					LOGINFO ("FINAL CONTENT: \n%ls\n\n", contetnt);
				}

				IO::CreateAdd (buffer, contetnt);

			} else { LOGWARN ("main config file already exists.\n"); }

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

			if (!IO::IsExisting (buffer)) { IO::CreateEmpty (buffer); }
			else { LOGWARN ("global config file already exists.\n"); }

			FREE (buffer);
		}
	}


	void AddQuailToPath (
		const u16& quailLength,
		const c16* const& quail
	) {

		c16* env; ALLOCATE (c16, env, 2048);
		DWORD envSize = -1;
		
		LSTATUS errorCode;
		HKEY key;

		// OPEN
    	errorCode = RegOpenKeyExW (HKEY_LOCAL_MACHINE, KEY_ENVIRONMENT_VARIABLES_W, 0, KEY_ALL_ACCESS, &key);
    	if (errorCode != ERROR_SUCCESS) { ERROR ("Could not open key at `%ls`\n", KEY_ENVIRONMENT_VARIABLES_W); }

		errorCode = RegGetValueW (key, NULL, PROPERTY_PATH_W, RRF_RT_ANY, NULL, env, &envSize); // GET
		if (errorCode != ERROR_SUCCESS) { ERROR ("Could not get `%ls` value.\n", PROPERTY_PATH_W); }

		if (wcsstr (env, quail) != nullptr) {
			LOGWWARN ("Environment Variable `PATH` entry for Quail already exists.\n");
		} else {
			LOGWINFO ("Creating new entry in Environment Variable `PATH` for Quail.\n");

			{ // Creating proper entry string representing Quail.
				auto&& begin = env + (envSize / 2) - 1; // - '\0' sign.

				memcpy  (begin, quail, quailLength);	// Add Quail
				wmemset (begin + (quailLength / 2) - 1, L';', 1);
				wmemset (begin + (quailLength / 2), L'\0', 1);

				envSize = envSize + quailLength + 1;
			
				//LOGWINFO ("`Path:` [%d]: %s\n", envSize, env);
			}
			
			
			{ // Adding the entry to the path variable.
				errorCode = RegSetValueExW (key, PROPERTY_PATH_W, 0, REG_SZ, (LPBYTE)env, envSize);
				if (errorCode != ERROR_SUCCESS) { ERROR ("Could not set `%ls` value with `%ls`\n", PROPERTY_PATH_W, env); }

				// Update all other applications because we did just edited environment variables!
				SendMessageTimeoutW (HWND_BROADCAST, WM_SETTINGCHANGE, 0, (LPARAM)"Environment", SMTO_BLOCK, 100, NULL);
			}
    		
			LOGWINFO ("Successfully added Quail to `Path` environment variable.\n");
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
					case REG_OPENED_EXISTING_KEY: 	LOGWWARN 	("Key already exists.\n"); 			break;
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
					case REG_OPENED_EXISTING_KEY: 	LOGWWARN 	("Key already exists.\n"); 			break;
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
