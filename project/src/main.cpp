// Created 2024.10.28 by Matthew Strumiłło (dotBlueShoes)
//
#include "base/types.hpp"
#include "base/log.hpp"
//
#include "locale/error_codes.hpp"
#include "activities/match.hpp"
#include "instalation/main.hpp"

s32 main (s32 argumentsCount, c8* arguments[]) {

	// Align fututre debug-logs
	DEBUG (DEBUG_FLAG_LOGGING) putc ('\n', stdout);

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
			ACTIVITIES::MATCH::Single (argument, argumentLength, depth, commands);
		}

		//case 2: {
		//	auto&& argument = arguments[1];
		//
		//	u32 argumentLength = 0; for (; argument[argumentLength] != 0; ++argumentLength);
		//	ACTIVITIES::MATCH::Single (argument, argumentLength);
		//
		//} break;
		//
		//case 3: {
		//	
		//	printf ("\n%s\n\n", arguments[2]);
		//
		//} break;
		//
		//default: {
		//
		//}
		
	}

	LOGINFO ("Finalized Execution\n\n");

	return 0;
}
