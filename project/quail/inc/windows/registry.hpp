// Created 2024.11.05 by Matthew Strumiłło (dotBlueShoes)
//  LICENSE: GNU GENERAL PUBLIC LICENSE Version 3, 29 June 2007
//
#pragma once
#include <global/windows/registry.hpp>

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
