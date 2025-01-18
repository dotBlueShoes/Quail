// Created 2025.01.14 by Matthew Strumiłło (dotBlueShoes)
//  LICENSE: GNU GENERAL PUBLIC LICENSE Version 3, 29 June 2007
//
#pragma once
#include <global/config.hpp>

#include "registry.hpp"
#include "download.hpp"

namespace INSTALLATION {

	const RECT CAPTION_REGION 		{ 29, 75, CAPTION_REGION.left + 40, CAPTION_REGION.top + 10 };
	const RECT TEXT_REGION 			{ 29, 75 + 14 + 4, TEXT_REGION.left + 434 - 26, TEXT_REGION.top + 16 };
	const RECT REST_REGION 			{ TEXT_REGION.right, TEXT_REGION.top, 14, 14 };

	// Phases
	enum PHASE: u8 {
		PHASE_NONE 					= 0,
		PHASE_DOWNLOAD_MAIN 		= 1,
		PHASE_DOWNLOAD_UNINSTALLER 	= 2,
		PHASE_CREATE_REGISTRY 		= 3,
		PHASE_CREATE_PATH 			= 4,
		PHASE_CREATE_FILES 			= 5,
		PHASE_END					= 6,
	}; u8 currentPhase = 0;

	FILE* fileHandle;

	bool isLicense = false;
	bool isRegistry = true;
	bool isPath = true;


	void BeginPhaseOne (HWND& progressBar) {

		DOWNLOAD::runningHandles = 1;
		currentPhase = PHASE_DOWNLOAD_MAIN; 

		auto& directoryPathLength = CONFIG::topConfigsFolderLength;
		auto&& directoryPath = CONFIG::topConfigsFolder;
		c16* buffer; ALLOCATE (c16, buffer, directoryPathLength + CONFIG::EXECUTABLE_NAME_LENGTH + 1);

		{ // CONSTRUCT 
			memcpy (buffer, directoryPath, directoryPathLength); 	// Copy '\0'.
			buffer[(directoryPathLength / 2) - 1] = L'\\';			// Emplace '\\' sign into a single of 2 bytes.
			memcpy (buffer + (directoryPathLength / 2), CONFIG::EXECUTABLE_NAME, CONFIG::EXECUTABLE_NAME_LENGTH);
			LOGINFO ("executable filepath: %ls\n", buffer);
		}

		// Create file for Quail Executable. Check. Free unused.
		fileHandle = _wfopen (buffer, L"wb"); 
		if (!fileHandle) ERROR ("File opening failed\n"); 
		FREE (buffer);

		// Create download for Quail Executable.
		DOWNLOAD::Create (DOWNLOAD::syncHandle, DOWNLOAD::asyncHandle, fileHandle, progressBar, CONFIG::URL_QUAIL_EXECUTABLE);

	}

	void EndPhaseOne () {
		DOWNLOAD::Delete (DOWNLOAD::syncHandle, DOWNLOAD::asyncHandle);
		fclose (INSTALLATION::fileHandle);

		++currentPhase;
	}

	void BeginPhaseTwo (HWND& progressBar) {

		{ // WM_PAINT
			InvalidateRect (GetParent (progressBar), &CAPTION_REGION, FALSE);
			InvalidateRect (GetParent (progressBar), &TEXT_REGION, FALSE);
			InvalidateRect (GetParent (progressBar), &REST_REGION, FALSE);
		}

		DOWNLOAD::runningHandles = 1;
		

		{ // Actuall State Two
			auto& directoryPathLength = CONFIG::topConfigsFolderLength;
			auto&& directoryPath = CONFIG::topConfigsFolder;
			c16* buffer; ALLOCATE (c16, buffer, directoryPathLength + CONFIG::UNINSTALLER_NAME_LENGTH + 1);

			{ // CONSTRUCT
				memcpy (buffer, directoryPath, directoryPathLength); 	// Copy '\0'.
				buffer[(directoryPathLength / 2) - 1] = L'\\';			// Emplace '\\' sign into a single of 2 bytes.
				memcpy (buffer + (directoryPathLength / 2), CONFIG::UNINSTALLER_NAME, CONFIG::UNINSTALLER_NAME_LENGTH);
				LOGINFO ("uninstaller filepath: %ls\n", buffer);
			}

			// Create file for Quail Executable. Check. Free unused.
			fileHandle = _wfopen (buffer, L"wb"); 
			if (!fileHandle) ERROR ("File opening failed\n"); 
			FREE (buffer);

			// Create download for Quail Executable.
			DOWNLOAD::Create (DOWNLOAD::syncHandle, DOWNLOAD::asyncHandle, fileHandle, progressBar, CONFIG::URL_QUAIL_EXECUTABLE);
		}

	}

	void EndPhaseTwo () {
		DOWNLOAD::Delete (DOWNLOAD::syncHandle, DOWNLOAD::asyncHandle);
		fclose (INSTALLATION::fileHandle);

		++currentPhase;
	}

	void PhaseThree (HWND& progressBar) {

		{ // WM_PAINT
			InvalidateRect (GetParent (progressBar), &CAPTION_REGION, FALSE);
			InvalidateRect (GetParent (progressBar), &TEXT_REGION, FALSE);
			InvalidateRect (GetParent (progressBar), &REST_REGION, FALSE);
		}

		if (isRegistry) WINDOWS::REGISTRY::CreateKeys (
			CONFIG::topConfigsFolderLength, 
			CONFIG::topConfigsFolder
		);

		++currentPhase;

	}

	void PhaseFour (HWND& progressBar) {

		{ // WM_PAINT
			InvalidateRect (GetParent (progressBar), &CAPTION_REGION, FALSE);
			InvalidateRect (GetParent (progressBar), &TEXT_REGION, FALSE);
			InvalidateRect (GetParent (progressBar), &REST_REGION, FALSE);
		}

		if (isPath) WINDOWS::REGISTRY::AddQuailToPath (
			CONFIG::topConfigsFolderLength, 
			CONFIG::topConfigsFolder
		);

		++currentPhase;
	}

	void PhaseFive (HWND& progressBar) {

		{ // WM_PAINT
			InvalidateRect (GetParent (progressBar), &CAPTION_REGION, FALSE);
			InvalidateRect (GetParent (progressBar), &TEXT_REGION, FALSE);
			InvalidateRect (GetParent (progressBar), &REST_REGION, FALSE);
		}

		WINDOWS::REGISTRY::CreateFiles (
			CONFIG::topConfigsFolderLength, 
			CONFIG::topConfigsFolder
		);

		++currentPhase;

	}

}