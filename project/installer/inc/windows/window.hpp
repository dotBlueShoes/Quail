// Made by Matthew Strumillo 2024-12-11
//
#pragma once

#include <blue/types.hpp>
#include <blue/log.hpp>


#include "../res/resource.h"

#include <windows.h>

namespace WINDOW {

	s64 WindowLoop (
		HWND window, 
		UINT message, 
		WPARAM wParam, 
		LPARAM lParam
	) {
		switch (message) {

			case WM_CREATE: {
				ShowWindow (window, true);
				return 0;
			}

			case WM_DESTROY: {
				PostQuitMessage(0);
    			return 0;
			}

			default: return DefWindowProcW (window, message, wParam, lParam); 
		}
	}

	void Create (
		HINSTANCE instance,
		HWND& window
	) {

		c16 windowTitle[] { L"Quail Installer" };
		c16 windowName[] { L"GRSsdaEHaRTUwefAGAHEWJRKkakGHDFHdip" };

		const auto& icon = LoadIcon (instance, MAKEINTRESOURCE (IDI_ICON_MAIN));
		DEBUG (DEBUG_FLAG_LOGGING) if (icon == nullptr) { LOGERROR ("Could not load icon! {0}", GetLastError ()); }

    	WNDCLASSEXW windowClass 	{ 0 };
		windowClass.cbSize 			= sizeof (WNDCLASSEXW);
		windowClass.lpfnWndProc		= WindowLoop;
    	windowClass.hInstance 		= instance;
    	//windowClass.hCursor 		= LoadCursorW (nullptr, IDC_ARROW);
    	windowClass.hIcon 			= icon;
    	//windowClass.hbrBackground 	= GetSysColorBrush (COLOR_BTNFACE);
    	windowClass.lpszClassName 	= windowName;

    	if (!RegisterClassExW (&windowClass)) exit (1);

    	//auto pMainWindow = std::unique_ptr<CMainWindow>(new CMainWindow(hInstance, nShowCmd));
    	window = CreateWindowExW (
        	0,
        	windowName,
        	windowTitle,
        	WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS,
        	CW_USEDEFAULT,
        	CW_USEDEFAULT,
        	CW_USEDEFAULT,
        	CW_USEDEFAULT,
        	nullptr,
        	nullptr,
        	instance,
        	nullptr //pMainWindow.get()
		);

    	if (!window) exit (1);
	}

}