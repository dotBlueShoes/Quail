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

		LOGINFO ("value is: %d\n", value);

		LSTATUS error = 0;
		u32 data = !value;
		HKEY key;

		LOGINFO ("value is: %d\n", data);

		error = RegOpenKeyW(
			HKEY_LOCAL_MACHINE,
			KEY_PATH_W,
			&key
		);

		if (error != ERROR_SUCCESS) ERROR ("Could not open the key!\n");

		CreatePropertyS32 (key, error, PROPERTY_QUAIL_IS_FORCE_C8_DISPLAY, data);
		CHECK_PROPERTY (error, PROPERTY_QUAIL_IS_FORCE_C8_DISPLAY);
		
	}


	void SetPropertyListingLineSize (
		IN 		const u32& value
	) {

		LSTATUS error = 0;
		HKEY key;

		error = RegOpenKeyW(
			HKEY_LOCAL_MACHINE,
			KEY_PATH_W,
			&key
		);

		if (error != ERROR_SUCCESS) ERROR ("Could not open the key!\n");

		CreatePropertyS32 (key, error, PROPERTY_QUAIL_LISTING_LINE_SIZE, value);
		CHECK_PROPERTY (error, PROPERTY_QUAIL_LISTING_LINE_SIZE);
		
	}

	void GetPropertyListingLineSize () {
		
		LSTATUS status;
		u32 data;

		unsigned long dataSize = sizeof (data);

		status = RegGetValueW (
			HKEY_LOCAL_MACHINE, 
			KEY_PATH_W, 
			PROPERTY_QUAIL_LISTING_LINE_SIZE, 
			RRF_RT_REG_DWORD, 
			NULL, 
			&data,
			&dataSize
		);

		if ((status != ERROR_SUCCESS)) {
			ERROR ("Could not get 'ListingLineSize' from registry.\n");
		}

		LOGWINFO (" > Successfully read property '%s' as: '%d'\n", PROPERTY_QUAIL_LISTING_LINE_SIZE, data);

		CONFIG::listingLineSize = data;

		if (CONFIG::listingLineSize < CONFIG::LISTING_LINE_SIZE_MIN && CONFIG::listingLineSize != 0) {
			LOGWARN (
				"ListingLineSize value (%d) is invalid! Minimal value is (%d). Defaulting to NO_CAP (0).\n", 
				CONFIG::listingLineSize, CONFIG::LISTING_LINE_SIZE_MIN
			)
		}
		
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
