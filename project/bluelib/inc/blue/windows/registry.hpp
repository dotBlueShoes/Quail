// Created 2025.01.14 by Matthew Strumiłło (dotBlueShoes)
//  LICENSE: GNU GENERAL PUBLIC LICENSE Version 3, 29 June 2007
//
#pragma once
#include "types.hpp"

namespace WINDOWS::REGISTRY {

	#define CHECK_PROPERTY(error, propertyName) { if (error != ERROR_SUCCESS) ERROR ("Setting up property '%ls' failed.\n\n", propertyName); }

	void CreateKeyMachine (
		OUT 	HKEY& 				key, 
		OUT 	LSTATUS& 			error, 
		OUT 	DWORD& 				status,
		IN	 	const c16* const& 	keyName
	) {
		error = RegCreateKeyExW (
			HKEY_LOCAL_MACHINE, 			// Type
			keyName, 						// Path
			0, 								// RESERVED
			NULL, 							// optional. Class Type
			REG_OPTION_NON_VOLATILE, 		// defualt - store data in file not in RAM.
			KEY_WRITE | KEY_QUERY_VALUE, 	// Access rights for the key to be created.
			NULL, 							// Security descriptor for the new key.
			&key, 							// A pointer to a variable that receives a handle to the opened or created key.
			&status							// Was key created or was it only openned because it arleady exsisted.
		);
	}


	void CreatePropertyC16 (
		OUT		HKEY 				key, 
		OUT		LSTATUS 			error,
		IN		const c16* const& 	property,
		IN		const c16* const& 	data,
		IN		const u32& 			dataLength
	) {
		error = RegSetValueExW (
			key, 							// catalog (key)
			property, 						// name
			0, 								//  RESERVED
			REG_EXPAND_SZ, 					// data type - https://learn.microsoft.com/en-us/windows/win32/sysinfo/registry-value-types
			(LPCBYTE) data, 				// data
			dataLength						// data length
		);
	}


	void CreatePropertyS32 (
		OUT		HKEY 				key, 
		OUT		LSTATUS 			error,
		IN		const c16* const& 	property,
		IN		const u32& 			data
	) {
		error = RegSetValueExW (
			key, 							// catalog (key)
			property, 						// name
			0, 								//  RESERVED
			REG_DWORD, 						// data type - https://learn.microsoft.com/en-us/windows/win32/sysinfo/registry-value-types
			(LPCBYTE) &data, 				// data
			sizeof (data)					// data length
		);
	}

}
