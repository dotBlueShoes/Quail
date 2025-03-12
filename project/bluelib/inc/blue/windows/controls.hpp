// Created 2024.12.29 by Matthew Strumiłło (dotBlueShoes)
//  LICENSE: GNU GENERAL PUBLIC LICENSE Version 3, 29 June 2007
//
#pragma once
#include "types.hpp"

#include <RichEdit.h>
#include <CommCtrl.h>
#include <Shlobj.h>
#include <Commdlg.h>

namespace WINDOWS::CONTROLS {

	//const c8* MSFTEDIT_DLL_PATH { "riched20.dll" }; //
	const c8* MSFTEDIT_DLL_PATH { "Msftedit.dll" }; // a.k.a TextEdit 4.1
	const c16 BUTTON_CLASS[] { L"BUTTON" };

	void LoadRichEdit () { LoadLibraryA (MSFTEDIT_DLL_PATH); }

	void CreateProgressBarRange (
		HWND& progressBar, 
		const HWND& parentWindow, 
		const HINSTANCE& instance,
		const DWORD& style,
		const pair<u16>& position,
		const pair<u16>& size,
		const pair<u16>& range
	) {
		progressBar = CreateWindowExW (
			0, PROGRESS_CLASSW, 
			nullptr, 
			style, // PBS_MARQUEE, PBS_SMOOTH, PBS_SMOOTHREVERSE, PBS_VERTICAL
			position.x, position.y, size.x, size.y, 
			parentWindow, 
			(HMENU) 200, 
			instance, 
			nullptr
		);

		// Setting up the numeric range of progress bar.
		SendMessageW (progressBar, PBM_SETRANGE, 0, (LPARAM) MAKELONG (range.x, range.y));
	}

	void CreateProgressBarRangeStep (
		HWND& progressBar, 
		const HWND& parentWindow, 
		const HINSTANCE& instance,
		const DWORD& style,
		const pair<u16>& position,
		const pair<u16>& size,
		const pair<u16>& range,
		const u16 stepSize
	) {
		progressBar = CreateWindowExW (
			0, PROGRESS_CLASSW, 
			nullptr, 
			WS_CHILD | WS_VISIBLE, // PBS_MARQUEE, PBS_SMOOTH, PBS_SMOOTHREVERSE, PBS_VERTICAL
			position.x, position.y, size.x, size.y, 
			parentWindow, 
			(HMENU) 200, 
			instance, 
			nullptr
		);

		// Setting up the numeric range of progress bar.
		SendMessageW (progressBar, PBM_SETRANGE, 0, (LPARAM) MAKELONG (range.x, range.y));
		// Sets the step size.
		SendMessageW (progressBar, PBM_SETSTEP, (WPARAM) stepSize, 0); 
	}

	void CreateButton (
		HWND& button, 
		const HWND& parentWindow, 
		const HINSTANCE& instance, 
		const pair<s16>& position,
		const pair<s16>& size,
		const u32&  windowStyles,
		const c16* const& text = L"Click Me!"
	) {
		button = CreateWindowExW (
			0, BUTTON_CLASS,
			text,
			windowStyles,
			position.x, position.y,
			size.x, size.y,
			parentWindow,
			nullptr,
			instance,
			nullptr
		);
	}

	HRESULT BrowseFolder (
		const HWND& window, 
		const c16* const& startingFolderPath,
		c16*& folderPath, 
		u16 size
	) {
		
		IFileDialog* fileDialog = nullptr;
		IShellItem* shellItem = nullptr;
		HRESULT errorCode = S_OK;
		DWORD options;
		
		//IFileDialogEvents* dialogEvents = nullptr;

		// Initialize COM
		errorCode = CoInitialize (NULL); 

		if (FAILED (errorCode)) return errorCode;

		// Create the file dialog.
		errorCode = CoCreateInstance ( 
			CLSID_FileOpenDialog, 
			nullptr, CLSCTX_ALL, 
			IID_PPV_ARGS (&fileDialog)
		); 

		if (FAILED (errorCode)) {
			CoUninitialize ();
			return errorCode;
		}

		errorCode = (*fileDialog).GetOptions (&options);

		if (FAILED (errorCode)) {
			(*fileDialog).Release ();
			CoUninitialize ();
			return errorCode;
		}

		// Set the dialog options to allow folder selection.
		errorCode = (*fileDialog).SetOptions ( 
			options | FOS_PICKFOLDERS
		); 

		if (FAILED (errorCode)) {
			(*fileDialog).Release ();
			CoUninitialize ();
			return errorCode;
		}

		if (startingFolderPath != nullptr) {

			// Set the initial folder for item.
        	errorCode = SHCreateItemFromParsingName (
				startingFolderPath, nullptr, 
				IID_PPV_ARGS(&shellItem)
			);

        	if (FAILED (errorCode)) {
        	    (*fileDialog).Release ();
				CoUninitialize ();
				return errorCode;
        	}

			// Set the folder for dialog
        	(*fileDialog).SetFolder (shellItem);
        	(*shellItem).Release ();
		}

		// Show the dialog.
		errorCode = (*fileDialog).Show (window); 

		if (FAILED (errorCode)) {
			(*fileDialog).Release ();
			CoUninitialize ();
			return errorCode;
		}
		
		// Get the result.
		errorCode = (*fileDialog).GetResult (&shellItem); 

		if (FAILED (errorCode)) {
			(*fileDialog).Release ();
			CoUninitialize ();
			return errorCode;
		}

		// Get the folder path.
		errorCode = (*shellItem).GetDisplayName (SIGDN_FILESYSPATH, (LPWSTR*) &folderPath); 

		(*shellItem).Release ();
		(*fileDialog).Release ();

		CoUninitialize ();

		return errorCode;
	}


	void CreateRichEdit (
		HWND& richEdit,
		const HWND& parentWindow,
		const HINSTANCE& instance,
		const pair<s16>& position,
		const pair<s16>& size,
		const u32&  windowStyles,
		const u64& id = 0,
		const c16* text = L"Type here"
	) {
		richEdit = CreateWindowExW (
			NULL, MSFTEDIT_CLASS, 
			text,
			windowStyles,
			position.x, position.y, 
			size.x, size.y,
			parentWindow, 
			(HMENU)id,
			instance, 
			nullptr
		);

		//#define WS_EX_DLGMODALFRAME     0x00000001L
		//#define WS_EX_NOPARENTNOTIFY    0x00000004L
		//#define WS_EX_TOPMOST           0x00000008L
		//#define WS_EX_ACCEPTFILES       0x00000010L
		//#define WS_EX_TRANSPARENT       0x00000020L
		//#define WS_EX_MDICHILD          0x00000040L
		//#define WS_EX_TOOLWINDOW        0x00000080L
		//#define WS_EX_WINDOWEDGE        0x00000100L
		//#define WS_EX_CLIENTEDGE        0x00000200L
		//#define WS_EX_CONTEXTHELP       0x00000400L
		//#define WS_EX_RIGHT             0x00001000L
		//#define WS_EX_LEFT              0x00000000L
		//#define WS_EX_RTLREADING        0x00002000L
		//#define WS_EX_LTRREADING        0x00000000L
		//#define WS_EX_LEFTSCROLLBAR     0x00004000L
		//#define WS_EX_RIGHTSCROLLBAR    0x00000000L
		//#define WS_EX_CONTROLPARENT     0x00010000L
		//#define WS_EX_STATICEDGE        0x00020000L
		//#define WS_EX_APPWINDOW         0x00040000L
		//#define WS_EX_OVERLAPPEDWINDOW  (WS_EX_WINDOWEDGE | WS_EX_CLIENTEDGE)
		//#define WS_EX_PALETTEWINDOW     (WS_EX_WINDOWEDGE | WS_EX_TOOLWINDOW | WS_EX_TOPMOST)
		//#define WS_EX_LAYERED           0x00080000
		//#define WS_EX_NOINHERITLAYOUT   0x00100000L // Disable inheritance of mirroring by children
		//#define WS_EX_NOREDIRECTIONBITMAP 0x00200000L
		//#define WS_EX_LAYOUTRTL         0x00400000L // Right to left mirroring
		//#define WS_EX_COMPOSITED        0x02000000L
		//#define WS_EX_NOACTIVATE        0x08000000L

		//richEdit = CreateWindowExW (
		//	0,
		//	MSFTEDIT_CLASS,
		//	L"",
		//	windowStyles,
		//	0, 0, 332, 21,
		//	parentWindow,
		//	NULL,
		//	instance,
		//	NULL
		//);

		#ifdef DEBUG_FLAG_LOGGING
			if (richEdit == nullptr) {
				ERROR ("Could not create `RichEdit` Control.\n");
			}
		#endif
	}

	bool IsVerticalScrollbarAtMax (HWND window, s16 yOffset) {

    	SCROLLINFO scroll;
    	scroll.cbSize = sizeof (scroll);
    	scroll.fMask = SIF_ALL;  // Get all scroll information.

    	if (GetScrollInfo (window, SB_VERT, &scroll)) {
			//LOGINFO ("Pos: %d, %d\n", scroll.nPos, scroll.nMax - yOffset);
    	    // Check if the current position is at the maximum value.
    	    if (scroll.nPos == (scroll.nMax - yOffset)) return true;
    	}
		// 32230, 32364 = 134
		// 32218, 32352 = 134 (dif 12)

    	return false; 
	}

}
