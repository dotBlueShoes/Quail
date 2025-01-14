// Created 2024.11.05 by Matthew Strumiłło (dotBlueShoes)
//  LICENSE: GNU GENERAL PUBLIC LICENSE Version 3, 29 June 2007
//
#include <blue/io.hpp>

#include "windows/console.hpp"
#include "windows/window.hpp"

int WinMain (
	HINSTANCE	instance,
	HINSTANCE	previousInstance,
	LPSTR		commandline,
	s32			isConsole
) {

	UNREFERENCED_PARAMETER (previousInstance);
	UNREFERENCED_PARAMETER (commandline);
    UNREFERENCED_PARAMETER (isConsole);

	DEBUG (DEBUG_FLAG_LOGGING) WINDOWS::AttachConsole ();

	LOGINFO ("Application Statred!\n");

	{ // PREP. Quail path buffor.
		ALLOCATE (c16, CONFIGURATION::topConfigsFolderPath, MAX_PATH);
	}

	{ // Window Creation.
		WINDOWS::CONTROLS::LoadRichEdit ();

		HWND window; WINDOWS::WINDOW::Create (instance, window, isConsole, { CW_USEDEFAULT, CW_USEDEFAULT }, { 496, 360 });
		MSG msg { 0 }; 

		while (msg.message != WM_QUIT) { // Main loop
			if (PeekMessage (&msg, NULL, 0U, 0U, PM_REMOVE)) {
				TranslateMessage (&msg);
				DispatchMessage (&msg);
				continue;
			}

			if (WINDOWS::WINDOW::isInstalling) {
				if (DOWNLOAD::runningHandles == 1) DOWNLOAD::Progress (DOWNLOAD::asyncHandle);
				else WINDOWS::WINDOW::OnDownloadFinished (window);
			}
		}
	}

	LOGINFO ("Executed Installer Sucessfully.\n");

	{ // Free assets.
		FREE (CONFIGURATION::topConfigsFolderPath);
	}

	return 0;
}
