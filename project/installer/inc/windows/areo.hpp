// Made by Matthew Strumillo 2024-12-11
//
#pragma once

#include <blue/windows/types.hpp>
#include <blue/error.hpp>
#include <blue/types.hpp>
#include <blue/log.hpp>

// AREO
#include <dwmapi.h>
#include <gdiplus.h>
// END AREO

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

	void SetBlurEffect (HWND window, DWORD color) {

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
			sizeof (ACCENT_POLICY)
		};

		BOOL errorCode = SetWindowCompositionAttribute(window, &data);

		if (!errorCode) {
			LOGINFO ("Something went wrong. SetWindowCompositionAttribute");
		}

		{ // ReAdd Controls
			// Get the current window style
    		LONG_PTR style = GetWindowLongPtr (window, GWL_STYLE);
	
    		// Ensure that the window has a title bar and system menu
    		style |= WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX;
	
    		// Apply the updated style to the window
    		SetWindowLongPtr (window, GWL_STYLE, style);
	
    		// Redraw the window to reflect the style changes
    		//SetWindowPos(window, HWND_TOP, 0, 0, 0, 0, SWP_FRAMECHANGED);
		}
	}
}