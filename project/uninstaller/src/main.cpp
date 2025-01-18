// Created 2024.01.14 by Matthew Strumiłło (dotBlueShoes)
//  LICENSE: GNU GENERAL PUBLIC LICENSE Version 3, 29 June 2007
//
#include <blue/windows/console.hpp>
#include <blue/types.hpp>
#include <blue/log.hpp>

#include <global/windows/registry.hpp>


const c16 MSG_TITLE		[] = L"Quail Uninstaller";
const c16 MSG_START		[] = L"Are you sure u want to proceed the uninstallation of all Quail components";
const c16 MSG_SUCCESS	[] = L"Uninstallation was Succesfull";
const c16 MSG_FAILURE	[] = L"Uninstallation was Interrupted";


void UninstallALL() {
	
	{ // Remove Registry Keys

		// To delete a non-empty registry key directly, we'll have to 
		//  recursively delete all of its subkeys and values first.

		//HKEY key;
		//LSTATUS errorCode;
//
		//errorCode = RegOpenKeyExW (
		//	HKEY_LOCAL_MACHINE, 
		//	WINDOWS::REGISTRY::KEY_PATH_UNINSTALL_W, 
		//	0, 
		//	KEY_READ | KEY_WRITE, 
		//	&key
		//);
//
		//if (errorCode != ERROR_SUCCESS) MSGINFO ("no key"); return;
//
		//DWORD dwIndex = 0;
        //WCHAR szSubKeyName[255];
        //DWORD dwSubKeyNameSize = sizeof (szSubKeyName) / sizeof (szSubKeyName[0]);
		

		//WINDOWS::REGISTRY::PROPERTY_UNINSTALL_DISPLAY_ICON
		//WINDOWS::REGISTRY::PROPERTY_UNINSTALL_DISPLAY_NAME
		//WINDOWS::REGISTRY::PROPERTY_UNINSTALL_DISPLAY_VERSION
		//WINDOWS::REGISTRY::PROPERTY_UNINSTALL_ESTIMATED_SIZE
		//WINDOWS::REGISTRY::PROPERTY_UNINSTALL_NO_REPAIR
		//WINDOWS::REGISTRY::PROPERTY_UNINSTALL_NO_MODIFY
		//WINDOWS::REGISTRY::PROPERTY_UNINSTALL_HELP_LINK
		//WINDOWS::REGISTRY::PROPERTY_UNINSTALL_INSTALL_LOCATION
		//WINDOWS::REGISTRY::PROPERTY_UNINSTALL_PUBLISHER
		//WINDOWS::REGISTRY::PROPERTY_UNINSTALL_UNINSTALL_STRING
		//WINDOWS::REGISTRY::PROPERTY_UNINSTALL_URL_INFO_ABOUT
		//WINDOWS::REGISTRY::PROPERTY_UNINSTALL_URL_UPDATE_INFO

		// Close the handle to the subkey
        //RegCloseKey (hSubKey);

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

	{ // Remove Quail From Path

	}

	{ // Remove itself.
		//BOOL errorCode;
		//errorCode = DeleteFileW ();
		//if (!DeleteFile (uninstallerPath.c_str())) {
        //std::wcerr << L"Failed to delete the uninstaller.\n";
    }

	{ // Remove Quail directory

	}

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