// Created 2024.11.05 by Matthew Strumiłło (dotBlueShoes)
//
#pragma once
#include <blue/memory.hpp>
#include <blue/types.hpp>
#include <blue/log.hpp>

namespace REGISTRY {

	// PROPERTIES

	const c16 REGISTRY_PATH_W[]			= L"SOFTWARE\\dotBlueShoes\\Quail";
	const c16 PROPERTY_FILEPATH_W[]		= L"filepath";

	const c16 DEFAULT_FILEPATH_W[] 		= L"C:\\Program Files\\dotBlueShoes\\Quail\\config.txt";
	const u32 DEFAULT_FILEPATH_LENGTH	= sizeof (DEFAULT_FILEPATH_W);

	const c16 DEFAULT_FOLDERPATH_W[] 	= L"C:\\Program Files\\dotBlueShoes\\Quail";
	const u32 DEFAULT_FOLDERPATH_LENGTH	= sizeof (DEFAULT_FOLDERPATH_W);

	const c16 MAIN_CONFIG_W[]			= L"config.txt";
	const u32 MAIN_CONFIG_LENGTH		= sizeof (MAIN_CONFIG_W);

	const c16 GLOBAL_CONFIG_W[]			= L"global.txt";
	const u32 GLOBAL_CONFIG_LENGTH		= sizeof (GLOBAL_CONFIG_W);

	// VARIABLES

	u32 topConfigsFolderPathLength;		// Where are quail top configs.
	c16* topConfigsFolderPath;

	// FUNCTIONS

	//void ReplaceFolderPathWithFilePath (
	//	const c16* const& folderPath,
	//	const u32& folderPathLength
	//) {
	//	const u32 configFilepathLength = folderPathLength + 1 + REGISTRY::CONFIG_LENGTH;
	//	c16* configFilepath; ALLOCATE (c16, configFilepath, configFilepathLength);// = (c16*) malloc (configFilepathLength); // ALLOCATION
	//
	//	{ // CONSTRUCT
	//		memcpy (configFilepath, folderPath, folderPathLength);
	//		configFilepath[(folderPathLength / 2) - 1] = L'\\';
	//		memcpy (configFilepath + (folderPathLength / 2), REGISTRY::CONFIG_W, REGISTRY::CONFIG_LENGTH);
	//	}
	//
	//	{ // FREE AND SWAP
	//		FREE (mainConfigFilePath);
	//		mainConfigFilePathLength = configFilepathLength;
	//		mainConfigFilePath = configFilepath;
	//	}
	//}

	void CreateFiles (const u32& directoryPathLength, const c16* const& directoryPath) {
		c16* buffer;

		{ // Main Config
			{ // CONSTRUCT (Main Config Path)
				ALLOCATE (c16, buffer, directoryPathLength + REGISTRY::MAIN_CONFIG_LENGTH + 1);

				memcpy (buffer, directoryPath, directoryPathLength);
				buffer[(directoryPathLength / 2) - 1] = L'\\';
				memcpy (buffer + (directoryPathLength / 2), REGISTRY::MAIN_CONFIG_W, REGISTRY::MAIN_CONFIG_LENGTH);

				LOGINFO ("main config filepath: %ls\n", buffer);
			}

			if (!IO::IsExisting (buffer)) { IO::Create (buffer); }
			else { LOGWARN ("main config file arleady exists.\n"); }

			FREE (buffer);
		}

		{ // Global Config
			{ // CONSTRUCT (Global Config Path)
				ALLOCATE (c16, buffer, directoryPathLength + REGISTRY::GLOBAL_CONFIG_LENGTH + 1);

				memcpy (buffer, directoryPath, directoryPathLength);
				buffer[(directoryPathLength / 2) - 1] = L'\\';
				memcpy (buffer + (directoryPathLength / 2), REGISTRY::GLOBAL_CONFIG_W, REGISTRY::GLOBAL_CONFIG_LENGTH);

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
			
				wmemset (begin + 0, L';', 2);					// Replace `\0` with `;`.
				memcpy  (begin + 1, quail, quailLength);		// Add Quail
				wmemset (begin + (quailLength / 2), L'\0', 2); 	// Replace `\\` with `\0`.
			
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

		//const auto& filepath 		= DEFAULT_FOLDERPATH_W;
		//const auto& filepathLength	= DEFAULT_FOLDERPATH_LENGTH;

		HKEY key;
		LSTATUS error;
		unsigned long status;

		error = RegCreateKeyExW (
			HKEY_LOCAL_MACHINE, 			// Type
			REGISTRY_PATH_W, 				// Path
			0, 								// RESERVED
			NULL, 							// optional. Class Type
			REG_OPTION_NON_VOLATILE, 		// defualt - store data in file not in RAM.
			KEY_WRITE | KEY_QUERY_VALUE, 	// Access rights for the key to be created.
			NULL, 							// Security descriptor for the new key.
			&key, 							// A pointer to a variable that receives a handle to the opened or created key.
			&status							// Was key created or was it only openned because it arleady exsisted.
		);

		if (error != ERROR_SUCCESS) {
        	ERROR ("Creating key failed.\n\n");
    	}

		switch (status) {
			case REG_CREATED_NEW_KEY: 		LOGINFO ("Key successfully created.\n"); 	break;
			case REG_OPENED_EXISTING_KEY: 	LOGWWARN ("Key arleady exists.\n"); 		break;
			default: 						ERROR ("Unknown key-status\n\n");
		}

		error = RegSetValueExW (
			key, 							// Type
			PROPERTY_FILEPATH_W, 			// Name
			0, 								// RESERVED
			REG_EXPAND_SZ, 					// value type - https://learn.microsoft.com/en-us/windows/win32/sysinfo/registry-value-types
			(LPCBYTE) filepath, 			// data
			filepathLength					// dataCount
		);

    	if (error != ERROR_SUCCESS) {
    	    ERROR ("Setting key value failed.\n\n");
    	}

		RegCloseKey (key);

	}

}