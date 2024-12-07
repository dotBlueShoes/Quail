#include <Windows.h>
#include <blue/types.hpp>
#include <blue/log.hpp>

#include <curl/curl.h>

// TODO
// 1. Attach console for debugging
// 2. See how to interpret curl exe code into libcurl syntax https://everything.curl.dev/examples/get.html
// .... registers, window, etc.

int WinMain (
	HINSTANCE	hInstance,
	HINSTANCE	hPrevInstance,
	LPSTR		lpCmdLine,
	int			nShowCmd
) {
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

	MSGINFO ("Executed Installer Sucessfully.\n");

	return 0;
}
