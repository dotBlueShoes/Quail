// Created 2024.10.28 by Matthew Strumiłło (dotBlueShoes)
//
#pragma once
#include "activities.hpp"
//
#include "comparesearch.hpp"

namespace ACTIVITIES::MATCH {

void Single (
	const c8* const& activity,
	const u32& activityLength
) {

	// ERROR ( For mins length must be equal to 2. )
	if (activityLength < ACTIVITIES::MIN_LENGTH) {

		ERROR ("\n\tError! Invalid argument: %s\n\n", activity);

	} else if (activityLength > ACTIVITIES::MIN_LENGTH && activityLength <= ACTIVITIES::MAX_LENGTH) {

		u32 index = 0;

		COMPARESEARCH::ArrayPartFirstMatch ( 
			activity, activityLength, sizeof (c8),
			index, 
			ACTIVITIES::MAXS_WITHOUT_ARGUMENTS.size (),
			ACTIVITIES::MAXS_WITHOUT_ARGUMENTS.data ()
		);

		ACTIVITIES::MatchWithoutArguments (activity, activityLength, index);

	} else if (activityLength > ACTIVITIES::MAX_LENGTH) {

		ERROR ("\n\tError! Invalid argument: %s\n\n", activity);

	} else { // Matching with MIN activities
		u32 index = 0;

		COMPARESEARCH::ArrayPartFirstMatch ( 
			activity, activityLength, sizeof (c8),
			index, 
			ACTIVITIES::MINS_WITHOUT_ARGUMENTS.size (),
			ACTIVITIES::MINS_WITHOUT_ARGUMENTS.data ()
		);
		
		ACTIVITIES::MatchWithoutArguments (activity, activityLength, index);
	}
	
}

}