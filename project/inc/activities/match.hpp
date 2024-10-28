// Created 2024.10.28 by Matthew Strumiłło (dotBlueShoes)
//
#pragma once
#include "activities.hpp"
//
#include "locale/error_codes.hpp"
#include "comparesearch.hpp"
#include "base/log.hpp"

namespace ACTIVITIES::MATCH {

void Single (
	const c8* const& activity,
	const u32& activityLength
) {

	// ERROR ( For mins length must be equal to 2. )
	if (activityLength < ACTIVITIES::MIN_LENGTH) {

		ERROR ("\n\tError! Invalid argument: %s\n\n", activity);

	} else if (activityLength > ACTIVITIES::MIN_LENGTH && activityLength < ACTIVITIES::MAX_LENGTH) {

		// TODO MAX_activity
		exit (0);

	} else if (activityLength > ACTIVITIES::MAX_LENGTH) {

		ERROR ("\n\tError! Invalid argument: %s\n\n", activity);

	}

	{ // Matching with MIN activities
		u32 index = 0;

		COMPARESEARCH::ArrayPartFirstMatch ( 
			activity, activityLength, sizeof (c8),
			index, 
			ACTIVITIES::MINS_WITHOUT_ARGUMENTS.size (),
			ACTIVITIES::MINS_WITHOUT_ARGUMENTS.data ()
		);

		// ERROR
		if (index == ACTIVITIES::MINS_WITHOUT_ARGUMENTS_ERROR) {
			ERROR ("\n\t`quail %s` Error! Invalid argument: %s\n\n", activity, activity);
		}

		// SUCCESS
		printf ("\n\tquail command: %s\n\n", ACTIVITIES::MINS_WITHOUT_ARGUMENTS[index]);
	}
	
}

}