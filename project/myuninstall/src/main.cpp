// Created 2024.01.14 by Matthew Strumiłło (dotBlueShoes)
//  LICENSE: GNU GENERAL PUBLIC LICENSE Version 3, 29 June 2007
//
#include <global/logger/win.hpp>
//
#include <blue/windows/console.hpp>
//
#include <global/windows/registry.hpp>
//
#include <shobjidl.h>
#include <shlwapi.h>
#include <atlbase.h>


#define DIRECTORY_REMOVAL_DELAY L"3"

const c16 MSG_TITLE		[] = L"Quail Uninstaller";
const c16 MSG_START		[] = L"Are you sure u want to proceed the uninstallation Quail?";
const c16 MSG_SUCCESS	[] = L"Uninstallation was Successful";
const c16 MSG_FAILURE	[] = L"Uninstallation was Interrupted";
const c16 MSG_NOTHING	[] = L"Quail components were NOT removed due to the intentional user selection";
//
const c16 MSG_R_REG		[] = L"Remove registries?";
const c16 MSG_R_VPE		[] = L"Remove entry in path environment variable?";
const c16 MSG_R_DIR		[] = L"Remove directories?";


void RemoveRegistryUninstaller () {

	//  ABOUT
	// To delete a non-empty registry key directly, we'll have to 
	//  delete all of its subkeys and values first.
	//

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

	//  ABOUT
	// To delete a non-empty registry key directly, we'll have to 
	//  delete all of its subkeys and values first.
	//

	RegDeleteKeyW (HKEY_LOCAL_MACHINE, WINDOWS::REGISTRY::PROPERTY_QUAIL_CONFIGS_FILEPATH_W);
	RegDeleteKeyW (HKEY_LOCAL_MACHINE, WINDOWS::REGISTRY::PROPERTY_QUAIL_IS_FORCE_C8_DISPLAY);
	RegDeleteKeyW (HKEY_LOCAL_MACHINE, WINDOWS::REGISTRY::PROPERTY_QUAIL_LISTING_LINE_SIZE);
	
	RegDeleteKeyW (HKEY_LOCAL_MACHINE, WINDOWS::REGISTRY::KEY_PATH_W);
}


void RemoveQuailFromPath () {

	LSTATUS errorCode;
	HKEY key;

	//DWORD envSize = 2048 * sizeof (c16); // up to win 7 limit.
	//DWORD envSize = 32767 * sizeof (c16); // now the limit is.
	DWORD pathEnvVarSize = 4096 * sizeof (c16); // for now.

	c16* pathEnvVar; ALLOCATE (c16, pathEnvVar, pathEnvVarSize);

	MEMORY::EXIT::PUSH (pathEnvVar, FREE);

	{ // Get 
		errorCode = RegOpenKeyExW (HKEY_LOCAL_MACHINE, WINDOWS::REGISTRY::KEY_ENVIRONMENT_VARIABLES_W, 0, KEY_ALL_ACCESS, &key);
		if (errorCode != ERROR_SUCCESS) { ERROR ("Could not open key at `%ls`\n", WINDOWS::REGISTRY::KEY_ENVIRONMENT_VARIABLES_W); }

		errorCode = RegGetValueW (key, NULL, WINDOWS::REGISTRY::PROPERTY_PATH_W, RRF_RT_ANY, NULL, pathEnvVar, &pathEnvVarSize);
		if (errorCode != ERROR_SUCCESS) { ERROR ("Could not get `%ls` value.\n", WINDOWS::REGISTRY::PROPERTY_PATH_W); } 
	}

	c16* occurrence = wcsstr (pathEnvVar, CONFIG::topConfigsFolder);

	if (occurrence == nullptr) { LOGWARN ("Quail directory was not found in the 'path' variable.\n") FREE (pathEnvVar); return; }


	const auto length = CONFIG::topConfigsFolderLength / sizeof (c16);
	const auto&& end = occurrence + length;

	LOGINFO ("HERE: [%d], %ls\n", CONFIG::topConfigsFolderLength, CONFIG::topConfigsFolder);
	LOGINFO ("Before: %ls\n\n", pathEnvVar);

	u32 index = 0;

	for (; end[index] != L'\0'; ++index) {
		occurrence[index] = end[index];
	} occurrence[index] = L'\0'; // null-terminate string.

	LOGINFO ("After: %ls\n\n", pathEnvVar);

	const u32 newLength = pathEnvVarSize - CONFIG::topConfigsFolderLength;
	LOGINFO ("HERE: [%d], [%d]\n", pathEnvVarSize, newLength);

	{ // SET
		errorCode = RegSetValueExW (key, WINDOWS::REGISTRY::PROPERTY_PATH_W, 0, REG_SZ, (LPBYTE)pathEnvVar, newLength);	
		if (errorCode != ERROR_SUCCESS) { ERROR ("Could not set `%ls` value with `%ls`\n", WINDOWS::REGISTRY::PROPERTY_PATH_W, pathEnvVar); }
			
		// Update all other applications because we did just edited an environment variable!
		SendMessageTimeoutW (HWND_BROADCAST, WM_SETTINGCHANGE, 0, (LPARAM)"Environment", SMTO_BLOCK, 100, NULL);
	}

	FREE (pathEnvVar); MEMORY::EXIT::POP ();

}


void RemoveDirectory () {

	LOGINFO ("%ls\n", CONFIG::topConfigsFolder);

	STARTUPINFOW startup { sizeof (startup) };
    PROCESS_INFORMATION process;

	// Create a command to delete the uninstaller after it finishes
    // We will use cmd.exe to run the delete command after the uninstaller finishes.
	const c16 commandFormat [] = L"cmd.exe /C timeout /T " DIRECTORY_REMOVAL_DELAY L" /NOBREAK && rd /S /Q \"%s\"";
    c16 command[MAX_PATH + sizeof (commandFormat)];

    swprintf_s (
		command, 
		sizeof (command) / sizeof (c16),
        commandFormat, 
		CONFIG::topConfigsFolder
	);
    	
	//  ABOUT
	// We're staring a new process to delete the uninstaller. However
	//  mind that it is going to execute immediately - therefore the delay.
	//

    if (CreateProcessW (NULL, command, NULL, NULL, FALSE, 0, NULL, NULL, &startup, &process)) {
        CloseHandle (process.hProcess);
        CloseHandle (process.hThread);
    } else {
		ERROR ("Failed to create process for deleting uninstaller. Error code: %lu\n", GetLastError());
    }

    LOGINFO ("Uninstallation complete. Uninstaller will be deleted shortly.\n");

}


void Setup () {

	HKEY key = nullptr;

	LSTATUS error = RegOpenKeyExW (
		HKEY_LOCAL_MACHINE,
		WINDOWS::REGISTRY::KEY_PATH_W,
		0,
		KEY_ALL_ACCESS,
		&key
	);

	// Check if valid unistallation.
	if (error != ERROR_SUCCESS || key == nullptr) {
		ERRORWIN ("Could not get quail filepath from registry. Quail might be already uninstalled.");
	} else {
		RegCloseKey (key);
	}

	// Then get quail filepath.
	WINDOWS::REGISTRY::PROPERTY::GET::TopConfigsFolder (
		CONFIG::topConfigsFolderLength, 
		CONFIG::topConfigsFolder
	);
}


void UninstallALL() {

	s32 collectiveResponse = 0;
	s32 response;

	Setup ();

	{ // Properly deallocate data if we hit ERROR.
		MEMORY::EXIT::PUSH (CONFIG::topConfigsFolder, FREE);
	}

	response = MessageBoxW (
		nullptr, MSG_R_REG, MSG_TITLE, MB_YESNOCANCEL | MB_ICONQUESTION
	);

	if (response == IDYES) { 
		++collectiveResponse;
		// Remove UNINSTALLER Registry Keys
		LOGINFO ("1. Removing uninstaller registries\n");
		RemoveRegistryUninstaller ();
		// Remove Quail Registry Keys
		LOGINFO ("2. Removing quail registries\n");
		RemoveRegistryQuail ();
	} else if (response == IDCANCEL) goto uni_error;

	response = MessageBoxW (
		nullptr, MSG_R_VPE, MSG_TITLE, MB_YESNOCANCEL | MB_ICONQUESTION
	);

	if (response == IDYES) { 
		++collectiveResponse;
		// Remove Quail From Path
		LOGINFO ("3. Removing path entry\n");
		RemoveQuailFromPath ();
	} else if (response == IDCANCEL) goto uni_error;

	response = MessageBoxW (
		nullptr, MSG_R_DIR, MSG_TITLE, MB_YESNOCANCEL | MB_ICONQUESTION
	);

	if (response == IDYES) { 
		++collectiveResponse;
		LOGINFO ("4. Removing itself+directory\n");
		RemoveDirectory ();
	} else if (response == IDCANCEL) goto uni_error;

	FREE (CONFIG::topConfigsFolder); MEMORY::EXIT::POP ();
	
	switch (collectiveResponse) {
		case 0: {
			MessageBoxW (nullptr, MSG_NOTHING, MSG_TITLE, MB_OK);
			return;
		} 

		default: {

			//  ABOUT
			// Due to the delay on the execution of the directory removal
			//  it is not possible to wait for user input at that state.
			// Application must end execution quickly...
			//
			//MessageBoxW (nullptr, MSG_SUCCESS, MSG_TITLE, MB_OK);
			
			return;
		} 
	}

	uni_error:	
		FREE (CONFIG::topConfigsFolder); MEMORY::EXIT::POP ();
	 	MessageBoxW (nullptr, MSG_FAILURE, MSG_TITLE, MB_OK);
		return;

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

	{ // Init Logging
		TIMESTAMP_BEGIN = TIMESTAMP::GetCurrent ();
		DEBUG (DEBUG_FLAG_LOGGING) {
			WINDOWS::AttachConsole ();
			putc ('\n', stdout); // Align fututre debug-logs
		}
		LOGINFO ("Application Statred!\n");
	}

	s32 response;

	response = MessageBoxW (
		nullptr, MSG_START, MSG_TITLE, MB_YESNO | MB_ICONQUESTION
	);

	switch (response) {

		case IDYES: {
			UninstallALL ();
		} break;

		case IDNO: {
			MessageBoxW (nullptr, MSG_FAILURE, MSG_TITLE, MB_OK);
		} break;

		default: break;
	}

	{ // Deinit Logging
		LOGMEMORY ();
		LOGINFO ("Finalized Execution\n");
		DEBUG (DEBUG_FLAG_LOGGING) {
			putc ('\n', stdout); // Align debug-logs
			Sleep (2000);
		}
	}

	return 0;
}
