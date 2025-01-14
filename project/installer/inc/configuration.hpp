// Created 2025.01.14 by Matthew Strumiłło (dotBlueShoes)
//  LICENSE: GNU GENERAL PUBLIC LICENSE Version 3, 29 June 2007
//
#pragma once
#include <blue/types.hpp>

namespace CONFIGURATION {

	const c16 QUAIL_W[]				= L"Quail.exe";
	const u32 QUAIL_LENGTH			= sizeof (QUAIL_W);

	const c16 UNINSTALLER_W[]		= L"uninstall.exe";
	const u32 UNINSTALLER_LENGTH	= sizeof (UNINSTALLER_W);

	const c16 MAIN_CONFIG_W[]		= L"config.txt";
	const u32 MAIN_CONFIG_LENGTH	= sizeof (MAIN_CONFIG_W);
	
	const c16 GLOBAL_CONFIG_W[]		= L"global.txt";
	const u32 GLOBAL_CONFIG_LENGTH	= sizeof (GLOBAL_CONFIG_W);
	
	u32 topConfigsFolderPathLength;		// Where are quail top configs.
	c16* topConfigsFolderPath;

}