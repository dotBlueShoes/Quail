// Created 2024.01.14 by Matthew Strumiłło (dotBlueShoes)
//  LICENSE: GNU GENERAL PUBLIC LICENSE Version 3, 29 June 2007
//
#pragma once
#include <blue/windows/registry.hpp>
#include <blue/types.hpp>
#include <blue/error.hpp>

#include "global/config.hpp"

namespace WINDOWS::REGISTRY {

	const c16 KEY_PATH_W							[] = L"SOFTWARE\\dotBlueShoes\\Quail";
	const c16 PROPERTY_QUAIL_CONFIGS_FILEPATH_W		[] = L"ConfigsFilepath";
	const c16 PROPERTY_QUAIL_IS_FORCE_C8_DISPLAY	[] = L"IsForceC8Display";
	const c16 PROPERTY_QUAIL_LISTING_LINE_SIZE		[] = L"ListingLineSize";

	const c16 VALUE_DEFAULT_QUAIL_FOLDER_W			[] = L"C:\\Program Files\\dotBlueShoes\\Quail";

	const c16 KEY_PATH_UNINSTALL_W					[] = L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\Quail";

	const c16 PROPERTY_UNINSTALL_DISPLAY_ICON		[] = L"DisplayIcon";
	const c16 PROPERTY_UNINSTALL_DISPLAY_NAME		[] = L"DisplayName";
	const c16 PROPERTY_UNINSTALL_DISPLAY_VERSION	[] = L"DisplayVersion";
	const c16 PROPERTY_UNINSTALL_ESTIMATED_SIZE		[] = L"EstimatedSize";
	const c16 PROPERTY_UNINSTALL_NO_REPAIR			[] = L"NoRepair";
	const c16 PROPERTY_UNINSTALL_NO_MODIFY			[] = L"NoModify";
	const c16 PROPERTY_UNINSTALL_HELP_LINK			[] = L"HelpLink";
	const c16 PROPERTY_UNINSTALL_INSTALL_LOCATION	[] = L"InstallLocation";
	const c16 PROPERTY_UNINSTALL_PUBLISHER			[] = L"Publisher";
	const c16 PROPERTY_UNINSTALL_UNINSTALL_STRING	[] = L"UninstallString";
	const c16 PROPERTY_UNINSTALL_URL_INFO_ABOUT		[] = L"URLInfoAbout";
	const c16 PROPERTY_UNINSTALL_URL_UPDATE_INFO	[] = L"URLUpdateInfo";

	const c16 KEY_ENVIRONMENT_VARIABLES_W 			[] = L"System\\CurrentControlSet\\Control\\Session Manager\\Environment";
	const c16 PROPERTY_PATH_W 						[] = L"Path";


	void GetPropertyIsWideCharacters (
		OUT		bool& value
	) {

		LSTATUS error;
		u32 data;
		DWORD dataSize = sizeof (data);

		error = RegGetValueW (
			HKEY_LOCAL_MACHINE, 
			KEY_PATH_W, 
			PROPERTY_QUAIL_IS_FORCE_C8_DISPLAY, 
			RRF_RT_REG_DWORD, 
			NULL, 
			&data,
			&dataSize
		);

		if (error != ERROR_SUCCESS) {
			ERROR ("Could not get 'IsForceC8Display' from registry.\n");
		}

		LOGWINFO (" > Successfully read property '%s' as: '%d'\n", PROPERTY_QUAIL_IS_FORCE_C8_DISPLAY, data);

		//CONFIG::isForceC8Display = data & (1 << 0);
		//value = data & (1 << 0);
		value = data;
		
	}


	void GetPropertyTopConfigsFolder (
		OUT		u32& valueSize,
		OUT		c16*& value
	) {

		LSTATUS error;
    	c16* data;
    	DWORD dataSize = 0;

		// First get required size for memory allocation.
    	error = RegGetValueW (
			HKEY_LOCAL_MACHINE, 
			KEY_PATH_W, 
			PROPERTY_QUAIL_CONFIGS_FILEPATH_W, 
			RRF_NOEXPAND | RRF_RT_REG_EXPAND_SZ, 
			NULL, 
			NULL, 
			&dataSize
		);
		
    	if ((error != ERROR_SUCCESS) || (dataSize < 1)) {
			ERROR ("Could not get 'ConfigsFilepath' from registry. #1\n");
		}

		// ALLOCATION
		ALLOCATE (c16, data, dataSize);

		// Second get data.
        error = RegGetValueW (
			HKEY_LOCAL_MACHINE, 
			KEY_PATH_W, 
			PROPERTY_QUAIL_CONFIGS_FILEPATH_W, 
			RRF_NOEXPAND | RRF_RT_REG_EXPAND_SZ, 
			NULL, 
			data,
			&dataSize
		);

		if (error != ERROR_SUCCESS) {
			ERROR ("Could not get 'ConfigsFilepath' from registry. #2\n");
		}
        	
		LOGWINFO (" > Successfully read property '%s' as: '%s'\n", PROPERTY_QUAIL_CONFIGS_FILEPATH_W, data);

		valueSize = dataSize;
    	value = data;

		//CONFIG::topConfigsFolderLength = size;
    	//CONFIG::topConfigsFolder = data;

	}

	void SetPropertyIsWideCharacters (
		IN 		const bool& value
	) {

		LSTATUS error;
		HKEY key;

		error = RegOpenKeyW (
			HKEY_LOCAL_MACHINE,
			KEY_PATH_W,
			&key
		);

		if (error != ERROR_SUCCESS) ERROR ("Could not open the key!\n");

		CreatePropertyS32 (key, error, PROPERTY_QUAIL_IS_FORCE_C8_DISPLAY, value);
		CHECK_PROPERTY (error, PROPERTY_QUAIL_IS_FORCE_C8_DISPLAY);
		
	}


	void SetPropertyListingLineSize (
		IN 		const u16& value
	) {

		LSTATUS error;
		HKEY key;

		error = RegOpenKeyW (
			HKEY_LOCAL_MACHINE,
			KEY_PATH_W,
			&key
		);

		if (error != ERROR_SUCCESS) ERROR ("Could not open the key!\n");

		CreatePropertyS32 (key, error, PROPERTY_QUAIL_LISTING_LINE_SIZE, value);
		CHECK_PROPERTY (error, PROPERTY_QUAIL_LISTING_LINE_SIZE);
		
	}

	void GetPropertyListingLineSize (
		OUT		u16& value
	) {
		
		LSTATUS error;
		u32 data;
		DWORD dataSize = sizeof (data);

		error = RegGetValueW (
			HKEY_LOCAL_MACHINE, 
			KEY_PATH_W, 
			PROPERTY_QUAIL_LISTING_LINE_SIZE, 
			RRF_RT_REG_DWORD, 
			NULL, 
			&data,
			&dataSize
		);

		if (error != ERROR_SUCCESS) {
			ERROR ("Could not get 'ListingLineSize' from registry.\n");
		}

		const bool isInvalidRange = data < CONFIG::LISTING_LINE_SIZE_MIN && data != 0;

		if (isInvalidRange) {
			LOGWARN (
				" > ListingLineSize value (%d) is invalid! Minimal value is (%d). Defaulting to NO_CAP (0).\n", 
				data, CONFIG::LISTING_LINE_SIZE_MIN
			)
		} else {
			LOGWINFO (
				" > Successfully read property '%s' as: '%d'\n", 
				PROPERTY_QUAIL_LISTING_LINE_SIZE, data
			);
		}

		value = data;
		
	}

}
