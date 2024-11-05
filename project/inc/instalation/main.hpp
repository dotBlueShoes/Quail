// Created 2024.10.29 by Matthew Strumiłło (dotBlueShoes)
//
#pragma once
#include "base/types.hpp"
#include "base/log.hpp"
#include "base/io.hpp"

#include "registry.hpp"

namespace INSTALLATION {

	void CreateAll () {

		REGISTRY::CreateKeys ();

		{ // Create config file if it exists don't

			REGISTRY::LoadKeyValues ();

			const auto& filepath 		= OPEN::mainConfigFilePath;
			const auto& filepathLength	= OPEN::mainConfigFolderPathLength;

			const u32 configFilepathLength = filepathLength + 1 + REGISTRY::CONFIG_LENGTH;
			c16* configFilepath = (c16*) malloc (configFilepathLength); // ALLOCATION

			{ // CONSTRUCT
				memcpy (configFilepath, filepath, filepathLength);
				configFilepath[(filepathLength / 2) - 1] = L'\\';
				memcpy (configFilepath + (filepathLength / 2), REGISTRY::CONFIG_W, REGISTRY::CONFIG_LENGTH);
			}

			if (IO::IsExisting (configFilepath)) {

				printf ("\nINFO: File already exists.\n");

			} else {

				IO::Create (configFilepath);

			}

			// FREE AND SWAP
			free (OPEN::mainConfigFilePath);
			OPEN::mainConfigFilePathLength = configFilepathLength;
			OPEN::mainConfigFilePath = configFilepath;

		}

	}

}
