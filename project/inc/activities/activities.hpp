// Created 2024.10.28 by Matthew Strumiłło (dotBlueShoes)
//
#pragma once
#include "base/types.hpp"
#include "base/log.hpp"
//
#include "locale/error_codes.hpp"
//
#include "activities/version.hpp"
#include "activities/help.hpp"
//
#include "open/main.hpp"

namespace ACTIVITIES {

	const u8  MAX_ERROR_LENGTH ( 2 ); 
	const c8* ERROR_TYPE   	( "--" );

	const u8  MAX_LENGTH 		  ( 2 + 7 );
	const c8* MAX_EXAMPLE	( "--example" );
	const c8* MAX_VERSION	( "--version" );
	const c8* MAX_HELP		( "--help\0\0\0" );
	const c8* MAX_OPEN		( "--open\0\0\0" );
	const c8* MAX_LIST		( "--list\0\0\0" );

	const u8  MIN_LENGTH     ( 2 ); 
	const c8* MIN_VERSION ( "-v" );
	const c8* MIN_LIST    ( "-l" );
	const c8* MIN_OPEN    ( "-o" );
	const c8* MIN_HELP	  ( "-h" );
	

	const arr8<const c8*, 5> MAXS_WITHOUT_ARGUMENTS {
		MAX_OPEN, MAX_LIST, MAX_HELP, MAX_VERSION, ERROR_TYPE
	};

	const arr8<const c8*, 2> MAXS_WITH_ARGUMENTS {
		MAX_OPEN, ERROR_TYPE
	};


	const arr8<const c8*, 5> MINS_WITHOUT_ARGUMENTS {
		MIN_OPEN, MIN_LIST, MIN_HELP, MIN_VERSION, ERROR_TYPE
	};

	const arr8<const c8*, 2> MINS_WITH_ARGUMENTS {
		MIN_OPEN, ERROR_TYPE
	};


	const u8 MINS_WITHOUT_ARGUMENTS_ERROR 	(MINS_WITHOUT_ARGUMENTS.size () - 1);
	const u8 MAXS_WITHOUT_ARGUMENTS_ERROR 	(MAXS_WITHOUT_ARGUMENTS.size () - 1);
	const u8 MINS_WITH_ARGUMENTS_ERROR 		(MINS_WITH_ARGUMENTS.size () - 1);
	const u8 MAXS_WITH_ARGUMENTS_ERROR 		(MAXS_WITH_ARGUMENTS.size () - 1);


	void TranslateIndex (
		const c8* const& activity,
		const u32& activityLength,
		const u32& index,
		const u32& depth,
		c8** const& commands
	) {
		switch (index) {

			case 0: { // OPEN
				// TODO ( display contents of main config file )
				//printf ("\n\tquail open!\n\n");
				OPEN::Open (depth, commands);
			} break;

			case 1: { // LIST
				LOGINFO ("`quail list`!\n\n"); // TODO ( display contents of main config file )
			} break;

			case 2: { // HELP
				printf ("%s", HELP_ALL);
			} break;

			case 3: { // VERSION
				printf ("%s", VERSION_ALL);
			} break;

			default: {
				ERROR ("`quail %s` Invalid argument: %s\n\n", activity, activity);
			} 

		}
	}

}

