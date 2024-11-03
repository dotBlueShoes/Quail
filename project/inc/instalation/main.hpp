// Created 2024.10.29 by Matthew Strumiłło (dotBlueShoes)
//
#pragma once
#include "base/types.hpp"
#include "base/log.hpp"
#include "base/io.hpp"

#include "interpreter/open_main.hpp"

namespace INSTALLATION {

	const c16 REGISTRY_PATH_W[]			= L"SOFTWARE\\dotBlueShoes\\Quail";
	const c16 PROPERTY_FILEPATH_W[]		= L"filepath";

	const c16 DEFAULT_FILEPATH_W[] 		= L"C:\\Program Files\\dotBlueShoes\\Quail";
	const u32 DEFAULT_FILEPATH_LENGTH	= sizeof (DEFAULT_FILEPATH_W);

	const c16 CONFIG_W[]				= L"config.txt";
	const u32 CONFIG_LENGTH				= sizeof (CONFIG_W);

	void LoadRegistryKeyValue () {

		LSTATUS error;
		unsigned long status;

    	c16* data;
    	DWORD size = 0;

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

			wprintf (L"\n\tError. Could not read key value: %d\n\n", size);

		}

		// ALLOCATION
        data = (c16*) malloc (size);

        status = RegGetValueW (
			HKEY_LOCAL_MACHINE, 
			REGISTRY_PATH_W, 
			PROPERTY_FILEPATH_W, 
			RRF_NOEXPAND | RRF_RT_REG_EXPAND_SZ, NULL, 
			data,
			&size
		);
        	
		wprintf (L"\nINFO: Successfully read property '%s' as: '%s'", PROPERTY_FILEPATH_W, data);
		free (data);
    	
	}

	void CreateRegistryKey () {

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
        	ERROR ("\n\tError. Creating key failed.\n\n");
    	}

		if (status == REG_CREATED_NEW_KEY) {

			printf ("\nINFO: Key Created");

		} else if (status == REG_OPENED_EXISTING_KEY) {

			printf ("\nINFO: Key Openned");

		} else {

			ERROR ("\n\tError. Unknown key-status\n\n");

		}

		error = RegSetValueExW (
			key, 							// Type
			PROPERTY_FILEPATH_W, 			// Name
			0, 								// RESERVED
			REG_EXPAND_SZ, 					// value type - https://learn.microsoft.com/en-us/windows/win32/sysinfo/registry-value-types
			(LPCBYTE)filepath, 				// data
			filepathLength					// dataCount
		);

    	if (error != ERROR_SUCCESS) {
    	    ERROR ("\n\tError. Setting key value failed.\n\n");
    	}

		RegCloseKey (key);

	}

	void CreateAll () {

		CreateRegistryKey ();
		LoadRegistryKeyValue ();

		const auto& filepath 		= DEFAULT_FILEPATH_W;
		const auto& filepathLength	= DEFAULT_FILEPATH_LENGTH;

		const u32 configFilepathLength = filepathLength + 1 + CONFIG_LENGTH;
		c16* configFilepath = (c16*) malloc (configFilepathLength); // ALLOCATION
		
		{ // CONSTRUCT
			memcpy (configFilepath, filepath, filepathLength);
			configFilepath[(filepathLength / 2) - 1] = L'\\';
			memcpy (configFilepath + (filepathLength / 2), CONFIG_W, CONFIG_LENGTH);
		}

		if (IO::IsExisting (configFilepath)) {
			printf ("\nINFO: File already exists.");
		} else {
			IO::Create (configFilepath);
		}

		FILE* mainConfig = nullptr;
		IO::Read (configFilepath, mainConfig);
		free (configFilepath);

		{
			INTERPRETER::OPENMAIN::Interpreter interpreter { 0 };
			INTERPRETER::OPENMAIN::parsingstage = INTERPRETER::OPENMAIN::Start;

			while (interpreter.current != EOF) { // READ
				interpreter.last = interpreter.current;
				interpreter.current = getc (mainConfig);
				INTERPRETER::OPENMAIN::parsingstage (interpreter);
			}

			printf ("\nINFO: File read successfully\n");
		}

		IO::Close (mainConfig);
	}

}
