// Created 2024.01.14 by Matthew Strumiłło (dotBlueShoes)
//  LICENSE: GNU GENERAL PUBLIC LICENSE Version 3, 29 June 2007
//
#include <blue/windows/console.hpp>
#include <blue/types.hpp>
#include <blue/log.hpp>

#include <global/windows/registry.hpp>

//#include <windows.h>

//#include <shlwapi.h>
//#include <iostream>

#include <shobjidl.h>
#include <shlwapi.h>
#include <atlbase.h>


const c16 MSG_TITLE		[] = L"Quail Uninstaller";
const c16 MSG_START		[] = L"Are you sure u want to proceed the uninstallation of all Quail components";
const c16 MSG_SUCCESS	[] = L"Uninstallation was Succesfull";
const c16 MSG_FAILURE	[] = L"Uninstallation was Interrupted";

bool isRemoveDirectory = false;
bool isRemoveRegistry = false;
bool isRemovePath = true;


void RemoveRegistryUninstaller () {

	// To delete a non-empty registry key directly, we'll have to 
	//  delete all of its subkeys and values first.

	RegDeleteKeyW (HKEY_LOCAL_MACHINE, WINDOWS::REGISTRY::PROPERTY_UNINSTALL_DISPLAY_ICON);
	RegDeleteKeyW (HKEY_LOCAL_MACHINE, WINDOWS::REGISTRY::PROPERTY_UNINSTALL_DISPLAY_NAME);
	RegDeleteKeyW (HKEY_LOCAL_MACHINE, WINDOWS::REGISTRY::PROPERTY_UNINSTALL_DISPLAY_VERSION);
	RegDeleteKeyW (HKEY_LOCAL_MACHINE, WINDOWS::REGISTRY::PROPERTY_UNINSTALL_ESTIMATED_SIZE);
	RegDeleteKeyW (HKEY_LOCAL_MACHINE, WINDOWS::REGISTRY::PROPERTY_UNINSTALL_NO_REPAIR);
	RegDeleteKeyW (HKEY_LOCAL_MACHINE, WINDOWS::REGISTRY::PROPERTY_UNINSTALL_NO_MODIFY);
	RegDeleteKeyW (HKEY_LOCAL_MACHINE, WINDOWS::REGISTRY::PROPERTY_UNINSTALL_HELP_LINK);
	RegDeleteKeyW (HKEY_LOCAL_MACHINE, WINDOWS::REGISTRY::PROPERTY_UNINSTALL_INSTALL_LOCATION);
	RegDeleteKeyW (HKEY_LOCAL_MACHINE, WINDOWS::REGISTRY::PROPERTY_UNINSTALL_PUBLISHER);
	RegDeleteKeyW (HKEY_LOCAL_MACHINE, WINDOWS::REGISTRY::PROPERTY_UNINSTALL_UNINSTALL_STRING);
	RegDeleteKeyW (HKEY_LOCAL_MACHINE, WINDOWS::REGISTRY::PROPERTY_UNINSTALL_URL_INFO_ABOUT);
	RegDeleteKeyW (HKEY_LOCAL_MACHINE, WINDOWS::REGISTRY::PROPERTY_UNINSTALL_URL_UPDATE_INFO);

    RegDeleteKeyW (HKEY_LOCAL_MACHINE, WINDOWS::REGISTRY::KEY_PATH_UNINSTALL_W);
}


void RemoveRegistryQuail () {

	// To delete a non-empty registry key directly, we'll have to 
	//  delete all of its subkeys and values first.

	RegDeleteKeyW (HKEY_LOCAL_MACHINE, WINDOWS::REGISTRY::PROPERTY_QUAIL_FILEPATH_W);
	RegDeleteKeyW (HKEY_LOCAL_MACHINE, WINDOWS::REGISTRY::KEY_PATH_W);
}


void RemoveQuailFromPath () {

	LSTATUS errorCode;
	HKEY key;

	c16* pathEnvVar; ALLOCATE (c16, pathEnvVar, 2048);
	DWORD pathEnvVarSize = -1;

	{ // Get 
		errorCode = RegOpenKeyExW (HKEY_LOCAL_MACHINE, WINDOWS::REGISTRY::KEY_ENVIROMENT_VARIABLES_W, 0, KEY_ALL_ACCESS, &key);
		if (errorCode != ERROR_SUCCESS) { ERROR ("Could not open key at `%ls`\n", WINDOWS::REGISTRY::KEY_ENVIROMENT_VARIABLES_W); }

		errorCode = RegGetValueW (key, NULL, WINDOWS::REGISTRY::PROPERTY_PATH_W, RRF_RT_ANY, NULL, pathEnvVar, &pathEnvVarSize);
		if (errorCode != ERROR_SUCCESS) { ERROR ("Could not get `%ls` value.\n", WINDOWS::REGISTRY::PROPERTY_PATH_W); } 
	}

	c16* occurrence = wcsstr (pathEnvVar, CONFIG::topConfigsFolder);

	if (occurrence == nullptr) { LOGWARN ("Quail directory was not found in the 'path' varaible.\n") FREE (pathEnvVar); return; }


	const auto length = CONFIG::topConfigsFolderLength / sizeof (c16);
	const auto&& end = occurrence + length;

	LOGINFO ("Before: %ls\n\n", pathEnvVar);

	u32 index = 0;

	for (; end[index] != L'\0'; ++index) {
		occurrence[index] = end[index];
	} occurrence[index] = L'\0'; // null-terminate string.

	LOGINFO ("After: %ls\n\n", pathEnvVar);

	const u32 newLength = pathEnvVarSize - length;

	{ // SET
		errorCode = RegSetValueExW (key, WINDOWS::REGISTRY::PROPERTY_PATH_W, 0, REG_SZ, (LPBYTE)pathEnvVar, newLength);	
		if (errorCode != ERROR_SUCCESS) { ERROR ("Could not set `%ls` value with `%ls`\n", WINDOWS::REGISTRY::PROPERTY_PATH_W, pathEnvVar); }
			
		// Update all other applications because we did just edited an enviroment varaible!
		SendMessageTimeoutW (HWND_BROADCAST, WM_SETTINGCHANGE, 0, (LPARAM)"Environment", SMTO_BLOCK, 100, NULL);
	}

	FREE (pathEnvVar);

}


void RemoveItself () {
	c16* buffer; ALLOCATE (c16, buffer, CONFIG::topConfigsFolderLength + CONFIG::UNINSTALLER_NAME_LENGTH);

	{ // CONSTRUCT
		memcpy (buffer, CONFIG::topConfigsFolder, CONFIG::topConfigsFolderLength);
		wmemset (buffer + (CONFIG::topConfigsFolderLength / 2) - 1, L'\\', 1);
		memcpy (buffer + (CONFIG::topConfigsFolderLength / 2), CONFIG::UNINSTALLER_NAME, CONFIG::UNINSTALLER_NAME_LENGTH);
		//LOGINFO ("uninstaller: %ls\n", buffer);
	}

	BOOL errorCode;

	errorCode = DeleteFileW (buffer);
	if (errorCode != TRUE) LOGWARN ("Cound not remove uninstaller file!\n");

	FREE (buffer);
}


void RemoveDirectory () {

	IFileOperation* fileOperation;
	HRESULT errorCode; 

	IShellItem* fileOrFolderItem = nullptr;
	BOOL isAborted = FALSE; // not needed?

	errorCode = CoInitializeEx (NULL, COINIT_MULTITHREADED);
    if (FAILED (errorCode)) { CoUninitialize (); ERROR ("Couldn't initialize COM library."); }

    errorCode = CoCreateInstance (CLSID_FileOperation, NULL, CLSCTX_ALL, IID_PPV_ARGS (&fileOperation));
    if (FAILED (errorCode)) { CoUninitialize (); ERROR ("Couldn't CoCreateInstance."); }

    errorCode = fileOperation->SetOperationFlags (FOF_NOCONFIRMATION | FOF_SILENT | FOF_NOERRORUI);
    if (FAILED (errorCode)) { fileOperation->Release (); CoUninitialize (); ERROR ("Couldn't add flags."); }

    errorCode = SHCreateItemFromParsingName (CONFIG::topConfigsFolder, NULL, IID_PPV_ARGS (&fileOrFolderItem));
    if (FAILED (errorCode)) { fileOrFolderItem->Release (); fileOperation->Release (); CoUninitialize (); ERROR ("Couldn't get file into an item."); }
    	
	errorCode = fileOperation->DeleteItem (fileOrFolderItem, NULL); // 2 param is track of progress.
    fileOrFolderItem->Release ();
    if (FAILED (errorCode)) { fileOperation->Release (); CoUninitialize (); ERROR ("Failed to mark file/folder item for deletion."); }

    errorCode = fileOperation->PerformOperations ();
	if (FAILED (errorCode)) { fileOperation->Release (); CoUninitialize (); ERROR ("Failed to carry out delete."); }

	errorCode = fileOperation->GetAnyOperationsAborted (&isAborted);

	fileOperation->Release ();
	CoUninitialize ();

	if (FAILED (errorCode)) { ERROR ("Could not GetAnyOperationsAborted()."); }
    if (isAborted) { LOGWARN ("Some operations were aborted.\n"); }
}


void UninstallALL() {

	// Get quail filepath.
	WINDOWS::REGISTRY::ReadPropertyTopConfigsFolder ();
	
	{ // Remove UNINSTALLER Registry Keys
		RemoveRegistryUninstaller ();
	}

	if (isRemoveRegistry) { // Remove Quail Registry Keys
		RemoveRegistryQuail ();
	}

	if (isRemovePath) { // Remove Quail From Path
		RemoveQuailFromPath ();
	}

	if (isRemoveDirectory) { // Remove itself.
		RemoveItself ();
    }

	if (isRemoveDirectory) { // Remove Quail directory.
		RemoveDirectory ();
	}

	FREE (CONFIG::topConfigsFolder);

}


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
	DEBUG (DEBUG_FLAG_LOGGING) putc ('\n', stdout); // Align fututre debug-logs
	LOGINFO ("Application Statred!\n");

	s32 response = MessageBoxW (
		nullptr, MSG_START, MSG_TITLE, MB_YESNO | MB_ICONQUESTION
	);

	switch (response) {

		case IDYES: {
			UninstallALL ();
			MessageBoxW (nullptr, MSG_SUCCESS, MSG_TITLE, MB_OK);
		} break;

		case IDNO: {
			MessageBoxW (nullptr, MSG_FAILURE, MSG_TITLE, MB_OK);
		} break;

		default: break;
	}

	LOGINFO ("Finalized Execution\n");
	LOGMEMORY ();

	DEBUG (DEBUG_FLAG_LOGGING) Sleep (2000);
	DEBUG (DEBUG_FLAG_LOGGING) putc ('\n', stdout); // Align debug-logs

	return 0;
}