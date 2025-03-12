// Created 2024.01.14 by Matthew Strumiłło (dotBlueShoes)
//  LICENSE: GNU GENERAL PUBLIC LICENSE Version 3, 29 June 2007
//
#pragma once
#include <blue/types.hpp>
#include <blue/error.hpp>

#include "global/config.hpp"

namespace WINDOWS::REGISTRY {

	const c16 KEY_PATH_W							[] = L"SOFTWARE\\dotBlueShoes\\Quail";
	const c16 PROPERTY_QUAIL_FILEPATH_W				[] = L"filepath";
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

	const c16 KEY_ENVIROMENT_VARIABLES_W 			[] = L"System\\CurrentControlSet\\Control\\Session Manager\\Environment";
	const c16 PROPERTY_PATH_W 						[] = L"path";


	void ReadPropertyTopConfigsFolder () {

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

			WERROR ("Could not read key value: %d" ERROR_NEW_LINE, size);

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
        	
		LOGWINFO (" > Successfully read property '%s' as: '%s'\n", PROPERTY_QUAIL_FILEPATH_W, data);

		CONFIG::topConfigsFolderLength = size;
    	CONFIG::topConfigsFolder = data;

	}

}
