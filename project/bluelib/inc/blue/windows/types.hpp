// Created 2024.12.29 by Matthew Strumiłło (dotBlueShoes)
//  LICENSE: GNU GENERAL PUBLIC LICENSE Version 3, 29 June 2007
//
#pragma once
#include "../types.hpp"

#include <Windowsx.h>
#include <Windows.h>

#ifdef ERROR
	#undef ERROR
#endif

#define GetProcMemory(procedureType, module, procedureName) { \
	procedureName = (procedureType) GetProcAddress (GetModuleHandleA (#module), #procedureName); \
	if (procedureName == nullptr) ERROR ("Function not created. Failed to load `%s` method: `%s`\n", #module, #procedureName); \
}

bool IsValidDirectory (
	IN 		const c16* const& 	path, 
	IN 		const u32& 			pathLength
) {

	// Is a valid pointer.
	if (path == nullptr && path[0] != L'\0') return false;

	// Minimum: L"C:/" -> 3 characters * 2. Maximum: MAX_PATH which is 260 by default
	if (pathLength < 6 || pathLength > MAX_PATH) return false;

	// Follows the specified format: "n:/..."
	if (path[1] != L':' && path[1] != L'\\') return false;

    // Ensure the path contains no invalid characters.
	const c16 invchars[] { L'<', L'>', L'\"', L'|', L'?', L'*', L'\0' };
	if (wcspbrk (path, invchars) != nullptr) return false;

	// Again ensure the path contains no invalid characters in this case ":" sign.
	for (u32 i = 3; i < pathLength; ++i) {
		if (path[i] == L':') return false;
	}

    // Try creating the directory to check for access permissions.
    if (CreateDirectoryW (path, NULL)) {

        RemoveDirectoryW (path); // Remove Test directory.
        return true;  

    } else {

		if (GetLastError () == ERROR_ALREADY_EXISTS) {
			return true; // Directory already exist.
		}

        return false;  // Cannot create the directory due to permission issues.
    }
	
}
