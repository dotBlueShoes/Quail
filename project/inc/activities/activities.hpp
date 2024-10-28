// Created 2024.10.28 by Matthew Strumiłło (dotBlueShoes)
//
#pragma once
#include "base/types.hpp"

namespace ACTIVITIES {

	const u8  MAX_ERROR_LENGTH ( 2 ); 
	const c8* ERROR    		( "--" );

	const u8  MAX_LENGTH 		  ( 2 + 7 );
	const c8* MAX_EXAMPLE	( "--example" );
	const c8* MAX_HELP		( "--help\0\0\0" );
	const c8* MAX_OPEN		( "--open\0\0\0" );
	const c8* MAX_LIST		( "--list\0\0\0" );

	const u8  MIN_LENGTH  ( 2 ); 
	const c8* MIN_LIST ( "-l" );
	const c8* MIN_OPEN ( "-o" );
	const c8* MIN_HELP ( "-h" );
	

	const arr8<const c8*, 4> MAXS_WITHOUT_ARGUMENTS {
		MAX_HELP, MAX_OPEN, MAX_LIST, ERROR
	};

	const arr8<const c8*, 2> MAXS_WITH_ARGUMENTS {
		MAX_OPEN, ERROR
	};


	const arr8<const c8*, 4> MINS_WITHOUT_ARGUMENTS {
		MIN_HELP, MIN_OPEN, MIN_LIST, ERROR
	};

	const arr8<const c8*, 2> MINS_WITH_ARGUMENTS {
		MIN_OPEN, ERROR
	};


	const u8 MINS_WITHOUT_ARGUMENTS_ERROR 	(MINS_WITHOUT_ARGUMENTS.size () - 1);
	const u8 MAXS_WITHOUT_ARGUMENTS_ERROR 	(MAXS_WITHOUT_ARGUMENTS.size () - 1);
	const u8 MINS_WITH_ARGUMENTS_ERROR 		(MINS_WITH_ARGUMENTS.size () - 1);
	const u8 MAXS_WITH_ARGUMENTS_ERROR 		(MAXS_WITH_ARGUMENTS.size () - 1);

}

