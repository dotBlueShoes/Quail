// Created 2024-12-11 by Matthew Strumiłło (dotBlueShoes)
//  LICENSE: GNU GENERAL PUBLIC LICENSE Version 3, 29 June 2007
//
#pragma once
//
#include <blue/windows/controls.hpp>
#include <blue/types.hpp>
#include <blue/log.hpp>
#include <blue/io.hpp>
//
#include <global/license.h>
//
#include "../installation.hpp"
#include "../localization.hpp"
#include "../res/resource.h"
#include "registry.hpp"
#include "download.hpp"

namespace WINDOWS::WINDOW {

	// IDS
	enum WINDOW_IDS : u16 {
		WINDOW_IDS_LAST					= 1,
		WINDOW_IDS_NEXT					= 2,
		WINDOW_IDS_CANCEL				= 3,
		WINDOW_IDS_BROWSE				= 4,
		WINDOW_IDS_PATH					= 5,
		WINDOW_IDS_COMPONENTS			= 6,
		WINDOW_IDS_LICENSE				= 7,
		WINDOW_IDS_DOWNLOAD				= 8,
	};

	// BUTTON STYLES
	const DWORD BUTTON_STYLE_ACTIVE 	= WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON;
	const DWORD BUTTON_STYLE_VISIBLE 	= WS_CHILD | WS_VISIBLE;
	const DWORD BUTTON_STYLE_OFF 		= WS_CHILD;

	// CUSTOM ACTIVES
	bool isLicenseActive				= false;

	// COLORS
	const COLORREF ACCENT_COLOR			= 0xd77800; // 0xe597b5; -> my system accent color "purple".
	const COLORREF BORDER_INACTIVE		= 0xa0a0a0;
	const COLORREF BACKGROUND_FIRST 	= 0xffffff;
	const COLORREF BACKGROUND_SECONDARY = 0xf0f0f0;
	const COLORREF TEXT_FIRST			= 0x000000;

	// RECTS
	const RECT textDescriptionRegion 	{ 29, 25, textDescriptionRegion.left + 40, textDescriptionRegion.top + 10 };
	const RECT textTagRegion 			{ 16, 5, textTagRegion.left + 40, textTagRegion.top + 10 };

	const pair<s16> rePosition 			{ 29 + 3, 102 + 2 };
	const pair<s16> reSize 				{ 354 - 3, 18 - 1 };
	const RECT reRectPadding { 
		rePosition.x - 1 - 3, 
		rePosition.y - 1 - 2, 
		reSize.x + rePosition.x + 1, 
		reSize.y + rePosition.y + 1
	};

	const u16 wsLeftMargin 				= 10;
	const pair<s16> wsPosition 			{ 30 + wsLeftMargin, 100 };
	const pair<s16> wsSize 				{ 438 - wsLeftMargin, 134 };
	const RECT wsLicensePadding { 
		wsPosition.x - 1 - wsLeftMargin, 
		wsPosition.y - 1, 
		wsSize.x + wsPosition.x + 1, 
		wsSize.y + wsPosition.y + 1
	};

	const pair<s16> wlbMargin 			{ 2, 2 };
	const pair<s16> wlbPosition 		{ 29 + wlbMargin.x, 118 + wlbMargin.y };
	const pair<s16> wlbSize 			{ 438 - wlbMargin.x, 88 - wlbMargin.y };
	const RECT wlbLicensePadding { 
		wlbPosition.x - 1 - wlbMargin.x, 
		wlbPosition.y - 1 - wlbMargin.y, 
		wlbSize.x + wlbPosition.x + 1, 
		wlbSize.y + wlbPosition.y + 1
	};

	const pair<u16> PROGRESSBAR_RANGE 	{ 0, 255 };

	///////////////////////////////////////////////////////////////////////////
	
	// HANDLERS
	HWND wpbDownload, wbLast, wbNext, wbCancel, rePath, wbBrowse, wsLicense, wlbComponents;
	HFONT font, fontBold, fontMono;
	WNDPROC topLicenseControlLoop;
	HBITMAP image = nullptr;

	// PAGES
	enum PAGE_TYPE: u8 {
		PAGE_TYPE_ENTRY 		= 0,
		PAGE_TYPE_LICENSE 		= 1,
		PAGE_TYPE_DIRECTORY 	= 2,
		PAGE_TYPE_REGISTRY  	= 3,
		PAGE_TYPE_CONFIRMATION 	= 4,
		PAGE_TYPE_DOWNLOAD  	= 5,
		PAGE_TYPE_EXIT			= 6,
	}; u8 currentPage = 0;

}


namespace WINDOWS::WINDOW::KEYBOARD {

	//  ABOUT
	// Each page has it's own button selection array.
	//

	const u16 pageActivablesStarts [] {
		 0, // PAGE_TYPE_ENTRY
		 2, // PAGE_TYPE_LICENSE
		 6, // PAGE_TYPE_DIRECTORY
		11, // PAGE_TYPE_REGISTRY
		14, // PAGE_TYPE_CONFIRMATION
		17, // PAGE_TYPE_DOWNLOAD
		18, // PAGE_TYPE_EXIT
	};

	const u16 pageActivablesEnds [] {
		 2, // PAGE_TYPE_ENTRY
		 6, // PAGE_TYPE_LICENSE
		11, // PAGE_TYPE_DIRECTORY
		14, // PAGE_TYPE_REGISTRY
		17, // PAGE_TYPE_CONFIRMATION
		18, // PAGE_TYPE_DOWNLOAD
		19, // PAGE_TYPE_EXIT
	};

	const u16 pageActivablesList [] { 
		WINDOW_IDS_NEXT, WINDOW_IDS_CANCEL,	
		WINDOW_IDS_LICENSE, WINDOW_IDS_LAST, WINDOW_IDS_NEXT, WINDOW_IDS_CANCEL,
		WINDOW_IDS_NEXT, WINDOW_IDS_CANCEL, WINDOW_IDS_PATH, WINDOW_IDS_BROWSE, WINDOW_IDS_LAST,
		WINDOW_IDS_NEXT, WINDOW_IDS_CANCEL, WINDOW_IDS_LAST,
		WINDOW_IDS_NEXT, WINDOW_IDS_CANCEL, WINDOW_IDS_LAST,
		WINDOW_IDS_LAST,
		WINDOW_IDS_CANCEL,
	};

	u16 active = 0;

	void HandleFocusChange (const u16& current, const u16& last) {
		switch (last) {
			case WINDOW_IDS_LAST: {
				SetWindowLongPtr (wbLast, GWL_STYLE, BUTTON_STYLE_VISIBLE);
				InvalidateRect (wbLast, NULL, true);
			} break;

			case WINDOW_IDS_NEXT: {
				if (IsWindowEnabled(wbNext))  {
					SetWindowLongPtr (wbNext, GWL_STYLE, BUTTON_STYLE_VISIBLE);
					InvalidateRect (wbNext, NULL, true);
				}
			} break;

			case WINDOW_IDS_CANCEL: {
				SetWindowLongPtr (wbCancel, GWL_STYLE, BUTTON_STYLE_VISIBLE);
				InvalidateRect (wbCancel, NULL, true);
			} break;

			case WINDOW_IDS_LICENSE: {
				isLicenseActive = false;
				InvalidateRect (GetParent(wsLicense), &wsLicensePadding, true);
			} break;

			case WINDOW_IDS_BROWSE: {
				SetWindowLongPtr (wbBrowse, GWL_STYLE, BUTTON_STYLE_VISIBLE);
				InvalidateRect (wbBrowse, NULL, true);
			} break;

			case WINDOW_IDS_PATH: {
				SetFocus (GetParent(rePath));
				InvalidateRect (GetParent(rePath), &reRectPadding, true);
			} break;

			default: {
				LOGINFO ("n. %d\n", last);
			}
		}

		switch (current) {
			case WINDOW_IDS_LAST: {
				SetWindowLongPtr (wbLast, GWL_STYLE, BUTTON_STYLE_ACTIVE);
				InvalidateRect (wbLast, NULL, true);
			} break;

			case WINDOW_IDS_NEXT: {
				if (IsWindowEnabled(wbNext))  {
					SetWindowLongPtr (wbNext, GWL_STYLE, BUTTON_STYLE_ACTIVE);
					InvalidateRect (wbNext, NULL, true);
				}
			} break;

			case WINDOW_IDS_CANCEL: {
				SetWindowLongPtr (wbCancel, GWL_STYLE, BUTTON_STYLE_ACTIVE);
				InvalidateRect (wbCancel, NULL, true);
			} break;

			case WINDOW_IDS_LICENSE: {
				isLicenseActive = true;
				InvalidateRect (GetParent(wbCancel), &wsLicensePadding, true);
			} break;

			case WINDOW_IDS_BROWSE: {
				SetWindowLongPtr (wbBrowse, GWL_STYLE, BUTTON_STYLE_ACTIVE);
				InvalidateRect (wbBrowse, NULL, true);
			} break;

			case WINDOW_IDS_PATH: {
				SetFocus (rePath);
				InvalidateRect (GetParent(rePath), &reRectPadding, true);
			} break;

			default: {
				LOGINFO ("n. %d\n", current);
			}
		}
	}

}

namespace WINDOWS::WINDOW {

	void DrawPage (const HDC& windowContext) {

		{ // Line
			const pair<u16> origin { 0, 59 }, end { 496, 59 };
			HPEN pen = CreatePen (PS_SOLID, 1, 0xa0a0a0);

			HPEN previousPen = (HPEN) SelectObject (windowContext, pen);

            MoveToEx (windowContext, origin.x, origin.y, NULL);
            LineTo (windowContext, end.x, end.y);

            SelectObject (windowContext, previousPen); // reversing (restoring to original value)
			DeleteObject (pen);
		}

		{ // Rectangle
			const RECT rect { 0, 60, 496, 256 + rect.top };
			HBRUSH brushFill, previousFill;

			brushFill = CreateSolidBrush (BACKGROUND_SECONDARY);
			previousFill = (HBRUSH) SelectObject (windowContext, brushFill);

			FillRect (windowContext, &rect, brushFill);

			SelectObject (windowContext, previousFill); // reversing (restoring to original value)
			DeleteObject (brushFill);
		}

	}


	void DrawFooter (const HDC& windowContext) {
		HBRUSH brushFill, previousFill;
		HPEN penBorder, previousBorder;
		
		brushFill = CreateSolidBrush (BACKGROUND_SECONDARY);
		penBorder = CreatePen (PS_SOLID, 1, BORDER_INACTIVE);
		
		previousFill = (HBRUSH) SelectObject (windowContext, brushFill);
		previousBorder = (HPEN) SelectObject (windowContext, penBorder);
		
		Rectangle (windowContext, 0, 314, 496, 314 + 46);

		SelectObject (windowContext, previousFill); // reversing (restoring to original value)
		SelectObject (windowContext, previousBorder); // reversing (restoring to original value)
		DeleteObject (brushFill);
		DeleteObject (penBorder);
	}


	void DrawEntry (const HDC& windowContext) {

		{ // Drawing Image background.
			BITMAP bitmap;

			HDC sourceContext = CreateCompatibleDC (windowContext); // Create a separate context for said bitmap.
			HGDIOBJ previousBitmap = SelectObject (sourceContext, image); // Keep a handle to the replaced object.

        	GetObject (image, sizeof (bitmap), &bitmap); // Load from Handle into an actual rendering datatype.
        	BitBlt (windowContext, 0, 0, bitmap.bmWidth, bitmap.bmHeight, sourceContext, 0, 0, SRCCOPY); // Copy.

        	SelectObject (sourceContext, previousBitmap); // reversing (restoring to original value)
        	DeleteDC (sourceContext);
		}

		// Header Text
		HFONT previousFont = SelectFont (windowContext, fontBold);
		SetTextColor (windowContext, TEXT_FIRST); // TODO: why every draw?
		SetBkMode (windowContext, TRANSPARENT);   // TODO: why every draw?

		{ // Text Control
			const RECT textRegion = { 164 + 16, 16, textRegion.left + 40, textRegion.top + 10 };
			DrawTextW (windowContext, LOCAL::EntryWelcome, -1, (RECT*) &textRegion, DT_SINGLELINE | DT_NOCLIP);
		}

		// Normal Text
		SelectFont (windowContext, font);

		{ // Text Control
			const RECT textRegion = { 164 + 16, 16 + 32, textRegion.left + 40, textRegion.top + 10 };
			DrawTextW (windowContext, LOCAL::EntryText, -1, (RECT*) &textRegion, DT_NOCLIP);
		}

		SelectFont (windowContext, previousFont); // reversing (restoring to original value)

	}


	void DrawDirectory (const HDC& windowContext) {

		{ // Drawing RICHEDIT outer.
			HBRUSH brushFill, previousFill;
			HPEN penBorder, previousBorder;
		
			brushFill = CreateSolidBrush (BACKGROUND_FIRST);

			const auto focussedWindow = GetFocus ();

			if (focussedWindow == rePath) 	penBorder = CreatePen (PS_SOLID, 1, ACCENT_COLOR);
			else 							penBorder = CreatePen (PS_SOLID, 1, BORDER_INACTIVE);
		
			previousFill = (HBRUSH) SelectObject (windowContext, brushFill);
			previousBorder = (HPEN) SelectObject (windowContext, penBorder);
		
			Rectangle (
				windowContext, 
				reRectPadding.left, 
				reRectPadding.top, 
				reRectPadding.right, 
				reRectPadding.bottom
			);

			SelectObject (windowContext, previousFill); 	// reversing (restoring to original value)
			SelectObject (windowContext, previousBorder); 	// reversing (restoring to original value)
			DeleteObject (brushFill);
			DeleteObject (penBorder);
		}

		// Header Text
		HFONT previousFont = SelectFont (windowContext, fontBold);
		SetBkMode (windowContext, TRANSPARENT);   // TODO: why every draw?
		SetTextColor (windowContext, TEXT_FIRST); // TODO: why every draw?

		// Text Control
		DrawTextW (windowContext, LOCAL::TagDirectory, -1, (RECT*) &textTagRegion, DT_SINGLELINE | DT_NOCLIP);

		SelectFont (windowContext, font);

		// Text Control
		DrawTextW (windowContext, LOCAL::DescriptionDirectory, -1, (RECT*) &textDescriptionRegion, DT_NOCLIP);

		{ // Text Control
			const RECT textRegion = { 29, 75, textRegion.left + 40, textRegion.top + 14 };
			DrawTextW (windowContext, LOCAL::BrowseTip, -1, (RECT*) &textRegion, DT_SINGLELINE | DT_NOCLIP);
		}

		{
			const RECT textRegion = { 29, 294, textRegion.left + 40, textRegion.top + 14 };
    		swprintf (LOCAL::diskSpace + LOCAL::DISK_SPACE_1_SIZE, LOCAL::MAX_EXE_SIZE, L"%.2f KB", DISK_SPACE_KB);
    		DrawTextW (windowContext, LOCAL::diskSpace, -1, (RECT*) &textRegion, DT_SINGLELINE | DT_NOCLIP);
		}

		SelectFont (windowContext, previousFont);

	}


	void DrawLicense (const HDC& windowContext) {

		{ // Drawing RICHEDIT outer.
			HBRUSH brushFill, previousFill;
			HPEN penBorder, previousBorder;
		
			brushFill = CreateSolidBrush (BACKGROUND_FIRST);

			if (isLicenseActive) penBorder = CreatePen (PS_SOLID, 1, ACCENT_COLOR);
			else penBorder = CreatePen (PS_SOLID, 1, BORDER_INACTIVE);
		
			previousFill = (HBRUSH) SelectObject (windowContext, brushFill);
			previousBorder = (HPEN) SelectObject (windowContext, penBorder);
		
			Rectangle (
				windowContext, 
				wsLicensePadding.left, 
				wsLicensePadding.top, 
				wsLicensePadding.right, 
				wsLicensePadding.bottom
			);

			SelectObject (windowContext, previousFill); 	// reversing (restoring to original value)
			SelectObject (windowContext, previousBorder); 	// reversing (restoring to original value)
			DeleteObject (brushFill);
			DeleteObject (penBorder);
		}

		// Header Text
		HFONT previousFont = SelectFont (windowContext, fontBold);
		SetBkMode (windowContext, TRANSPARENT);   // TODO: why every draw?
		SetTextColor (windowContext, TEXT_FIRST); // TODO: why every draw?

		// Text Control
		DrawTextW (windowContext, LOCAL::TagLicense, -1, (RECT*) &textTagRegion, DT_SINGLELINE | DT_NOCLIP);

		SelectFont (windowContext, font);

		// Text Control
		DrawTextW (windowContext, LOCAL::DescriptionLicense, -1, (RECT*) &textDescriptionRegion, DT_NOCLIP);

		{ // Text Control
			const RECT textRegion = { 29, 75, textRegion.left + 40, textRegion.top + 10 };
			DrawTextW (windowContext, LOCAL::LicenseTop, -1, (RECT*) &textRegion, DT_NOCLIP);
		}

		{ // Text Control
			const RECT textRegion = { 29, 257, textRegion.left + 40, textRegion.top + 10 };
			DrawTextW (windowContext, LOCAL::LicenseBot, -1, (RECT*) &textRegion, DT_NOCLIP);
		}

		SelectFont (windowContext, previousFont);

	}


	void DrawRegistry (const HDC& windowContext) {

		{ // Drawing RICHEDIT outer.
			HBRUSH brushFill, previousFill;
			HPEN penBorder, previousBorder;
		
			brushFill = CreateSolidBrush (BACKGROUND_FIRST);
			penBorder = CreatePen (PS_SOLID, 1, BORDER_INACTIVE);
		
			previousFill = (HBRUSH) SelectObject (windowContext, brushFill);
			previousBorder = (HPEN) SelectObject (windowContext, penBorder);
		
			Rectangle (
				windowContext, 
				wlbLicensePadding.left, 
				wlbLicensePadding.top, 
				wlbLicensePadding.right, 
				wlbLicensePadding.bottom
			);

			SelectObject (windowContext, previousFill); 	// reversing (restoring to original value)
			SelectObject (windowContext, previousBorder); 	// reversing (restoring to original value)
			DeleteObject (brushFill);
			DeleteObject (penBorder);
		}

		// Header Text
		HFONT previousFont = SelectFont (windowContext, fontBold);
		SetBkMode (windowContext, TRANSPARENT);   // TODO: why every draw?
		SetTextColor (windowContext, TEXT_FIRST); // TODO: why every draw?
		
		// Text Control
		DrawTextW (windowContext, LOCAL::TagRegistry, -1, (RECT*) &textTagRegion, DT_SINGLELINE | DT_NOCLIP);

		SelectFont (windowContext, font);

		// Text Control
		DrawTextW (windowContext, LOCAL::DescriptionRegistry, -1, (RECT*) &textDescriptionRegion, DT_NOCLIP);

		{ // Text Control
			const RECT textRegion = { 29, 75, textRegion.left + 40, textRegion.top + 10 };
			DrawTextW (windowContext, LOCAL::RegistryTop, -1, (RECT*) &textRegion, DT_NOCLIP);
		}

		SelectFont (windowContext, previousFont);

	}


	void DrawConfirmation (const HDC& windowContext) {

		// Header Text
		HFONT previousFont = SelectFont (windowContext, fontBold);
		SetBkMode (windowContext, TRANSPARENT);   // TODO: why every draw?
		SetTextColor (windowContext, TEXT_FIRST); // TODO: why every draw?

		// Text Control
		DrawTextW (windowContext, LOCAL::TagConfirmation, -1, (RECT*) &textTagRegion, DT_SINGLELINE | DT_NOCLIP);

		SelectFont (windowContext, font);

		// Text Control
		DrawTextW (windowContext, LOCAL::DescriptionConfirmation, -1, (RECT*) &textDescriptionRegion, DT_NOCLIP);

		{ // TOP MSG
		
		 	const RECT textRegion = { 
				29, 75, 
				textRegion.left + 500, 
				textRegion.top + (14 * 3) 
			};
			
    	 	const s32 stringLength = swprintf (
				LOCAL::confirmationTop - 1 + LOCAL::CONFIRMATION_TOP_1_SIZE + CONFIG::QUAIL_NAME_VERSION_SIZE + LOCAL::CONFIRMATION_TOP_2_SIZE, 
				MAX_PATH, 
				L"%s\".", CONFIG::topConfigsFolder
			);

			{ // Emplace a single new line symbol if the text is to long.
				const u8 MAX_CHARACTERS_IN_LINE = 86;

				if (stringLength > MAX_CHARACTERS_IN_LINE) {

					// Issue: Possible Memory leak. // Move all by one
					for (u16 i = stringLength; i > MAX_CHARACTERS_IN_LINE; --i) {
						LOCAL::confirmationTop[i] = LOCAL::confirmationTop[i - 1];
					}

					LOCAL::confirmationTop[MAX_CHARACTERS_IN_LINE] = L'\n';
				} 
			}

    	 	DrawTextW (windowContext, LOCAL::confirmationTop, -1, (RECT*) &textRegion, DT_WORDBREAK);
		}


		if (INSTALLATION::isRegistry) {
			const RECT textRegion = { 29, 75 + 56, textRegion.left + 500, textRegion.top + 14 };
			DrawTextW (windowContext, LOCAL::CONFIRMATION_REGISTRY, -1, (RECT*) &textRegion, DT_NOCLIP);
		}

		if (INSTALLATION::isPath) {
			const RECT textRegion = { 29, 75 + 56 + 28, textRegion.left + 500, textRegion.top + 14 };
			DrawTextW (windowContext, LOCAL::CONFIRMATION_PATH, -1, (RECT*) &textRegion, DT_NOCLIP);
		}

		if (INSTALLATION::isBatch) {
			const RECT textRegion = { 29, 75 + 56 + 28 + 28, textRegion.left + 500, textRegion.top + 14 };
			DrawTextW (windowContext, LOCAL::CONFIRMATION_BATCH, -1, (RECT*) &textRegion, DT_NOCLIP);
		}

		if (CONFIG::isForceC8Display) {
			const RECT textRegion = { 29, 75 + 56 + 28 + 28 + 28, textRegion.left + 500, textRegion.top + 14 };
			DrawTextW (windowContext, LOCAL::CONFIRMATION_FORCE_C8, -1, (RECT*) &textRegion, DT_NOCLIP);
		}


		if (INSTALLATION::currentPhase == INSTALLATION::PHASE_NONE) { // BOT MSG
			const RECT textRegion = { 29, 280, textRegion.left + 40, textRegion.top + 14 };
			DrawTextW (windowContext, LOCAL::ConfirmationBot, -1, (RECT*) &textRegion, DT_NOCLIP);
		}

		SelectFont (windowContext, previousFont);

	}


	void DrawDownload (const HDC& windowContext) {

		// Header Text
		HFONT previousFont = SelectFont (windowContext, fontBold);
		SetBkMode (windowContext, TRANSPARENT);   // TODO: why every draw?
		SetTextColor (windowContext, TEXT_FIRST); // TODO: why every draw?

		// Text Control
		DrawTextW (windowContext, LOCAL::TagDownload, -1, (RECT*) &textTagRegion, DT_SINGLELINE | DT_NOCLIP);

		SelectFont (windowContext, font);

		// Text Control
		DrawTextW (windowContext, LOCAL::DescriptionDownload, -1, (RECT*) &textDescriptionRegion, DT_NOCLIP);

		SelectFont (windowContext, fontMono);

		switch (INSTALLATION::currentPhase) { // Text Control 

			case INSTALLATION::PHASE_DOWNLOAD_MAIN: {
				DrawTextW (windowContext, LOCAL::InstallerTagDownloading, -1, (RECT*) &INSTALLATION::CAPTION_REGION, DT_NOCLIP);
				DrawTextA (windowContext, CONFIG::URL_QUAIL_EXECUTABLE, -1, (RECT*) &INSTALLATION::TEXT_REGION, 0);
				DrawTextA (windowContext, "...", -1, (RECT*) &INSTALLATION::REST_REGION, DT_NOCLIP);
			} break;

			case INSTALLATION::PHASE_DOWNLOAD_UNINSTALLER: {
				DrawTextW (windowContext, LOCAL::InstallerTagDownloading, -1, (RECT*) &INSTALLATION::CAPTION_REGION, DT_NOCLIP);
				DrawTextA (windowContext, CONFIG::URL_QUAIL_UNINSTALLER, -1, (RECT*) &INSTALLATION::TEXT_REGION, 0);
				DrawTextA (windowContext, "...", -1, (RECT*) &INSTALLATION::REST_REGION, DT_NOCLIP);
			} break;

			case INSTALLATION::PHASE_CREATE_REGISTRY: {
				DrawTextW (windowContext, LOCAL::InstallerTagRegistry, -1, (RECT*) &INSTALLATION::CAPTION_REGION, DT_NOCLIP);
				DrawTextA (windowContext, LOCAL::INSTALL_TASK_REGISTRY, -1, (RECT*) &INSTALLATION::TEXT_REGION, 0);
			} break;

			case INSTALLATION::PHASE_CREATE_PATH: {
				DrawTextW (windowContext, LOCAL::InstallerTagRegistry, -1, (RECT*) &INSTALLATION::CAPTION_REGION, DT_NOCLIP);
				DrawTextA (windowContext, LOCAL::INSTALL_TASK_PATH, -1, (RECT*) &INSTALLATION::TEXT_REGION, 0);
			} break;

			case INSTALLATION::PHASE_CREATE_FILES: {
				DrawTextW (windowContext, LOCAL::InstallerTagFiles, -1, (RECT*) &INSTALLATION::CAPTION_REGION, DT_NOCLIP);
				DrawTextA (windowContext, LOCAL::INSTALL_TASK_FILES, -1, (RECT*) &INSTALLATION::TEXT_REGION, 0);
			} break;

			default: break;
		}

		SelectFont (windowContext, previousFont);

	}


	void DrawExit (const HDC& windowContext) {

		{ // Drawing Image background.
			BITMAP bitmap;

			HDC sourceContext = CreateCompatibleDC (windowContext); // Create a separate context for said bitmap.
			HGDIOBJ previousBitmap = SelectObject (sourceContext, image); // Keep a handle to the replaced object.

        	GetObject (image, sizeof (bitmap), &bitmap); // Load from Handle into an actual rendering datatype.
        	BitBlt (windowContext, 0, 0, bitmap.bmWidth, bitmap.bmHeight, sourceContext, 0, 0, SRCCOPY); // Copy.

        	SelectObject (sourceContext, previousBitmap); // reversing (restoring to original value)
        	DeleteDC (sourceContext);
		}

		{
			// Header Text
			HFONT previousFont = SelectFont (windowContext, fontBold);
			SetTextColor (windowContext, TEXT_FIRST); // TODO: why every draw?
			SetBkMode (windowContext, TRANSPARENT);   // TODO: why every draw?

			{ // Text Control
				const RECT textRegion = { 164 + 16, 16, textRegion.left + 40, textRegion.top + 10 };
				DrawTextW (windowContext, LOCAL::ExitTag, -1, (RECT*) &textRegion, DT_SINGLELINE | DT_NOCLIP);
			}

			// Normal Text
			SelectFont (windowContext, font);

			{ // Text Control
				const RECT textRegion = { 164 + 16, 16 + 32, textRegion.left + 296, textRegion.top + (14 * 2) };
				DrawTextW (windowContext, LOCAL::ExitText, -1, (RECT*) &textRegion, DT_WORDBREAK);
			}

			SelectFont (windowContext, previousFont);
		}

	}


	bool SyncValidateRichEditPath () {
		CONFIG::topConfigsFolderLength = GetWindowTextLengthW (rePath) + 1;
		GetWindowTextW (rePath, CONFIG::topConfigsFolder, CONFIG::topConfigsFolderLength);
		CONFIG::topConfigsFolderLength *= 2; // wide-character

		LOGINFO ("SyncValidateRichEditPath () Call\n");
		LOGINFO ("path: %ls\n", CONFIG::topConfigsFolder);

		return IO::IsValidDirectory (CONFIG::topConfigsFolder, CONFIG::topConfigsFolderLength);
	}


	void FinishInstallation (HWND& window) {

		INSTALLATION::currentPhase = INSTALLATION::PHASE_NONE;
		currentPage = PAGE_TYPE_EXIT - 1;

		// We're simply simulating a wbNext Click Msg.
		SendMessageW (window, WM_COMMAND, MAKEWPARAM (GetDlgCtrlID (wbNext), BN_CLICKED), (LPARAM) wbNext);

	}

}











namespace WINDOWS::WINDOW {

	void ScrollBarEvent (const HWND& window) { // Once License is read we don't check again for it.
		if (!INSTALLATION::isLicense) {
			INSTALLATION::isLicense = WINDOWS::CONTROLS::IsVerticalScrollbarAtMax (wsLicense, wsSize.y);

			if (INSTALLATION::isLicense) { // Trigger when scrollbar hit it's y's max value.
				const auto& rect = wsLicensePadding;
						
				// Invalidate top border (1 pixel wide).
    			const RECT topBorder = { rect.left, rect.top, rect.right, rect.top + 1 };
    			InvalidateRect (GetParent (window), &topBorder, FALSE);

    			// Invalidate bottom border (1 pixel wide).
    			const RECT bottomBorder = { rect.left, rect.bottom - 1, rect.right, rect.bottom };
    			InvalidateRect (GetParent (window), &bottomBorder, FALSE);

    			// Invalidate left border (1 pixel wide).
    			const RECT leftBorder = { rect.left, rect.top, rect.left + 1, rect.bottom };
    			InvalidateRect (GetParent (window), &leftBorder, FALSE);

    			// Invalidate right border (1 pixel wide).
    			const RECT rightBorder = { rect.right - 1, rect.top, rect.right, rect.bottom };
    			InvalidateRect (GetParent (window), &rightBorder, FALSE);

				EnableWindow (wbNext, true);
			}
		}
	}


	s64 LicenseControlLoop (
		HWND window, 
		UINT message, 
		WPARAM wParam, 
		LPARAM lParam
	) {
		
		switch (message) {

			// Weirdly WM_VSCROLL
			// 1. Is not sent to it's Parent window.
			// 2. Is not being created via WM_MOUSEWHEEL, WM_MBUTTONDOWN, WM_MBUTTONUP.
			//  - This code re-implements said functionality.

			case WM_MOUSEWHEEL: {
				ScrollBarEvent (window);
			} break;

			case WM_MBUTTONDOWN: {
				ScrollBarEvent (window);
			} break;

			case WM_MBUTTONUP: {
				ScrollBarEvent (window);
			} break;
            
			case WM_VSCROLL: {
				ScrollBarEvent (window);
			} break;
		}

		// Call the original RichEdit window procedure for other messages.
		return CallWindowProcW (topLicenseControlLoop, window, message, wParam, lParam);
	}


	void HandlePageSwitch (const HWND& window) {

		LOGINFO ("...page switch\n");

		switch (currentPage) { 

			case PAGE_TYPE_ENTRY: {

				{ // THIS
					//KEYBOARD::HandleFocusChange (WINDOW_IDS_NEXT, WINDOW_IDS_LAST);
					ShowWindow (wbLast, HIDE_WINDOW);
				}
				
				{ // Next
					EnableWindow (wbNext, true);
					SendMessageW (wbNext, WM_SETTEXT, 0, (u64)LOCAL::BUTTON_NEXT);
					ShowWindow (wsLicense, HIDE_WINDOW);
				}

			} break;

			case PAGE_TYPE_LICENSE: {

				{ // PREV
					ShowWindow (wbLast, SHOW_OPENWINDOW);
				}

				{ // THIS
					//KEYBOARD::HandleFocusChange (WINDOW_IDS_NEXT, WINDOW_IDS_LAST);
					isLicenseActive = true;
					//
					EnableWindow (wbNext, INSTALLATION::isLicense);
					SendMessageW (wbNext, WM_SETTEXT, 0, (u64)LOCAL::BUTTON_AGREE);
					ShowWindow (wsLicense, SHOW_OPENWINDOW);
				}

				{ // NEXT
					ShowWindow (wbBrowse, HIDE_WINDOW);
					ShowWindow (rePath, HIDE_WINDOW);
				}

			} break;

			case PAGE_TYPE_DIRECTORY: {

				{ // PREV
					SendMessageW (wbNext, WM_SETTEXT, 0, (u64)LOCAL::BUTTON_NEXT);
					ShowWindow (wsLicense, HIDE_WINDOW);
				}

				{ // THIS
					LOGINFO ("directory\n");
					//KEYBOARD::HandleFocusChange (WINDOW_IDS_PATH, WINDOW_IDS_LAST);
					ShowWindow (wlbComponents, HIDE_WINDOW);
					ShowWindow (wbBrowse, SHOW_OPENWINDOW);
					ShowWindow (rePath, SHOW_OPENWINDOW);
				}
				
			} break;

			case PAGE_TYPE_REGISTRY: {

				{ // PREV
					ShowWindow (wbBrowse, HIDE_WINDOW);
					ShowWindow (rePath, HIDE_WINDOW);
				}

				{ // THIS
					//KEYBOARD::HandleFocusChange (WINDOW_IDS_NEXT, WINDOW_IDS_LAST);
					ShowWindow (wlbComponents, SHOW_OPENWINDOW);
				}

				{ // NEXT
					SendMessageW (wbNext, WM_SETTEXT, 0, (u64)LOCAL::BUTTON_NEXT);
				}
				
			} break;

			case PAGE_TYPE_CONFIRMATION: {

				{ // PREV
					ShowWindow (wlbComponents, HIDE_WINDOW);
				}

				{ // THIS
					if (INSTALLATION::currentPhase == INSTALLATION::PHASE_NONE) {
						SendMessageW (wbNext, WM_SETTEXT, 0, (u64)LOCAL::BUTTON_START);
					} else {
						KEYBOARD::active = KEYBOARD::pageActivablesStarts[PAGE_TYPE_CONFIRMATION];
						//KEYBOARD::HandleFocusChange (WINDOW_IDS_NEXT, WINDOW_IDS_LAST);
						EnableWindow (wbLast, false);
					}
				}

				{ // NEXT
					ShowWindow (wpbDownload, HIDE_WINDOW);
					EnableWindow (wbNext, true);
				}
				
			} break;

			case PAGE_TYPE_DOWNLOAD: {

				{ // PREV
					SendMessageW (wbNext, WM_SETTEXT, 0, (u64) LOCAL::BUTTON_NEXT);
					EnableWindow (wbLast, true);
				}

				{ // THIS
					//KEYBOARD::HandleFocusChange (WINDOW_IDS_LAST, WINDOW_IDS_NEXT);
					ShowWindow (wpbDownload, SHOW_OPENWINDOW);
					EnableWindow (wbCancel, false);
					EnableWindow (wbNext, false);

					if (INSTALLATION::currentPhase == INSTALLATION::PHASE_NONE) {
						INSTALLATION::BeginPhaseOne (wpbDownload);
					}

					if (INSTALLATION::currentPhase == INSTALLATION::PHASE_END) {
						SendMessageW (window, WM_COMMAND, MAKEWPARAM (GetDlgCtrlID (wbNext), BN_CLICKED), (LPARAM) wbNext); // We're simply simulating a wbNext Click Msg.
					}
				}
				
			} break;

			case PAGE_TYPE_EXIT: {

				{ // PREV
					EnableWindow (wbCancel, true);
					
					ShowWindow (wpbDownload, HIDE_WINDOW);
					ShowWindow (wbLast, HIDE_WINDOW);
					ShowWindow (wbNext, HIDE_WINDOW);
				}

				{ // THIS
					SendMessageW (wbCancel, WM_SETTEXT, 0, (u64) LOCAL::BUTTON_FINISH); // Change "Close" msg to "Finish".
					
					{ // Make Finish Button now the default button.
						DWORD style = GetWindowLongPtr (wbCancel, GWL_STYLE);
						style |= BS_DEFPUSHBUTTON;
						SetWindowLongPtr (wbCancel, GWL_STYLE, style);
						
						InvalidateRect (wbCancel, nullptr, FALSE);
					}
				}

			} break;

		}
	}


	void PageAdd () {
		const auto last = KEYBOARD::pageActivablesList[KEYBOARD::active];
		currentPage++;
		KEYBOARD::active = KEYBOARD::pageActivablesStarts[currentPage];
		const auto current = KEYBOARD::pageActivablesList[KEYBOARD::active];
		KEYBOARD::HandleFocusChange (current, last);
	}


	void PageSub () {
		const auto& last = KEYBOARD::pageActivablesList[KEYBOARD::active];
		currentPage--;
		KEYBOARD::active = KEYBOARD::pageActivablesStarts[currentPage];
		const auto& current = KEYBOARD::pageActivablesList[KEYBOARD::active];
		KEYBOARD::HandleFocusChange (current, last);
	}


	void OnButtonLast (const HWND& window) {
		if (currentPage == PAGE_TYPE_DIRECTORY) {
			
			if (SyncValidateRichEditPath ()) {
				LOGINFO ("Selected a valid directory for installation.\n");
				PageSub ();
				InvalidateRect (window, NULL, true); 	// Make the whole window redraw itself. Also clears previous draw.
			} else { MSGERROR (LOCAL::MSG_INVALID_DIR_PATH); }

		} else {
			PageSub ();
			InvalidateRect (window, NULL, true); 		// Make the whole window redraw itself. Also clears previous draw.
		}

		SendMessageW (wbLast, BM_SETSTATE, FALSE, 0); 	// Release the pressed state
	}


	void OnButtonNext (const HWND& window) {
		if (currentPage == PAGE_TYPE_DIRECTORY) {

			if (SyncValidateRichEditPath ()) {
				LOGINFO ("Selected a valid directory for installation.\n");
				PageAdd ();
				InvalidateRect (window, NULL, true); 	// Make the whole window redraw itself. Also clears previous draw.
			} else { MSGERROR (LOCAL::MSG_INVALID_DIR_PATH); }

		} else {
			PageAdd ();
			InvalidateRect (window, NULL, true); 		// Make the whole window redraw itself. Also clears previous draw.
		}

		SetFocus (window); 								// Issue. Something else breaks and its a quick fix. TODO describe what...

		SendMessageW (wbLast, BM_SETSTATE, FALSE, 0); 	// Release the pressed state
	}


	void OnButtonCancel (const HWND& window) {
		if (currentPage != PAGE_TYPE_EXIT) {
			s32 response = MessageBoxW (
				nullptr, LOCAL::MSG_CANCEL_CONFIRM, LOCAL::WINDOW_TITLE, MB_YESNO | MB_ICONQUESTION
			);

			if (response == IDYES) { 
				SendMessageW (window, WM_CLOSE, 0, 0);
				SendMessageW (wbLast, BM_SETSTATE, FALSE, 0); 	// Release the pressed state
			}
		} else {
			SendMessageW (window, WM_CLOSE, 0, 0);
			SendMessageW (wbLast, BM_SETSTATE, FALSE, 0); 	// Release the pressed state
		}
	}


	void OnButtonBrowse (const HWND& window) {
		c16* tempBuffer = nullptr;

		if (SUCCEEDED (WINDOWS::CONTROLS::BrowseFolder (window, nullptr, tempBuffer, MAX_PATH))) {

			// SET.
			CONFIG::topConfigsFolderLength = (wcslen (tempBuffer) + 1) * 2;					// Calculate the actual length in byes.
			memcpy (CONFIG::topConfigsFolder, tempBuffer, CONFIG::topConfigsFolderLength);	// Copy to my own memory.
			SetWindowTextW (rePath, CONFIG::topConfigsFolder); 								// Update RichEdit Control
			CoTaskMemFree (tempBuffer);														// Release String created via BrowseFolder Control.
		}
	}


	void HandleButtonPresses (const HWND& window, const u16& buttonId) {
		switch (buttonId) {
			case WINDOW_IDS_LAST: 	OnButtonLast 	(window); break;
			case WINDOW_IDS_NEXT: 	OnButtonNext 	(window); break;
			case WINDOW_IDS_CANCEL: OnButtonCancel 	(window); break;
			case WINDOW_IDS_BROWSE: OnButtonBrowse 	(window); break;
		}
	}


	s64 WindowLoop (
		HWND window, 
		UINT message, 
		WPARAM wParam, 
		LPARAM lParam
	) {
		switch (message) {

			case WM_CREATE: {

				const HINSTANCE instance = GetWindowInstance (window);

				{ // Extra controls initialization
					INITCOMMONCONTROLSEX commonControls;
					commonControls.dwSize = sizeof (commonControls);
		
					commonControls.dwICC = ICC_BAR_CLASSES | ICC_LISTVIEW_CLASSES;
					InitCommonControlsEx (&commonControls);
				}

				WINDOWS::CONTROLS::CreateProgressBarRange (
					wpbDownload, window, WINDOW_IDS_DOWNLOAD, instance, 
					WS_CHILD, 
					{ 28, 60 + 55 }, { 440, 21 }, PROGRESSBAR_RANGE
				);

				{ // ListView
            		wlbComponents = CreateWindowExW (
            		    0, WC_LISTVIEWW, nullptr,
            		    WS_CHILD | LVS_LIST | WS_VISIBLE, //  | LVS_SINGLESEL
            		    wlbPosition.x, wlbPosition.y, wlbSize.x, wlbSize.y,
            		    window, (HMENU) WINDOW_IDS_COMPONENTS,
            		    instance, nullptr
					);

					// Adds Checkbox for each item in the list.
					SendMessageW (wlbComponents, LVM_SETEXTENDEDLISTVIEWSTYLE, NULL, LVS_EX_CHECKBOXES);

            		// Add items to the ListView.
					LVITEMW lvItem = { 0 };
            		lvItem.mask = LVIF_TEXT;
            		lvItem.iSubItem = 0;

					lvItem.pszText = (LPWSTR) LOCAL::LVForceC8Display;
            		SendMessageW (wlbComponents, LVM_INSERTITEMW, 3, (LPARAM) &lvItem);
					lvItem.pszText = (LPWSTR) LOCAL::LVBatch;
            		SendMessageW (wlbComponents, LVM_INSERTITEMW, 2, (LPARAM) &lvItem);
            		lvItem.pszText = (LPWSTR) LOCAL::LVRegistry;
            		SendMessageW (wlbComponents, LVM_INSERTITEMW, 1, (LPARAM) &lvItem);
					lvItem.pszText = (LPWSTR) LOCAL::LVPath;
            		SendMessageW (wlbComponents, LVM_INSERTITEMW, 0, (LPARAM) &lvItem);
					

					// Issue. Removing 'WS_VISIBLE' from Creation makes text cropped for some reason.
					// Keeping it and hiding the window later fixes the problem.
					ShowWindow (wlbComponents, HIDE_WINDOW);

					// Make following items checked by default.
					ListView_SetCheckState (wlbComponents, 0, INSTALLATION::isPath);
					ListView_SetCheckState (wlbComponents, 1, INSTALLATION::isRegistry);
					ListView_SetCheckState (wlbComponents, 2, INSTALLATION::isBatch);
					ListView_SetCheckState (wlbComponents, 3, CONFIG::isForceC8Display);
				}
				

				WINDOWS::CONTROLS::CreateRichEdit (
					rePath, window, instance, 
					rePosition, reSize, 
					WS_CHILD | WS_TABSTOP, 
					WINDOW_IDS_PATH, REGISTRY::VALUE_DEFAULT_QUAIL_FOLDER_W
				);

				WINDOWS::CONTROLS::CreateRichEdit (
					wsLicense, window, instance, 
					wsPosition, wsSize,
					WS_CHILD | ES_MULTILINE | ES_AUTOVSCROLL | WS_VSCROLL | ES_READONLY,
					WINDOW_IDS_LICENSE, LICENSE
				);

				// Subclass the RichEdit control to catch the WM_VSCROLL messages
    			topLicenseControlLoop = (WNDPROC) GetWindowLongPtr (wsLicense, GWLP_WNDPROC);
    			SetWindowLongPtr (wsLicense, GWLP_WNDPROC, (LONG_PTR)LicenseControlLoop);
				
				WINDOWS::CONTROLS::CreateButton (
					wbLast, window, WINDOW_IDS_LAST, instance, 
					{ 244, 314 + 11 }, { 75, 23 }, 
					WS_CHILD, LOCAL::BUTTON_LAST
				);
				
				WINDOWS::CONTROLS::CreateButton (
					wbNext, window, WINDOW_IDS_NEXT, instance, 
					{ 323, 314 + 11 }, { 75, 23 }, 
					WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON, LOCAL::BUTTON_NEXT
				);
				
				WINDOWS::CONTROLS::CreateButton (
					wbCancel, window, WINDOW_IDS_CANCEL, instance, 
					{ 323 + 75 + 11, 314 + 11 }, { 75, 23 }, 
					WS_CHILD | WS_VISIBLE, LOCAL::BUTTON_CANCEL
				);

				{
					const pair<s16> position = { rePosition.x + reSize.x + 11, rePosition.y - 4 };
					const pair<s16> size = { 75, 23 };

					WINDOWS::CONTROLS::CreateButton (
						wbBrowse, window, WINDOW_IDS_BROWSE, instance, 
						position, size,
						WS_CHILD | BS_PUSHBUTTON, LOCAL::BUTTON_BROWSE
					);
				}
				
				{// Update Controls Fonts
					SendMessageW (window, 	WM_SETFONT, WPARAM (font), TRUE);
					SendMessageW (wbLast, 	WM_SETFONT, WPARAM (font), TRUE);
					SendMessageW (wbNext, 	WM_SETFONT, WPARAM (font), TRUE);
					SendMessageW (wbBrowse, WM_SETFONT, WPARAM (font), TRUE);
					SendMessageW (wbCancel, WM_SETFONT, WPARAM (font), TRUE);
					SendMessageW (rePath, 	WM_SETFONT, WPARAM (font), TRUE);

					SendMessageW (wsLicense, WM_SETFONT, WPARAM (fontMono), TRUE);
				}

			} break;

			case WM_PAINT: {

				HDC	windowContext;
				PAINTSTRUCT paint;

        		windowContext = BeginPaint (window, &paint);

				switch (currentPage) {
					case PAGE_TYPE_ENTRY: 			DrawEntry (windowContext); 										break;
					case PAGE_TYPE_LICENSE: 		DrawPage (windowContext); 	DrawLicense (windowContext); 		break;
					case PAGE_TYPE_DIRECTORY: 		DrawPage (windowContext); 	DrawDirectory (windowContext); 		break;
					case PAGE_TYPE_REGISTRY: 		DrawPage (windowContext); 	DrawRegistry (windowContext); 		break;
					case PAGE_TYPE_CONFIRMATION:	DrawPage (windowContext); 	DrawConfirmation (windowContext); 	break;
					case PAGE_TYPE_DOWNLOAD: 		DrawPage (windowContext); 	DrawDownload (windowContext); 		break;
					case PAGE_TYPE_EXIT: 			DrawExit (windowContext); 										break;
				}

				DrawFooter (windowContext);
				EndPaint (window, &paint);
        		
			} return TRUE;

			case WM_KEYDOWN: {
				switch (wParam) {

					case VK_RETURN: {
						const auto& current = KEYBOARD::pageActivablesList[KEYBOARD::active];

						if (current == WINDOW_IDS_NEXT) {
							if (!IsWindowEnabled (wbNext)) break;
						}

						HandleButtonPresses (window, current);
						HandlePageSwitch (window);
					} break;

					case VK_DOWN:
					case VK_LEFT: {
						const auto& pageActivablesStart = KEYBOARD::pageActivablesStarts[currentPage];
						const auto& pageActivablesEnd = KEYBOARD::pageActivablesEnds[currentPage];
						const auto last = KEYBOARD::pageActivablesList[KEYBOARD::active];

						if (KEYBOARD::active == pageActivablesStart) KEYBOARD::active = pageActivablesEnd;
						KEYBOARD::active--;

						const auto& current = KEYBOARD::pageActivablesList[KEYBOARD::active];
						KEYBOARD::HandleFocusChange (current, last);
					} break;

					case VK_UP:
					case VK_TAB:
					case VK_RIGHT: {
						const auto& pageActivablesStart = KEYBOARD::pageActivablesStarts[currentPage];
						const auto& pageActivablesEnd = KEYBOARD::pageActivablesEnds[currentPage];
						const auto last = KEYBOARD::pageActivablesList[KEYBOARD::active];

						KEYBOARD::active++;
						if (KEYBOARD::active == pageActivablesEnd) KEYBOARD::active = pageActivablesStart;

						const auto& current = KEYBOARD::pageActivablesList[KEYBOARD::active];
						KEYBOARD::HandleFocusChange (current, last);
					} break;

				}

				//return FALSE;
			} break;

			case WM_NOTIFY: {
        	    LPNMHDR pnmhdr = (LPNMHDR) lParam;
        	    if (pnmhdr->idFrom == WINDOW_IDS_COMPONENTS) { //IDC_LISTVIEW
        	        if (pnmhdr->code == LVN_ITEMCHANGED) {
			
        	            LPNMLISTVIEW pnmv = (LPNMLISTVIEW) lParam;
			
        	            if (pnmv->uNewState & LVIS_STATEIMAGEMASK) {

        	                u8 state = ((pnmv->uNewState & LVIS_STATEIMAGEMASK) >> 12) - 1;
							u8 item = pnmv->iItem;

							switch (item) {
								case 0: INSTALLATION::isRegistry = state; break;
								case 1: INSTALLATION::isPath = state; break;
								case 2: INSTALLATION::isBatch = state; break;
								case 3: CONFIG::isForceC8Display = state; break;
							}

        	            }
        	        }
        	    }
        	} break;

			case WM_COMMAND: {

				{ // Make the selected region marked as to redraw when focus change is being detected.
					switch (HIWORD (wParam)) {
					
						case EN_SETFOCUS: {
					
							if (LOWORD (wParam) == WINDOW_IDS_PATH) {
								InvalidateRect (window, &reRectPadding, true);
							}
					
						} break;
					
						case EN_KILLFOCUS: {
					
							if (LOWORD (wParam) == WINDOW_IDS_PATH) {
								InvalidateRect (window, &reRectPadding, true);
							}
					
						} break;

						case 0: {
							// Just in case if I misunderstood API. ...
							LOGINFO("Is 0?: %d, %lld\n", HIWORD (wParam), lParam);

							const u16& buttonId = LOWORD (wParam);
							HandleButtonPresses (window, buttonId);
							HandlePageSwitch (window);
						}
					
					}
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
			operatingSystem.dwOSVersionInfoSize = sizeof (OSVERSIONINFO);
			GetVersionEx (&operatingSystem);

			if (operatingSystem.dwMajorVersion <= 4) {
				ERROR ("Running on unsupported windows version!\n");
			}
		}

		const c16 windowName[] { L"PropSheetClass" };

		HICON icon;

		{ // ICONS
			icon = LoadIcon (instance, MAKEINTRESOURCE (IDI_ICON_MAIN));
			DEBUG (DEBUG_FLAG_LOGGING) if (icon == nullptr) { LOGERROR ("Could not load icon! %d\n", GetLastError ()); }
		}

		{ // IMAGES
			image = LoadBitmap (GetModuleHandle (NULL), MAKEINTRESOURCE (IDB_BITMAP1));
		}
		
		{ // FONTS
			//const c16 fontNameLicense[] = L"Cascadia Code";
			const c16 fontNameLicense[] = L"Consolas";
			const c16 fontNameSystem[] = L"Segoe UI";

			fontMono = CreateFontW (
				12, 0, 0, 0, FW_MEDIUM, FALSE, FALSE, FALSE, 
				ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, 
				DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, 
				fontNameLicense
			);

			fontBold = CreateFontW (
				16, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, 
				ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, 
				DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, 
				fontNameSystem
			);

			font = CreateFontW (
				14, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, 
				ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, 
				DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, 
				fontNameSystem
			);
		}

		WNDCLASSEXW windowClass 	{ 0 };
		windowClass.cbSize 			= sizeof (windowClass);
		windowClass.style			= CS_HREDRAW | CS_VREDRAW;
		windowClass.lpfnWndProc		= WindowLoop;
		windowClass.hInstance 		= instance;
		windowClass.hIcon 			= icon;
		windowClass.hCursor			= LoadCursor (NULL, IDC_ARROW);
		windowClass.hbrBackground	= (HBRUSH) GetStockObject (WHITE_BRUSH);
		windowClass.lpszClassName 	= windowName;

		if (!RegisterClassExW (&windowClass)) exit (1);

		{ // We're ensuring here that this value corresponds to client area.

			// Apparently WinAPI `CreateWindow` actually uses said size.x, size.y params as whole size including non-client area.
			// ... Why and how does I learn about it only now. 
			const DWORD style = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;
			RECT rect { 0, 0, size.x, size.y };
    		AdjustWindowRect (&rect, style, FALSE);

			// Get Centralized Window.
			RECT screenRect;
    		SystemParametersInfo(SPI_GETWORKAREA, 0, &screenRect, 0);
    		const s32 screenWidth = screenRect.right - screenRect.left;
    		const s32 screenHeight = screenRect.bottom - screenRect.top;
			const s32  xPos = (screenWidth - size.x) / 2 + screenRect.left;
    		const s32  yPos = (screenHeight - size.y) / 2 + screenRect.top;
		
			window = CreateWindowExW (
				0, windowName,
				LOCAL::WINDOW_TITLE,
				style,
				xPos,
				yPos,
				rect.right - rect.left, 
				rect.bottom - rect.top,
				nullptr,
				nullptr,
				instance,
				nullptr
			);

		}

		if (!window) exit (1);

		ShowWindow (window, SHOW_OPENNOACTIVATE);
		UpdateWindow (window);

		SetFocus (window);
	}

}
