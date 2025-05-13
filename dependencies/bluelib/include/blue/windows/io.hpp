// Created 2025.04.02 by Matthew Strumiłło (dotBlueShoes)
//  LICENSE: GNU GENERAL PUBLIC LICENSE Version 3, 29 June 2007
//
#pragma once
#include "types.hpp"

namespace IO {

	//  ISSUE, TODO
	// This method should do the checking again and remove the directories created if checking failed...
	//
	bool CreateAllPathDirectories (
		IN 		c16*& 				path, 
		IN 		const u32& 			pathLength
	) {

		for (u32 i = 2; i < pathLength; ++i) {

			if (path[i] == '\\') {
				path[i] = '\0';

				if (CreateDirectoryW (path, NULL) == false) {
					// Omit `ALREADY_EXISTS` errors. Any other mean permission issues.
					if (GetLastError () != ERROR_ALREADY_EXISTS) { return false; } 
				}

				path[i] = '\\';
			}

		}

		// Create last directory in path.
		if (CreateDirectoryW (path, NULL) == false) {
			// Omit `ALREADY_EXISTS` errors. Any other mean permission issues.
			if (GetLastError () != ERROR_ALREADY_EXISTS) { return false; } 
		}

		return true;

	}

	//bool CreatePathIntermediateDirectories (
	//	IN 		c16*& 				path, 
	//	IN 		const u32& 			pathLength,
	//	INOUT 	u32& 				lastExistingDirectoryPosition,
	//	INOUT 	u32& 				lastDirectoryPosition
	//) {
	//	
	//	for (u32 i = 2; i < pathLength; ++i) {
	//
	//		if (path[i] == '\\') {
	//
	//			{ // Track position to ensure there is no double '\\' in a path.
	//				if (i == (lastDirectoryPosition + 1)) {
	//					// Error. Path has double '\\' like "C:\\" or "C:\name\\\name"
	//					return false;
	//				}
	//
	//				lastDirectoryPosition = i;
	//			}
	//
	//			// Temporary change it to eos sign.
	//			path[i] = '\0';
	//
	//			// Try creating the directory to check for access permissions.
	//			if (CreateDirectoryW (path, NULL) == false) {
	//				if (GetLastError () == ERROR_ALREADY_EXISTS) { 
	//					lastExistingDirectoryPosition = i;
	//				} else { return false; } // Omit `ALREADY_EXISTS` errors. Any other mean permission issues.
	//			}
	//
	//			path[i] = '\\';
	//		}
	//	
	//	}
	//
	//	return true;
	//}


	bool IsValidDirectory (
		IN 		c16*& 				path, 
		IN 		const u32& 			pathLength
	) {
		const u32 pathLengthW = pathLength / 2;
		const u32 DRIVE_END_POS = 2;
	
		// Is a valid pointer.
		if (path == nullptr && path[0] != L'\0') return false;
	
		// Minimum: L"C:/" -> 3 characters. Maximum: MAX_PATH which is 260 by default
		if (pathLengthW < 3 || pathLengthW > MAX_PATH) return false;
	
		// Follows the specified format: "n:/..."
		if (path[1] != L':' || path[DRIVE_END_POS] != L'\\') return false;
	
		// If it's not just the top drive directory (hence 4 as in "C:\[nt]") do the following checks.
		if (pathLengthW > 4) {

			// Ensure the path contains no invalid characters.
			const c16 invchars[] { L':', L'<', L'>', L'\"', L'|', L'?', L'*', L'\0' };
			if (wcspbrk (path + 3, invchars) != nullptr) return false;

			{ 

				//  NOTE
				// String is one character longer then it is supposed.
				// Thus all the -1 or -2.
				//

				// Replace all '/' signs with '\' signs so it works with the algorithm.
				for (u32 i = DRIVE_END_POS; i < pathLengthW - 1; ++i) {
					//if (path[i] == '/') path[i] = '\\';
					path[i] = (path[i] == '/') ? '\\' : path[i];
				}

				// Remove unnecessary sign if there is one.
				if (path[pathLengthW - 2] == '\\') path[pathLengthW - 2] = '\0';
				
				u32 lastExistingDirectoryPosition = DRIVE_END_POS;
				u32 lastDirectoryPosition = DRIVE_END_POS;

				{ // Create temporary directories in path to check for access permissions.
					for (u32 i = DRIVE_END_POS; i < pathLengthW; ++i) {

						if (path[i] == '\\') {

							{ // Track position to ensure there is no double '\\' in a path.
								if (i == (lastDirectoryPosition + 1)) {
									// Error. Path has double '\\' like "C:\\" or "C:\name\\\name"
									return false;
								}

								lastDirectoryPosition = i;
							}

							// Temporary change it to eos sign.
							path[i] = '\0';

							if (CreateDirectoryW (path, NULL) == false) {
								if (GetLastError () == ERROR_ALREADY_EXISTS) { 
									lastExistingDirectoryPosition = i;
								} else { return false; } // Omit `ALREADY_EXISTS` errors. Any other mean permission issues.
							}

							path[i] = '\\';
						}
					
					}

					// Try creating the last directory in path to check for access permissions.
					if (CreateDirectoryW (path, NULL) == false) {
						if (GetLastError () == ERROR_ALREADY_EXISTS) { 
							return true; // - lastExistingDirectoryPosition is equal to pathLengthW. Required return!
						} else { return false; } // Omit `ALREADY_EXISTS` errors. Any other mean permission issues.
					}
				}
				
				{ // Remove created directories.

					// We have atleast created one directory.
					RemoveDirectoryW (path);

					for (u32 i = pathLengthW; i > lastExistingDirectoryPosition; --i) {
						if (path[i] == '\\') {
							path[i] = '\0';
							RemoveDirectoryW (path);
							path[i] = '\\';
						}
					}
				}

			}
			
		}

		return true;
	
		// Try creating the directory to check for access permissions.
		//if (CreateDirectoryW (path, NULL)) {
		//
		//	RemoveDirectoryW (path); // Remove Test directory.
		//	return true;  
		//
		//} else {
		//
		//	if (GetLastError () == ERROR_ALREADY_EXISTS) {
		//		return true; // Directory already exist.
		//	}
		//
		//	return false;  // Cannot create the directory due to permission issues.
		//}
		
	}
	
}
