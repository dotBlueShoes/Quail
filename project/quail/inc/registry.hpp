// Created 2024.11.05 by Matthew Strumiłło (dotBlueShoes)
//
#pragma once
#include <blue/types.hpp>
#include <blue/log.hpp>

#include "open/data.hpp"

namespace REGISTRY {

	const c16 REGISTRY_PATH_W[]			= L"SOFTWARE\\dotBlueShoes\\Quail";
	const c16 PROPERTY_FILEPATH_W[]		= L"filepath";

	const c16 DEFAULT_FILEPATH_W[] 		= L"C:\\Program Files\\dotBlueShoes\\Quail";
	const u32 DEFAULT_FILEPATH_LENGTH	= sizeof (DEFAULT_FILEPATH_W);

	const c16 CONFIG_W[]				= L"config.txt";
	const u32 CONFIG_LENGTH				= sizeof (CONFIG_W);

	const c16 GLOBAL_W[]				= L"global.txt";
	const u32 GLOBAL_LENGTH				= sizeof (GLOBAL_W);

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

		// GET
		error = RegGetValueW (key, NULL, name, RRF_RT_ANY, NULL, env, &envSize);

		if (error != ERROR_SUCCESS) {
			ERROR ("Could not get `%ls` value.\n", name);
		} 

		{ // Adding quail
			auto&& binary = ((u8*) env) + envSize; //642;
			auto&& begin = (c16*) binary - 2;

			wmemset (begin + 0, L';', 2);					// Replace `\0` with `;`.
			memcpy  (begin + 1, quail, quailLength);		// Add Quail
			wmemset (begin + (quailLength / 2), L'\0', 2); 	// Replace `\\` with `\0`.

			envSize = envSize +quailLength + 1;

			//LOGWINFO ("`Path:` [%d]: %s\n", envSize, env);
		}

		// SET
    	error = RegSetValueExW (key, name, 0, REG_SZ, (LPBYTE)env, envSize);
		
		if (error != ERROR_SUCCESS) {
			ERROR ("Could not set `%ls` value with `%ls`\n", name, env);
		}

		// Update all other applications because we did just edited enviroment varaibles!
		SendMessageTimeoutW (HWND_BROADCAST, WM_SETTINGCHANGE, 0, (LPARAM)"Environment", SMTO_BLOCK, 100, NULL);
		LOGWINFO ("Successfully added Quail to `Path` enviroment varaible.\n");

		// FREE
    	RegCloseKey (key);
		FREE (env);

	}

	void CreateKeys () {

		const auto& filepath 		= DEFAULT_FILEPATH_W;
		const auto& filepathLength	= DEFAULT_FILEPATH_LENGTH;

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

		if (status == REG_CREATED_NEW_KEY) {
			LOGINFO ("Key Created\n");
		} else if (status == REG_OPENED_EXISTING_KEY) {
			LOGINFO ("Key Openned\n");
		} else {
			ERROR ("Unknown key-status\n\n");
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

	void LoadKeyValues () {

		LSTATUS error;
		unsigned long status;

    	c16* data;
    	DWORD size = 0;

		// First get required size for memory allocation.
    	status = RegGetValueW (
			HKEY_LOCAL_MACHINE, 
			REGISTRY_PATH_W, 
			PROPERTY_FILEPATH_W, 
			RRF_NOEXPAND | RRF_RT_REG_EXPAND_SZ, 
			NULL, 
			NULL, 
			&size
		);
		
    	if ((status != ERROR_SUCCESS) && (size < 1)) {

			WERROR ("Could not read key value: %d\n\n", size);

		}

		// ALLOCATION
		ALLOCATE (c16, data, size);

		// Second get data.
        status = RegGetValueW (
			HKEY_LOCAL_MACHINE, 
			REGISTRY_PATH_W, 
			PROPERTY_FILEPATH_W, 
			RRF_NOEXPAND | RRF_RT_REG_EXPAND_SZ, 
			NULL, 
			data,
			&size
		);
        	
		LOGWINFO ("Successfully read property '%s' as: '%s'\n", PROPERTY_FILEPATH_W, data);

		OPEN::mainConfigFolderPathLength = size;
    	OPEN::mainConfigFilePath = data;

	}
}
