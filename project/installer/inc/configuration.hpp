// Created 2025.01.14 by Matthew Strumiłło (dotBlueShoes)
//  LICENSE: GNU GENERAL PUBLIC LICENSE Version 3, 29 June 2007
//
#pragma once
#include <global/version.h>
#include <blue/types.hpp>


#define QUAIL_NAME_W			L"Quail"
#define QUAIL_NAME				"Quail"

#define QUAIL_NAME_VERSION_W 	QUAIL_NAME_W L" " QUAIL_FULL_VERSION_W
#define QUAIL_NAME_VERSION 		QUAIL_NAME " " QUAIL_FULL_VERSION


namespace CONFIGURATION {

	const c16 QUAIL_W[]				= L"Quail.exe";
	const u32 QUAIL_LENGTH			= sizeof (QUAIL_W);

	const c16 UNINSTALLER_W[]		= L"uninstall.exe";
	const u32 UNINSTALLER_LENGTH	= sizeof (UNINSTALLER_W);

	const c16 MAIN_CONFIG_W[]		= L"config.txt";
	const u32 MAIN_CONFIG_LENGTH	= sizeof (MAIN_CONFIG_W);
	
	const c16 GLOBAL_CONFIG_W[]		= L"global.txt";
	const u32 GLOBAL_CONFIG_LENGTH	= sizeof (GLOBAL_CONFIG_W);


	const c8* URL_QUAIL_EXECUTABLE = "https://github.com/dotBlueShoes/MS_Fogger/releases/download/v1.1.2/fogger-1.12.2-1.1.2.0.jar";
	//const c8* URL_QUAIL_EXECUTABLE = "https://github.com/dotBlueShoes/Quail/releases/download/0.4/Quail.exe";

	const c8* URL_QUAIL_UNINSTALLER = "https://github.com/dotBlueShoes/bta-Gems/releases/download/0.4.5/gems_mod-0.4.5.jar";
	//const c8* URL_QUAIL_UNINSTALLER = "https://github.com/dotBlueShoes/Quail/releases/download/0.4/uninstaller.exe";

	const c16 EXECUTABLE_NAME[] 		= L"QuailNew.exe";
	//const c16 EXECUTABLE_NAME[] 		= L"Quail.exe";
	const u32 EXECUTABLE_NAME_LENGTH 	= sizeof (EXECUTABLE_NAME);

	const c16 UNINSTALLER_NAME[] 		= L"uninstall.exe";
	const u32 UNINSTALLER_NAME_LENGTH 	= sizeof (UNINSTALLER_NAME);
	
	u32 topConfigsFolderPathLength;		// Where are quail top configs.
	c16* topConfigsFolderPath;

}