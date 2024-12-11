// Made by Matthew Strumillo 2024-12-11
//
#pragma once

#include <blue/types.hpp>
#include <blue/log.hpp>


#include "../res/resource.h"

#include <windows.h>
#include <windowsx.h>
#include <commctrl.h>

namespace WINDOW {

	s64 WindowLoop (
		HWND window, 
		UINT message, 
		WPARAM wParam, 
		LPARAM lParam
	) {
		switch (message) {

			case WM_CREATE: {

			} break;

			case WM_COMMAND: {
				const auto command = GET_WM_COMMAND_ID(wParam, lParam);

				switch (command) {
					//case IDM_MODAL:
					//	DoModalPropSheet(hWnd);
					//	break;

					//case IDM_MODELESS:
					//	g_hwndPropSheet = DoModelessPropSheet(hWnd);
					//	break;

					//case IDM_WIZARD:
					//	DoWizardPropSheet(hWnd);
					//	break;

					//case IDM_EXIT:
					//	PostMessage(hWnd, WM_CLOSE, 0, 0);
					//	break;

					//case IDM_ABOUT:
					//	DialogBox(g_hInst, MAKEINTRESOURCE(IDD_ABOUT_DIALOG), hWnd, About);
					//	break;
				}

				return TRUE;
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
		HWND& window,
		s32 isConsole
	) {

		{ // Get Operating System Information.
			OSVERSIONINFO operatingSystem { 0 };
			operatingSystem.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);

			GetVersionEx (&operatingSystem);

			if (operatingSystem.dwMajorVersion <= 4) {
				ERROR ("Running on unsuported windows version!\n");
			}
		}

		c16 windowTitle[] { L"Quail Installer" };
		c16 windowName[] { L"PropSheetClass" };

		const auto& icon = LoadIcon (instance, MAKEINTRESOURCE (IDI_ICON_MAIN));
		DEBUG (DEBUG_FLAG_LOGGING) if (icon == nullptr) { LOGERROR ("Could not load icon! %d\n", GetLastError ()); }

		//wcex.hbrBackground = GetStockObject(WHITE_BRUSH);
		//wcex.lpszMenuName = MAKEINTRESOURCE(IDR_MAIN_MENU);
		//wcex.lpszClassName = g_szClassName;

    	WNDCLASSEXW windowClass 	{ 0 };
		windowClass.cbSize 			= sizeof (windowClass);
		windowClass.style			= CS_HREDRAW | CS_VREDRAW;
		windowClass.lpfnWndProc		= WindowLoop;
    	windowClass.hInstance 		= instance;
    	windowClass.hIcon 			= icon;
		windowClass.hCursor			= LoadCursor(NULL, IDC_ARROW);
    	//windowClass.hbrBackground = GetSysColorBrush (COLOR_BTNFACE);
		windowClass.hbrBackground	= (HBRUSH) GetStockObject(WHITE_BRUSH);
		//windowClass.lpszMenuName	= MAKEINTRESOURCE(IDR_MAIN_MENU);
    	windowClass.lpszClassName 	= windowName;

    	if (!RegisterClassExW (&windowClass)) exit (1);

    	window = CreateWindowExW (
        	0,
        	windowName,
        	windowTitle,
        	WS_OVERLAPPEDWINDOW, // | WS_CLIPCHILDREN | WS_CLIPSIBLINGS,
        	CW_USEDEFAULT,
        	CW_USEDEFAULT,
        	CW_USEDEFAULT,
        	CW_USEDEFAULT,
        	nullptr,
        	nullptr,
        	instance,
        	nullptr
		);

    	if (!window) exit (1);

		ShowWindow (window, isConsole);
		UpdateWindow (window);
	}

}