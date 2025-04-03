// Created 2024.01.16 by Matthew Strumiłło (dotBlueShoes)
//  LICENSE: GNU GENERAL PUBLIC LICENSE Version 3, 29 June 2007
//
#pragma once
#include <blue/types.hpp>

#include "version.h"

namespace CONFIG {

	#define QUAIL_NAME_W						L"Quail"
	#define QUAIL_NAME							"Quail"

	#define QUAIL_NAME_VERSION_W 				QUAIL_NAME_W L" " QUAIL_FULL_VERSION_W
	#define QUAIL_NAME_VERSION 					QUAIL_NAME " " QUAIL_FULL_VERSION

	#define DEFAULT_CONFIG_MAIN_1				L"%path 			= "
	#define DEFAULT_CONFIG_MAIN_2				L"\n!config			= start \"\" notepad \"%path%\\config.txt\"" \
												L"\n!global			= start \"\" notepad \"%path%\\global.txt\"" \
												L"\n!directory 		= explorer.exe \"%path%\""

	#define DEFAULT_CONFIG_GLOBAL				L"!directory 		= explorer.exe \"%_path%\""

	const c16 QUAIL_SHOURTCUT_NAME				[] = L"o.bat";
	const c16 QUAIL_SHOURTCUT_CONTENT			[] = L"@echo off\nQuail.exe -o %*\n";

	const u32 QUAIL_SHOURTCUT_NAME_LENGTH 		= sizeof (QUAIL_SHOURTCUT_NAME);
	const u32 QUAIL_SHOURTCUT_CONTENT_LENGTH 	= sizeof (QUAIL_SHOURTCUT_CONTENT);

	const u32 DEFAULT_CONFIG_MAIN_1_LENGTH 		= sizeof (DEFAULT_CONFIG_MAIN_1) - 2; // (-2) - removal of null terminator
	const u32 DEFAULT_CONFIG_MAIN_2_LENGTH 		= sizeof (DEFAULT_CONFIG_MAIN_2);

	const c8 QUAIL_NAME_VERSION_SIZE 			= sizeof (QUAIL_NAME_VERSION);
	const c8 QUAIL_NAME_SIZE 					= sizeof (QUAIL_NAME);

	const c16 CONFIG_MAIN_W						[] = L"config.txt";
	const u32 CONFIG_MAIN_LENGTH				= sizeof (CONFIG_MAIN_W);
	
	const c16 CONFIG_GLOBAL_W					[] = L"global.txt";
	const u32 CONFIG_GLOBAL_LENGTH				= sizeof (CONFIG_GLOBAL_W);

	const c8 URL_QUAIL_EXECUTABLE 				[] = "https://github.com/dotBlueShoes/Quail/releases/download/0.5/Quail.exe";
	const c8 URL_QUAIL_UNINSTALLER 				[] = "https://github.com/dotBlueShoes/Quail/releases/download/0.5/uninstall.exe";

	const c16 EXECUTABLE_NAME					[] = L"Quail.exe";
	const u32 EXECUTABLE_NAME_LENGTH 			= sizeof (EXECUTABLE_NAME);

	const c16 UNINSTALLER_NAME					[] = L"uninstall.exe";
	const u32 UNINSTALLER_NAME_LENGTH 			= sizeof (UNINSTALLER_NAME);

	bool isForceC8Display = false;
	u16 listingLineSize = 120;
	
	u32 topConfigsFolderLength; 
	c16* topConfigsFolder;

	u32 configMainFilePathLength;
	c16* configMainFilePath;
	
	u32 configGlobalFilePathLength;
	c16* configGlobalFilePath;

}
