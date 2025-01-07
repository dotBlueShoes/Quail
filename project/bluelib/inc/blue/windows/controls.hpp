// Created 2024.12.29 by Matthew Strumiłło (dotBlueShoes)
//
#pragma once
#include "types.hpp"
#include "../types.hpp"

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

	BOOL BrowseForFolder (const HWND& hwndOwner, c16* const& pszFolderPath, DWORD dwSize) {

		BROWSEINFOW bi = { 0 };
		bi.lpszTitle = L"Select a Folder";
		bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE;
		bi.hwndOwner = hwndOwner;

		// Open the folder browser dialog
		PIDLIST_ABSOLUTE pidl = SHBrowseForFolderW (&bi);

		if (pidl != NULL) {
		
		    // Get the path of the selected folder
		    if (SHGetPathFromIDListW (pidl, pszFolderPath)) {
		        return TRUE;  // Success
		    }
		}

		return FALSE;  // Failed to select a folder
	}

	HRESULT BrowseFolder (const HWND& hwndOwner, c16* pszFolderPath, DWORD dwSize) {
		
	    HRESULT hr = S_OK;
	    IFileDialog *pfd = NULL;

	    // Initialize COM
	    hr = CoInitialize(NULL);
	    if (FAILED(hr)) {
	        return hr;
	    }

	    // Create the file dialog
	    hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL, IID_PPV_ARGS(&pfd));
	    if (FAILED(hr)) {
	        CoUninitialize();
	        return hr;
	    }

	    // Set the dialog options to allow folder selection
	    IFileDialogEvents *pfdEvents = NULL;
	    DWORD dwOptions;

	    hr = pfd->GetOptions(&dwOptions);
	    if (FAILED(hr)) {
	        pfd->Release();
	        CoUninitialize();
	        return hr;
	    }

	    hr = pfd->SetOptions(dwOptions | FOS_PICKFOLDERS);
	    if (FAILED(hr)) {
	        pfd->Release();
	        CoUninitialize();
	        return hr;
	    }

	    // Show the dialog
	    hr = pfd->Show(hwndOwner);
	    if (FAILED(hr)) {
	        pfd->Release();
	        CoUninitialize();
	        return hr;
	    }

	    // Get the result (selected folder)
	    IShellItem *psi = NULL;
	    hr = pfd->GetResult(&psi);
	    if (FAILED(hr)) {
	        pfd->Release();
	        CoUninitialize();
	        return hr;
	    }

	    // Get the folder path as a string
	    hr = psi->GetDisplayName (SIGDN_FILESYSPATH, (LPWSTR*) &pszFolderPath);
	    psi->Release();

	    pfd->Release();
	    CoUninitialize();
	    return hr;
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
		//#define WS_EX_NOINHERITLAYOUT   0x00100000L // Disable inheritence of mirroring by children
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

}