// Created 2024.10.28 by Matthew Strumiłło (dotBlueShoes)
//  LICENSE: GNU GENERAL PUBLIC LICENSE Version 3, 29 June 2007
//
#pragma once
#include <blue/types.hpp>
#include <blue/log.hpp>

namespace ACTIVITIES {

	const u8  MAX_ERROR_LENGTH ( 2 ); 
	const c8* ERROR_TYPE   	( "--" );

	//  ABOUT
	// We're ensuring all max params are the same length.
	// TODO. Write why.

	const u8 MAX_LENGTH 	( 2 + 8 );
	const c8 MAX_VERSION	[] ( "--version"	"\0" 		);
	const c8 MAX_SETTINGS	[] ( "--settings" 	""			);
	const c8 MAX_HELP		[] ( "--help"		"\0\0\0\0" 	);
	const c8 MAX_OPEN		[] ( "--open"		"\0\0\0\0" 	);

	const u8 MIN_LENGTH 	( 2 ); 
	const c8 MIN_VERSION 	[] ( "-v" );
	const c8 MIN_SETTINGS 	[] ( "-s" );
	const c8 MIN_OPEN 		[] ( "-o" );
	const c8 MIN_HELP 		[] ( "-h" );
	

	const arr8<const c8*, 5> MAXS_ACTIVITIES {
		MAX_OPEN, MAX_HELP, MAX_SETTINGS, MAX_VERSION, ERROR_TYPE
	};


	const arr8<const c8*, 5> MINS_ACTIVITIES {
		MIN_OPEN, MIN_HELP, MIN_SETTINGS, MIN_VERSION, ERROR_TYPE
	};


	// NOTICE
	//  Directly corresponds to arrays defined above! Remember to change both.
	//
	enum ENUM_ACTIVITIES: u32 {
		ENUM_OPEN		= 0, 
		ENUM_HELP		= 1, 
		ENUM_SETTINGS	= 2,
		ENUM_VERSION	= 3, 
		ENUM_ERROR_TYPE	= 4,
	};

}
