#include <blue/io.hpp>

#include "windows/console.hpp"
#include "windows/window.hpp"

// TODO
// 2. See how to interpret curl exe code into libcurl syntax https://everything.curl.dev/examples/get.html
// .... registers, window, etc.

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
	// SetConsoleTitleW (L"Console with Icon");

	LOGINFO ("Application Statred!\n");

	{ // PREP. Quail Path buffor.
		ALLOCATE (c16, REGISTRY::topConfigsFolderPath, MAX_PATH);
	}

	//{ // REGISTRY THINGS
	//
	//	// Read these values from UI
	//	REGISTRY::topConfigsFolderPathLength = REGISTRY::DEFAULT_FOLDERPATH_LENGTH;
	//	REGISTRY::topConfigsFolderPath = REGISTRY::DEFAULT_FOLDERPATH_W;
	//	REGISTRY::mainConfigFilePathLength = REGISTRY::DEFAULT_FILEPATH_LENGTH;
	//	REGISTRY::mainConfigFilePath = REGISTRY::DEFAULT_FILEPATH_W;
	//	//
	//
	//	REGISTRY::CreateKeys ();
	//	REGISTRY::AddQuailToPath (REGISTRY::topConfigsFolderPathLength, REGISTRY::topConfigsFolderPath);
	//	
	//	if (!IO::IsExisting(REGISTRY::mainConfigFilePath)) {
	//		IO::Create (REGISTRY::mainConfigFilePath);
	//	}
	//}

	{ // Window Creation.
		
		WINDOWS::CONTROLS::LoadRichEdit ();
		//WINDOW::AREO::LoadMethods ();

		HWND window; WINDOW::Create (instance, window, isConsole, { CW_USEDEFAULT, CW_USEDEFAULT }, { 496, 360 });
		MSG msg { 0 }; 

		while (msg.message != WM_QUIT) { // Main loop
			if (PeekMessage (&msg, NULL, 0U, 0U, PM_REMOVE)) {
				TranslateMessage (&msg);
				DispatchMessage (&msg);
				continue;
			}

			if (WINDOW::isInstalling) {
				if (DOWNLOAD::runningHandles == 1) DOWNLOAD::Progress (DOWNLOAD::asyncHandle);
				else WINDOW::OnDownloadFinished (window);
			}
		}
	}

	//{ // CURL
	//	CURLcode error;
	//	CURL* curl = curl_easy_init ();
	//
	//	if (curl == nullptr) { MSGINFO ("Could not initialize 'CURL' library.\n"); exit (0);} 
	//
	//	{
	//		curl_easy_setopt (curl, CURLOPT_URL, "http://example.com/");
	//
    //		// Perform the request, 'res' holds the return code
    //		error = curl_easy_perform (curl);
	//
    //		// Check for errors
    //		if(error != CURLE_OK) {
	//			MSGERROR ("Could not perform curl action!");
	//			//fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror (error));
	//		}
	//
	//	}
	//
	//	curl_easy_cleanup (curl);
	//}

	LOGINFO ("Executed Installer Sucessfully.\n");
	//getchar ();

	{ // Release Quail Path buffor.
		FREE (REGISTRY::topConfigsFolderPath);
	}

	return 0;
}
