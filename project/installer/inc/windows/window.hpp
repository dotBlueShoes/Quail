// Made by Matthew Strumillo 2024-12-11
//
#pragma once

#include <blue/types.hpp>
#include <blue/log.hpp>


#include "../res/resource.h"

// AREO
#include <dwmapi.h>
#include <gdiplus.h>
// END AREO

#include <windows.h>
#include <windowsx.h>
#include <commctrl.h>

#include <blue/windows/controls.hpp>

#define ACRYLIC_TINT_LIGHT    0xEEEEEE
#define ACRYLIC_TINT_DARK     0x2B2B2B

namespace WINDOW::AREO {

	enum WINDOWCOMPOSITIONATTRIBUTE {
		WCA_UNDEFINED,
		WCA_NCRENDERING_ENABLED,
		WCA_NCRENDERING_POLICY,
		WCA_TRANSITIONS_FORCEDISABLED,
		WCA_ALLOW_NCPAINT,
		WCA_CAPTION_BUTTON_BOUNDS,
		WCA_NONCLIENT_RTL_LAYOUT,
		WCA_FORCE_ICONIC_REPRESENTATION,
		WCA_EXTENDED_FRAME_BOUNDS,
		WCA_HAS_ICONIC_BITMAP,
		WCA_THEME_ATTRIBUTES,
		WCA_NCRENDERING_EXILED,
		WCA_NCADORNMENTINFO,
		WCA_EXCLUDED_FROM_LIVEPREVIEW,
		WCA_VIDEO_OVERLAY_ACTIVE,
		WCA_FORCE_ACTIVEWINDOW_APPEARANCE,
		WCA_DISALLOW_PEEK,
		WCA_CLOAK,
		WCA_CLOAKED,
		WCA_ACCENT_POLICY,
		WCA_FREEZE_REPRESENTATION,
		WCA_EVER_UNCLOAKED,
		WCA_VISUAL_OWNER,
		WCA_HOLOGRAPHIC,
		WCA_EXCLUDED_FROM_DDA,
		WCA_PASSIVEUPDATEMODE,
		WCA_USEDARKMODECOLORS,
		WCA_CORNER_STYLE,
		WCA_PART_COLOR,
		WCA_DISABLE_MOVESIZE_FEEDBACK,
		WCA_LAST
	};

	struct WINDOWCOMPOSITIONATTRIBUTEDATA {
		WINDOWCOMPOSITIONATTRIBUTE Attribute;
		PVOID pvData;
		SIZE_T cbData;
	};

	enum ACCENT_STATE {
		ACCENT_DISABLED,
		ACCENT_ENABLE_GRADIENT,
		ACCENT_ENABLE_TRANSPARENTGRADIENT,
		ACCENT_ENABLE_BLURBEHIND,
		ACCENT_ENABLE_ACRYLICBLURBEHIND,
		ACCENT_ENABLE_HOSTBACKDROP,
		ACCENT_INVALID_STATE
	};

	enum ACCENT_FLAG {
		ACCENT_NONE,
		ACCENT_WINDOWS11_LUMINOSITY = 0x2,
		ACCENT_BORDER_LEFT = 0x20,
		ACCENT_BORDER_TOP = 0x40,
		ACCENT_BORDER_RIGHT = 0x80,
		ACCENT_BORDER_BOTTOM = 0x100,
		ACCENT_BORDER_ALL = (ACCENT_BORDER_LEFT | ACCENT_BORDER_TOP | ACCENT_BORDER_RIGHT | ACCENT_BORDER_BOTTOM)
	};

	struct ACCENT_POLICY {
		ACCENT_STATE AccentState;
		DWORD AccentFlags;
		DWORD dwGradientColor;
		DWORD dwAnimationId;
	};

	typedef BOOL (WINAPI* PFNSETWINDOWCOMPOSITIONATTRIBUTE)(HWND, WINDOWCOMPOSITIONATTRIBUTEDATA*);

	PFNSETWINDOWCOMPOSITIONATTRIBUTE SetWindowCompositionAttribute;

	void LoadMethods() {
		GetProcMemory (PFNSETWINDOWCOMPOSITIONATTRIBUTE, User32, SetWindowCompositionAttribute);
	}

	void EnableNCRendering (HWND window) {
	    enum DWMNCRENDERINGPOLICY ncrp = DWMNCRP_ENABLED;

	    HRESULT errorCode = DwmSetWindowAttribute (
			window,
	        DWMWA_NCRENDERING_POLICY,
	        &ncrp,
	        sizeof (ncrp)
		);

	    if (errorCode != 0) ERROR ("Failed 0\n");
	}

	void EnableBlurBehind (HWND window) {
	    DWM_BLURBEHIND bb { 0 };

	    bb.dwFlags = DWM_BB_ENABLE;
	    bb.fEnable = true;
	    bb.hRgnBlur = NULL;

	    HRESULT errorCode = DwmEnableBlurBehindWindow (window, &bb);

	    if (errorCode != 0)  ERROR ("Failed 1\n");
	}

	void ExtendIntoClientAll (HWND window) {
	    MARGINS margins { -1 };
	    HRESULT errorCode = DwmExtendFrameIntoClientArea (window, &margins);

	    if (errorCode != 0) ERROR ("Failed 2\n");
	}

	void SetBlurEffect(HWND window, DWORD color) {

		// ISSUE -> ACCENT_ENABLE_ACRYLICBLURBEHIND is laggy when resizing / moving
		//ACCENT_POLICY policy {
		//	ACCENT_ENABLE_ACRYLICBLURBEHIND,
		//	ACCENT_BORDER_ALL,
		//	color,
		//	0
		//};

		//ACCENT_ENABLE_GRADIENT,
		//ACCENT_ENABLE_TRANSPARENTGRADIENT,
		//ACCENT_ENABLE_BLURBEHIND,
		//ACCENT_ENABLE_ACRYLICBLURBEHIND,
		//ACCENT_ENABLE_HOSTBACKDROP,
		
		ACCENT_POLICY policy {
			ACCENT_ENABLE_BLURBEHIND,
			ACCENT_BORDER_ALL,
			color,
			0
		};

		WINDOWCOMPOSITIONATTRIBUTEDATA data {
			WCA_ACCENT_POLICY,
			&policy,
			sizeof(ACCENT_POLICY)
		};

		BOOL errorCode = SetWindowCompositionAttribute(window, &data);

		if (!errorCode) {
			LOGINFO("Something went wrong. SetWindowCompositionAttribute");
		}

		{ // ReAdd Controls
			// Get the current window style
    		LONG_PTR style = GetWindowLongPtr(window, GWL_STYLE);
	
    		// Ensure that the window has a title bar and system menu
    		style |= WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX;
	
    		// Apply the updated style to the window
    		SetWindowLongPtr (window, GWL_STYLE, style);
	
    		// Redraw the window to reflect the style changes
    		//SetWindowPos(window, HWND_TOP, 0, 0, 0, 0, SWP_FRAMECHANGED);
		}
	}
}

namespace WINDOW {

	HBITMAP image = nullptr;
	HWND progressBar, wbNext, wbCancel;
	HFONT font, fontBold;

	s64 WindowLoop (
		HWND window, 
		UINT message, 
		WPARAM wParam, 
		LPARAM lParam
	) {
		switch (message) {

			case WM_CREATE: {
				//SetLayeredWindowAttributes (window, 0, 100, LWA_ALPHA);
				//AREO::EnableNCRendering (window);
    			//AREO::EnableBlurBehind (window);
    			//AREO::ExtendIntoClientAll (window);
			} break;

			case WM_PAINT: {
				HDC	windowContext;
				//RECT windowRect;
				PAINTSTRUCT ps;

        		windowContext = BeginPaint (window, &ps);
				//GetClientRect (window, &windowRect);

				{ // Drawing Image background.
					// 1. We're loading an image into a handle
					// 2. We're coping from said handle (x,y) to window (x,y)
					// 3. We're 
					BITMAP bitmap;

					HDC sourceContext = CreateCompatibleDC (windowContext); // Create a separete context for said bitmap.
					HGDIOBJ sourceBitmap = SelectObject (sourceContext, image); // Keep a handle to the replaced object. // todo: after finished drawing this should be set to it's original value.

        			GetObject (image, sizeof (bitmap), &bitmap); // Load from Handle into an actuall rendering datatype.
        			BitBlt (windowContext, 0, 0, bitmap.bmWidth, bitmap.bmHeight, sourceContext, 0, 0, SRCCOPY); // Copy.
        			//SelectObject (sourceContext, sourceBitmap); // todo: after finished drawing this should be set to it's original value.

        			DeleteDC (sourceContext);
				}

				{ // Drawing Bottom background.
					HBRUSH brushFill, fill;
					HPEN penBorder, border;

					penBorder = CreatePen (PS_SOLID, 1, 0xa0a0a0);
					brushFill = CreateSolidBrush (0xf0f0f0);

					fill = (HBRUSH) SelectObject (windowContext, brushFill); // todo: after finished drawing this should be set to it's original value.
					border = (HPEN) SelectObject (windowContext, penBorder); // todo: after finished drawing this should be set to it's original value.

					Rectangle (windowContext, 0, 314, 496, 314 + 46);
					SelectObject (windowContext, fill); // todo: after finished drawing this should be set to it's original value.
					SelectObject (windowContext, border); // todo: after finished drawing this should be set to it's original value.

					DeleteObject (penBorder);
					DeleteObject (brushFill);
				}

				// Normal Text
				SetTextColor (windowContext, 0x00000000);
				SetBkMode (windowContext, TRANSPARENT);
				SelectFont (windowContext, fontBold); // todo: after finished drawing this should be set to it's original value.

				{ // Header
					const RECT textRegion = { 164 + 16, 16, textRegion.left + 40, textRegion.top + 10 };
					const c16 text[] = L"Welcome to Quail Setup Wizard";
					DrawTextW (windowContext, text, -1, (RECT*) &textRegion, DT_SINGLELINE | DT_NOCLIP);
				}

				SelectFont (windowContext, font);

				{ // Text
					const RECT textRegion = { 164 + 16, 16 + 32, textRegion.left + 40, textRegion.top + 10 };
					const c16 text[] = L"This wizard will guide you through the installation of\nQuail.\n\nClick Next to continue.";
					DrawTextW (windowContext, text, -1, (RECT*) &textRegion, DT_NOCLIP);
				}

				EndPaint (window, &ps);
				

        		
			} break;

			//case WM_PAINT: {
			//	PAINTSTRUCT ps;
			//	HDC hdcParent = BeginPaint(window, &ps);
			//
			//	// Get the device context for the child window
			//	HDC hdcChild = GetDC (windowButton);
			//
			//	// Use BitBlt to copy the child window's contents onto the parent
			//	BitBlt (hdcParent, 0, 0, 100, 100, hdcChild, 0, 0, SRCCOPY);
			//
			//	// Release the device context
			//	ReleaseDC (windowButton, hdcChild);
			//
			//	EndPaint (window, &ps);
			//} break;

			// this
			//case WM_NCHITTEST: {
        	//	return HTCAPTION;
			//}

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
				DeleteObject (image);
				PostQuitMessage (0);
    			return 0;
			} 
		}

		return DefWindowProcW (window, message, wParam, lParam);
	}

	void Create (
		const HINSTANCE& instance,
		HWND& window,
		const s32& isConsole,
		const pair<s32>& position,
		const pair<s32>& size
	) {

		{ // Get Operating System Information.
			OSVERSIONINFO operatingSystem { 0 };
			operatingSystem.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);

			GetVersionEx (&operatingSystem);

			if (operatingSystem.dwMajorVersion <= 4) {
				ERROR ("Running on unsuported windows version!\n");
			}
		}

		c16 windowTitle[] { L"Quail Setup Wizard" };
		c16 windowName[] { L"PropSheetClass" };

		HICON icon;
		//HFONT font;

		{ // ICONS
			icon = LoadIcon (instance, MAKEINTRESOURCE (IDI_ICON_MAIN));
			DEBUG (DEBUG_FLAG_LOGGING) if (icon == nullptr) { LOGERROR ("Could not load icon! %d\n", GetLastError ()); }
		}

		{ // IMAGES
			const c16 pathFile[] = L"C:\\Projects\\Quail\\project\\installer\\res\\wizard-background.bmp";
			image = (HBITMAP) LoadImageW (instance, pathFile, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
			DEBUG (DEBUG_FLAG_LOGGING) if (image == nullptr) { LOGERROR ("Could not load image! %d\n", GetLastError ()); }
		}
		
		{ // FONTS
			const c16 fontName[] = L"Segoe UI";

			fontBold = CreateFontW (
				16, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, 
				ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, 
				DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, 
				fontName
			);

			font = CreateFontW (
				16, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, 
				ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, 
				DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, 
				fontName
			);
		}

		//wcex.hbrBackground = GetStockObject(WHITE_BRUSH);
		//wcex.lpszMenuName = MAKEINTRESOURCE(IDR_MAIN_MENU);
		//wcex.lpszClassName = g_szClassName;

		WNDCLASSEXW windowClass 	{ 0 };
		windowClass.cbSize 			= sizeof (windowClass);
		windowClass.style			= CS_HREDRAW | CS_VREDRAW; // CS_DBLCLKS
		windowClass.lpfnWndProc		= WindowLoop;
		windowClass.hInstance 		= instance;
		windowClass.hIcon 			= icon;
		windowClass.hCursor			= LoadCursor (NULL, IDC_ARROW);
    	//windowClass.hbrBackground = GetSysColorBrush (COLOR_BTNFACE);
		windowClass.hbrBackground	= (HBRUSH) GetStockObject (WHITE_BRUSH);
		//windowClass.lpszMenuName	= MAKEINTRESOURCE(IDR_MAIN_MENU);
		windowClass.lpszClassName 	= windowName;

		if (!RegisterClassExW (&windowClass)) exit (1);

		// Apparently WinAPI `CreateWindow` actually uses said size.x, size.y params as whole size includind non-client area.
		// ... Why and how does I learn about it only now. We're ensuring here that this value coresponds to client area.
		const DWORD style = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;
		RECT rect { 0, 0, size.x, size.y };
    	AdjustWindowRect (&rect, style, FALSE);

		window = CreateWindowExW (
			0, // WS_EX_LAYERED , // requiered for AcrylicEffect.
			windowName,
			windowTitle,
			style,
			position.x,
			position.y,
			rect.right - rect.left, 
			rect.bottom - rect.top,
			nullptr,
			nullptr,
			instance,
			nullptr
		);

		if (!window) exit (1);

    	// Step 6: Make sure the window is hidden (it should not be shown to the user)
    	//ShowWindow(hwnd, SW_HIDE);

    	// Step 7: Now associate the console with the hidden window (optional, to ensure the console uses the icon)
    	SetConsoleTitleW (L"Console with Icon");

		SendMessageW (window, WM_SETFONT, WPARAM (font), TRUE);

		//HWND progressBar = CreateWindowExW (
		//	0, PROGRESS_CLASSW, 
		//	nullptr, 
		//	WS_CHILD | WS_VISIBLE, // PBS_MARQUEE, PBS_SMOOTH, PBS_SMOOTHREVERSE, PBS_VERTICAL
		//	10, 10, 200, 15, 
		//	window, (HMENU) 200, 
		//	instance, 
		//	nullptr
		//);

		
		//WINDOWS::CONTROLS::CreateProgressBarRange (
		//	progressBar, window, instance, 
		//	WS_CHILD | WS_VISIBLE, 
		//	{ 164, 0 }, { 200, 15 }, { 0, 255 }
		//);
		//
		//SendMessageW (progressBar, PBM_SETPOS, (WPARAM) 17, 0); // Sets
		//SendMessageW (progressBar, PBM_DELTAPOS, (WPARAM) 5, 0); // Adds
		////SendMessageW (progressBar, PBM_STEPIT, 0, 0); // Adds a step. (by step variant)

		WINDOWS::CONTROLS::CreateButton (wbNext, window, instance, { 323, 314 + 11 }, { 75, 23 }, L"Next >");
		WINDOWS::CONTROLS::CreateButton (wbCancel, window, instance, { 323 + 75 + 11, 314 + 11 }, { 75, 23 }, L"Cancel");
		
		SendMessageW (wbNext, WM_SETFONT, WPARAM (font), TRUE);
		SendMessageW (wbCancel, WM_SETFONT, WPARAM (font), TRUE);

		//SendMessageW (wbNext, WM_SETFONT, (WPARAM) font, (LPARAM) MAKELONG (TRUE, 0));
		//SendMessageW (wbCancel, WM_SETFONT, (WPARAM) font, (LPARAM) MAKELONG (TRUE, 0));

		//SendMessageW (wbNext, WM_SETFONT, NULL, (LPARAM) MAKELONG (TRUE, 0));
		//SendMessageW (wbCancel, WM_SETFONT, NULL, (LPARAM) MAKELONG (TRUE, 0));

		//windowButton = CreateWindowExW (
		//	0, L"BUTTON",
		//	L"Click Me!",
		//	WS_CHILD | WS_VISIBLE,
		//	20, 20,
		//	100, 40,
		//	window,
		//	nullptr,
		//	instance,
		//	nullptr
		//);

		//AREO::SetBlurEffect (window, ACRYLIC_TINT_DARK);

		ShowWindow (window, isConsole);
		UpdateWindow (window);
	}

}