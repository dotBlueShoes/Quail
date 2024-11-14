// Created 2024.10.28 by Matthew Strumiłło (dotBlueShoes)
//
#include "base/types.hpp"
#include "base/log.hpp"
#include "base/memory.hpp"
//
#include "locale/error_codes.hpp"
#include "activities/match.hpp"
#include "instalation/main.hpp"

s32 main (s32 argumentsCount, c8* arguments[]) {

	DEBUG (DEBUG_FLAG_LOGGING) putc ('\n', stdout); // Align fututre debug-logs

	INSTALLATION::CreateAll ();

	switch (argumentsCount) {

		case 0:
		case 1: {

			ERROR ("%s\n\n", LOCALE::ERROR_NO_ACTIVITY);

		} break;

		default: {
			auto&& argument = arguments[1];
			auto depth = argumentsCount - 2;
			auto commands = arguments + 2;
		
			u32 argumentLength = 0; for (; argument[argumentLength] != 0; ++argumentLength);
			ACTIVITIES::MATCH::Activity (argument, argumentLength, depth, commands);
		}
		
	}

	LOGINFO ("Finalized Execution\n");
	LOGMEMORY ();
	DEBUG (DEBUG_FLAG_LOGGING) putc ('\n', stdout); // Align debug-logs

	return 0;
}
