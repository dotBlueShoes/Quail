// Created 2024.01.16 by Matthew Strumiłło (dotBlueShoes)
//  LICENSE: GNU GENERAL PUBLIC LICENSE Version 3, 29 June 2007
//
#pragma once
#include <blue/types.hpp>

namespace CONFIG {

	const c16 MAIN_W			[] = L"config.txt";
	const u32 MAIN_LENGTH		= sizeof (MAIN_W);

	const c16 GLOBAL_W			[] = L"global.txt";
	const u32 GLOBAL_LENGTH		= sizeof (GLOBAL_W);
	
	u32 topConfigsFolderLength; 
	c16* topConfigsFolder;

	u32 configMainFilePathLength;
	c16* configMainFilePath;
	
	u32 configGlobalFilePathLength;
	c16* configGlobalFilePath;

}
