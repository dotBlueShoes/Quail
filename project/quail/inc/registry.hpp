// Created 2024.11.05 by Matthew Strumiłło (dotBlueShoes)
//  LICENSE: GNU GENERAL PUBLIC LICENSE Version 3, 29 June 2007
//
#pragma once
#include <global/windows/registry.hpp>
#include <global/config.hpp>

#include <blue/types.hpp>
#include <blue/log.hpp>

namespace WINDOWS::REGISTRY {

	void CreateQuailConfigsFilePaths (
		/* IN & OUT */ u32& 	folderPathLength,
		/* IN & OUT */ c16*& 	folderPath,
		/* OUT */ u32& 			mConfigFilePathLength,
		/* OUT */ c16*& 		mConfigFilePath,
		/* OUT */ u32& 			gConfigFilePathLength,
		/* OUT */ c16*& 		gConfigFilePath
	) {

		{ // Replace '\0' with '\\'. So that we can later create strings with it simpler.
			wmemset (folderPath + (folderPathLength / 2) - 1, L'\\', 1);
		}

		mConfigFilePathLength = folderPathLength + 1 + REGISTRY::CONFIG_MAIN_LENGTH;
		ALLOCATE (c16, mConfigFilePath, mConfigFilePathLength);

		{ // CONSTRUCT
			memcpy (mConfigFilePath, folderPath, folderPathLength);
			memcpy (mConfigFilePath + (folderPathLength / 2), REGISTRY::CONFIG_MAIN_W, REGISTRY::CONFIG_MAIN_LENGTH);
		}

		gConfigFilePathLength = folderPathLength + 1 + REGISTRY::CONFIG_GLOBAL_LENGTH;
		ALLOCATE (c16, gConfigFilePath, gConfigFilePathLength);

		{ // CONSTRUCT
			memcpy (gConfigFilePath, folderPath, folderPathLength);
			memcpy (gConfigFilePath + (folderPathLength / 2), REGISTRY::CONFIG_GLOBAL_W, REGISTRY::CONFIG_GLOBAL_LENGTH);
		}
	}


	void LoadKeyValues () {

		LSTATUS error;
		unsigned long status;

    	c16* data;
    	DWORD size = 0;

		// First get required size for memory allocation.
    	status = RegGetValueW (
			HKEY_LOCAL_MACHINE, 
			KEY_PATH_W, 
			PROPERTY_QUAIL_FILEPATH_W, 
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
			KEY_PATH_W, 
			PROPERTY_QUAIL_FILEPATH_W, 
			RRF_NOEXPAND | RRF_RT_REG_EXPAND_SZ, 
			NULL, 
			data,
			&size
		);
        	
		LOGWINFO ("Successfully read property '%s' as: '%s'\n", PROPERTY_QUAIL_FILEPATH_W, data);

		CONFIG::topConfigsFolderLength = size;
    	CONFIG::topConfigsFolder = data;

		CreateQuailConfigsFilePaths (
			CONFIG::topConfigsFolderLength, CONFIG::topConfigsFolder,
			CONFIG::configMainFilePathLength, CONFIG::configMainFilePath,
			CONFIG::configGlobalFilePathLength, CONFIG::configGlobalFilePath
		);

	}
}
