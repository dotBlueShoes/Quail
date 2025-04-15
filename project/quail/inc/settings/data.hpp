// Created 2025.04.15 by Matthew Strumiłło (dotBlueShoes)
//  LICENSE: GNU GENERAL PUBLIC LICENSE Version 3, 29 June 2007
//
#pragma once
#include <blue/error.hpp>

namespace ACTIVITIES::SETTINGS {

	const u16 STRING_SIZE		= 5;
	// + here could add other String values (enum types).
	const c8  STRING_FALSE  	[] ("false" ""  );
	const c8  STRING_TRUE   	[] ("true" "\0" );
	const c8  STRING_ERROR   	[] ("\0\0\0\0\0");

	const arr8<const c8*, 3> STRINGS {
		STRING_FALSE, STRING_TRUE, STRING_ERROR
	};

	const c8 SETTING_WC_DEFAULT [] ("isWideCharacters");
	const c8 SETTING_LS_DEFAULT [] ("listingLineSize");

	const c8 SETTING_WC [] ("iswidecharacters" 	""		);
	const c8 SETTING_LS [] ("listinglinesize" 	"\0"	);

	const arr8<const c8*, 3> SETTINGS {
		SETTING_WC, SETTING_LS, ERROR_TYPE
	};

	enum ENUM_SETTING: u8 {
		ENUM_SETTING_WC 	= 0,
		ENUM_SETTING_LS 	= 1,
		ENUM_SETTING_ERROR 	= 2,
	};

}