// Created 2024.11.05 by Matthew Strumiłło (dotBlueShoes)
//  LICENSE: GNU GENERAL PUBLIC LICENSE Version 3, 29 June 2007
//
#pragma once
#include <global/windows/registry.hpp>
#include <blue/windows/registry.hpp>

namespace WINDOWS::REGISTRY {

	void SetPropertyIsForceC8Display (
		IN 		const bool& value
	) {

		unsigned long error = 0;
		u32 data = !value;
		HKEY key;

		error = RegOpenKeyW(
			HKEY_LOCAL_MACHINE,
			KEY_PATH_W,
			&key
		);

		if (error != ERROR_SUCCESS) ERROR ("Could not open the key!\n");

		CreatePropertyS32 (key, error, PROPERTY_QUAIL_IS_FORCE_C8_DISPLAY, data);
		CHECK_PROPERTY (error, PROPERTY_QUAIL_IS_FORCE_C8_DISPLAY);

		CONFIG::isForceC8Display = data & (1 << 0);
		
	}

	void CreateQuailConfigsFilePaths (
		INOUT 	u32& 	folderPathLength,
		INOUT 	c16*& 	folderPath,
		OUT 	u32& 	mConfigFilePathLength,
		OUT 	c16*& 	mConfigFilePath,
		OUT 	u32& 	gConfigFilePathLength,
		OUT 	c16*& 	gConfigFilePath
	) {

		{ // Replace '\0' with '\\'. So that we can later create strings with it simpler.
			wmemset (folderPath + (folderPathLength / 2) - 1, L'\\', 1);
		}

		mConfigFilePathLength = folderPathLength + 1 + CONFIG::CONFIG_MAIN_LENGTH;
		ALLOCATE (c16, mConfigFilePath, mConfigFilePathLength);

		{ // CONSTRUCT
			memcpy (mConfigFilePath, folderPath, folderPathLength);
			memcpy (mConfigFilePath + (folderPathLength / 2), CONFIG::CONFIG_MAIN_W, CONFIG::CONFIG_MAIN_LENGTH);
		}

		gConfigFilePathLength = folderPathLength + 1 + CONFIG::CONFIG_GLOBAL_LENGTH;
		ALLOCATE (c16, gConfigFilePath, gConfigFilePathLength);

		{ // CONSTRUCT
			memcpy (gConfigFilePath, folderPath, folderPathLength);
			memcpy (gConfigFilePath + (folderPathLength / 2), CONFIG::CONFIG_GLOBAL_W, CONFIG::CONFIG_GLOBAL_LENGTH);
		}
	}

}
