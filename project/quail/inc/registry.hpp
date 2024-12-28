// Created 2024.11.05 by Matthew Strumiłło (dotBlueShoes)
//
#pragma once
#include <blue/types.hpp>
#include <blue/log.hpp>

namespace REGISTRY {

	// PROPERTIES

	const c16 REGISTRY_PATH_W[]			= L"SOFTWARE\\dotBlueShoes\\Quail";
	const c16 PROPERTY_FILEPATH_W[]		= L"filepath";

	const c16 CONFIG_W[]				= L"config.txt";
	const u32 CONFIG_LENGTH				= sizeof (CONFIG_W);

	// VARIABLES

	u32 topConfigsFolderPathLength; // Where are quail top configs.
	u32 mainConfigFilePathLength; // Full filepath to quail "main" config.
	c16* mainConfigFilePath;

	// FUNCTIONS

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

		topConfigsFolderPathLength = size;
    	mainConfigFilePath = data;

		{ // Setting up final datatypes.
			const auto& filepath 		= mainConfigFilePath;
			const auto& filepathLength	= topConfigsFolderPathLength;

			const u32 configFilepathLength = filepathLength + 1 + REGISTRY::CONFIG_LENGTH;
			c16* configFilepath; ALLOCATE (c16, configFilepath, configFilepathLength);// = (c16*) malloc (configFilepathLength); // ALLOCATION

			{ // CONSTRUCT
				memcpy (configFilepath, filepath, filepathLength);
				configFilepath[(filepathLength / 2) - 1] = L'\\';
				memcpy (configFilepath + (filepathLength / 2), REGISTRY::CONFIG_W, REGISTRY::CONFIG_LENGTH);
			}

			{ // FREE AND SWAP
				FREE (mainConfigFilePath);
				mainConfigFilePathLength = configFilepathLength;
				mainConfigFilePath = configFilepath;
			}
		}

	}
}
