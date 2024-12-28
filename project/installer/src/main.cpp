#include <blue/io.hpp>

#include "windows/console.hpp"
#include "windows/window.hpp"
#include "registry.hpp"

#include <curl/curl.h>

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

	LOGINFO ("Application Statred!\n");

	getchar();

	{ // REGISTRY THINGS
		REGISTRY::topConfigsFolderPathLength = REGISTRY::DEFAULT_FOLDERPATH_LENGTH;
		REGISTRY::mainConfigFilePathLength = REGISTRY::DEFAULT_FILEPATH_LENGTH;
		REGISTRY::mainConfigFilePath = REGISTRY::DEFAULT_FILEPATH_W;

		REGISTRY::CreateKeys ();

		LOGINFO ("1!\n");
		getchar();

		//

		//LOGINFO ("2!\n");
		//getchar();
		
		if (!IO::IsExisting(REGISTRY::mainConfigFilePath)) {
			// TODO. This can really messup thigs. I should get path find my string then add or not if it's present.
			REGISTRY::AddQuailToPath (REGISTRY::topConfigsFolderPathLength, REGISTRY::mainConfigFilePath);
			IO::Create (REGISTRY::mainConfigFilePath);
		}
	}

	LOGINFO ("3!\n");
	getchar();

	{ // Window Creation.
		InitCommonControls (); // Should be replaced with 'InitCommonControlsEx'.

		HWND window; WINDOW::Create (instance, window, isConsole);
		MSG msg { 0 }; 

		while (msg.message != WM_QUIT) { // Main loop
			if (PeekMessage (&msg, NULL, 0U, 0U, PM_REMOVE)) {
				TranslateMessage (&msg);
				DispatchMessage (&msg);
				continue;
			}
		}
	}

	{ // CURL
		CURLcode error;
		CURL* curl = curl_easy_init ();

		if (curl == nullptr) { MSGINFO ("Could not initialize 'CURL' library.\n"); exit (0);} 

		{
			curl_easy_setopt (curl, CURLOPT_URL, "http://example.com/");

    		// Perform the request, 'res' holds the return code
    		error = curl_easy_perform (curl);

    		// Check for errors
    		if(error != CURLE_OK) {
				MSGERROR ("Could not perform curl action!");
				//fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror (error));
			}

		}

		curl_easy_cleanup (curl);
	}

	LOGINFO ("Executed Installer Sucessfully.\n");
	getchar ();

	return 0;
}
