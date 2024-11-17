// Created 2024.10.29 by Matthew Strumiłło (dotBlueShoes)
//
#pragma once
#include "base/types.hpp"
#include "base/log.hpp"
#include "base/io.hpp"

#include "registry.hpp"
#include "envars.hpp"

namespace INSTALLATION {

	void CreateAll () {

		REGISTRY::CreateKeys ();

		{ // Create config file if it exists don't

			REGISTRY::LoadKeyValues ();

			const auto& filepath 		= OPEN::mainConfigFilePath;
			const auto& filepathLength	= OPEN::mainConfigFolderPathLength;

			const u32 configFilepathLength = filepathLength + 1 + REGISTRY::CONFIG_LENGTH;
			c16* configFilepath; ALLOCATE (c16, configFilepath, configFilepathLength);// = (c16*) malloc (configFilepathLength); // ALLOCATION

			{ // CONSTRUCT
				memcpy (configFilepath, filepath, filepathLength);
				configFilepath[(filepathLength / 2) - 1] = L'\\';
				memcpy (configFilepath + (filepathLength / 2), REGISTRY::CONFIG_W, REGISTRY::CONFIG_LENGTH);
			}

			{ // FREE AND SWAP
				FREE (OPEN::mainConfigFilePath);
				OPEN::mainConfigFilePathLength = configFilepathLength;
				OPEN::mainConfigFilePath = configFilepath;
			}

			if (IO::IsExisting (configFilepath)) {

				LOGINFO ("File already exists.\n");

			} else {

				REGISTRY::AddQuailToPath (OPEN::mainConfigFolderPathLength, OPEN::mainConfigFilePath);
				IO::Create (configFilepath);

			}

		}

	}

}
