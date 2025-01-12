// Created 2025-01-11 by Matthew Strumiłło (dotBlueShoes)
//  LICENSE: GNU GENERAL PUBLIC LICENSE Version 3, 29 June 2007
//
#pragma once

#include <blue/error.hpp>
#include <blue/log.hpp>

#include <curl/curl.h>

#include "registry.hpp"

namespace DOWNLOAD {

	const c8* URL_QUAIL = "https://github.com/dotBlueShoes/MS_Fogger/releases/download/v1.1.2/fogger-1.12.2-1.1.2.0.jar";
	//const c8* URL_QUAIL = "https://github.com/dotBlueShoes/Quail/releases/download/0.4/Quail.exe";
	
	const c16 EXECUTABLE_NAME[] 		= L"QuailNew.exe";
	//const c16 EXECUTABLE_NAME[] 		= L"Quail.exe";
	const u32 EXECUTABLE_NAME_LENGTH 	= sizeof (EXECUTABLE_NAME);

	s32 runningHandles = 1;
	CURLM* asyncHandle;
	CURL* syncHandle;
	FILE* file;

	u64 WriteDataCallback (void* buffer, u64 size, u64 nmemb, FILE *stream) {
		return fwrite (buffer, size, nmemb, stream);
	}

	int CallbackProgress (
		void *clientp, 
		curl_off_t dltotal, // dl - download 
		curl_off_t dlnow, 	//
		curl_off_t ultotal, // ul - upload
		curl_off_t ulnow	// 
	) {
		auto&& progressBar = (HWND) clientp;
		//LOGINFO ("Values: %lld, %lld, %lld, %lld\n", dltotal, dlnow, ultotal, ulnow);

		if (dltotal != 0 && dlnow != 0) {
			u64 current = (r32)dlnow / dltotal * 255;
			LOGINFO ("Current: %lld\n",  current);
			SendMessageW (progressBar, PBM_SETPOS, (WPARAM) current, 0);
		}
		
		return 0; // SUCCESS
	}

	void Create (
		/* OUT */ CURL*& sync, 
		/* OUT */ CURLM*& async, 
		/* OUT */ FILE*& filePath, 
		/* IN  */ const HWND& progressBar
	) {
		curl_global_init (CURL_GLOBAL_DEFAULT);
		async = curl_multi_init ();
		sync = curl_easy_init ();

		if (async == nullptr) ERROR ("curl_multi_init() failed\n");
		if (sync == nullptr) ERROR ("curl_easy_init() failed\n");

		curl_multi_add_handle (async, sync);

		curl_easy_setopt (sync, CURLOPT_URL, URL_QUAIL);						// URL of the file to download.
		curl_easy_setopt (sync, CURLOPT_WRITEFUNCTION, WriteDataCallback); 		// Callback function to write.
		curl_easy_setopt (sync, CURLOPT_WRITEDATA, filePath);            		// File to which we write the data.
		curl_easy_setopt (sync, CURLOPT_NOPROGRESS, 0L);						// Enable progress callback being called.
		curl_easy_setopt (sync, CURLOPT_XFERINFODATA, progressBar);				// Parse Data into CallbackProgress.
		curl_easy_setopt (sync, CURLOPT_XFERINFOFUNCTION, CallbackProgress);	// Callback function for progress.
		curl_easy_setopt (sync, CURLOPT_FOLLOWLOCATION, 1L);					// Make libcurl follow the redirect.
	}

	void Progress (
		/* OUT */ CURLM*& async
	) {
		CURLMcode errorCode = curl_multi_perform (async, &runningHandles);

		if (errorCode == CURLM_OK) {
			/* wait for activity, timeout or "nothing" */
			errorCode = curl_multi_poll (async, NULL, 0, 1000, NULL);
		} else {
			LOGWARN ("curl_multi_poll() failed, code %d.\n", (int) errorCode);
		}
	}

	void Delete (
		/* OUT */ CURL*& sync,
		/* OUT */ CURLM*& async
	) {
		curl_multi_remove_handle (async, sync);
  		curl_multi_cleanup (async);
		curl_easy_cleanup (sync);
  		curl_global_cleanup ();
	}

}