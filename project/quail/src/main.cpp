// Created 2024.10.28 by Matthew Strumiłło (dotBlueShoes)
//  LICENSE: GNU GENERAL PUBLIC LICENSE Version 3, 29 June 2007
//
#include <blue/types.hpp>
#include <blue/log.hpp>
#include <blue/memory.hpp>
//
#include "locale/error_codes.hpp"
#include "activities/match.hpp"
#include "registry.hpp"

s32 main (s32 argumentsCount, c8* arguments[]) {

	DEBUG (DEBUG_FLAG_LOGGING) putc ('\n', stdout); // Align fututre debug-logs

	WINDOWS::REGISTRY::LoadKeyValues ();

	switch (argumentsCount) {

		case 0:
		case 1: {

			printf ("\n    %s\n\n", LOCALE::ERROR_NO_ACTIVITY);

		} break;

		default: {
			auto&& argument = arguments[1];
			auto depth = argumentsCount - 2;
			auto commands = arguments + 2;
		
			u32 argumentLength = 0; for (; argument[argumentLength] != 0; ++argumentLength);
			ACTIVITIES::MATCH::Activity (argumentLength, argument, depth, commands);
		}
		
	}

	LOGINFO ("Finalized Execution\n");
	LOGMEMORY ();
	DEBUG (DEBUG_FLAG_LOGGING) putc ('\n', stdout); // Align debug-logs

	return 0;
}
