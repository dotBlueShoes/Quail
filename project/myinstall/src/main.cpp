// Created 2024.11.05 by Matthew Strumiłło (dotBlueShoes)
//  LICENSE: GNU GENERAL PUBLIC LICENSE Version 3, 29 June 2007
//
#include <global/logger.hpp>
//
#include <blue/windows/console.hpp>
#include <blue/io.hpp>
//
#include "windows/window.hpp"
#include "installation.hpp"


int WinMain (
	HINSTANCE	instance,
	HINSTANCE	previousInstance,
	LPSTR		commandline,
	s32			isConsole
) {

	UNREFERENCED_PARAMETER (previousInstance);
	UNREFERENCED_PARAMETER (commandline);
    UNREFERENCED_PARAMETER (isConsole);

	{ // Init Logging
		TIMESTAMP_BEGIN = TIMESTAMP::GetCurrent ();
		DEBUG (DEBUG_FLAG_LOGGING) {
			WINDOWS::AttachConsole ();
			putc ('\n', stdout); // Align fututre debug-logs
		}
		LOGINFO ("Application Statred!\n");
	}

	// Check for Admin Rights.
	//IsUserAnAdmin()
	if (!IsUserAnAdmin()) {
		MessageBoxA (nullptr, "Instalator requires admin rights.", "ERROR", MB_OK);
		return -2;
	}

	{ // PREP. Quail path buffer.
		ALLOCATE (c16, CONFIG::topConfigsFolder, MAX_PATH);
	}

	{ // Window Creation.
		WINDOWS::CONTROLS::LoadRichEdit ();

		{ // Properly deallocate data if we hit ERROR.
			MEMORY::EXIT::PUSH (CONFIG::topConfigsFolder, FREE);
		}

		HWND window; WINDOWS::WINDOW::Create (instance, window, isConsole, { CW_USEDEFAULT, CW_USEDEFAULT }, { 496, 360 });
		MSG msg { 0 }; 


		auto&& progressBar = WINDOWS::WINDOW::wpbDownload;

		while (msg.message != WM_QUIT) { // Main loop
			if (PeekMessage (&msg, NULL, 0U, 0U, PM_REMOVE)) {
				TranslateMessage (&msg);
				DispatchMessage (&msg);
				continue;
			}

			switch (INSTALLATION::currentPhase) {

				// TODO: If Else inside can be replaced with a function pointer array.

				case INSTALLATION::PHASE_DOWNLOAD_MAIN: {
					if (DOWNLOAD::runningHandles == 1) { DOWNLOAD::Progress (DOWNLOAD::asyncHandle); }
					else { INSTALLATION::EndPhaseOne (); INSTALLATION::BeginPhaseTwo (progressBar); }
				} break;

				case INSTALLATION::PHASE_DOWNLOAD_UNINSTALLER: {
					if (DOWNLOAD::runningHandles == 1) { DOWNLOAD::Progress (DOWNLOAD::asyncHandle); }
					else { INSTALLATION::EndPhaseTwo (); }
				} break;

				case INSTALLATION::PHASE_CREATE_REGISTRY: 	INSTALLATION::PhaseThree (progressBar);			break;
				case INSTALLATION::PHASE_CREATE_PATH: 		INSTALLATION::PhaseFour (progressBar); 			break;
				case INSTALLATION::PHASE_CREATE_FILES: 		INSTALLATION::PhaseFive (progressBar); 			break;
				case INSTALLATION::PHASE_END: 				WINDOWS::WINDOW::FinishInstallation (window);	break;
				default: 																					break;

			}
		}
	}

	{ // Free assets.
		FREE (CONFIG::topConfigsFolder); 
		// Not needed.
		// MEMORY::EXIT::POP ();
	}

	{ // Deinit Logging
		LOGMEMORY ();
		LOGINFO ("Finalized Execution\n");
		DEBUG (DEBUG_FLAG_LOGGING) {
			putc ('\n', stdout); // Align debug-logs
			Sleep (3000);
		}
	}

	return 0;
}
