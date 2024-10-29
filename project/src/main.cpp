// Created 2024.10.28 by Matthew Strumiłło (dotBlueShoes)
//
#include "base/types.hpp"
//
#include "locale/error_codes.hpp"
#include "activities/match.hpp"
#include "instalation/main.hpp"

s32 main (s32 argumentsCount, c8* arguments[]) {

	INSTALLATION::CreateAll ();

	switch (argumentsCount) {

		case 0:
		case 1: {

			printf ("\n\t%s\n\n", LOCALE::ERROR_NO_ACTIVITY);

		} break;

		case 2: {
			auto&& argument = arguments[1];

			u32 argumentLength = 0; for (; argument[argumentLength] != 0; ++argumentLength);
			ACTIVITIES::MATCH::Single (argument, argumentLength);

		} break;

		case 3: {
			
			printf ("\n%s\n\n", arguments[2]);

		} break;

		default: {

		}
		
	}

	return 0;
}
