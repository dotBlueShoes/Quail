// Created 2024.10.28 by Matthew Strumiłło (dotBlueShoes)
//
#pragma once
#include "activities.hpp"
//
#include "comparesearch.hpp"

namespace ACTIVITIES::MATCH {

void Activity (
	const c8* const& activity,
	const u32& activityLength,
	const u32& depth,
	const c8* const* const& commands
) {

	if (activityLength < ACTIVITIES::MIN_LENGTH) {

		ERROR ("Invalid argument: %s\n\n", activity);

	} else if (activityLength > ACTIVITIES::MIN_LENGTH && activityLength <= ACTIVITIES::MAX_LENGTH) {

		u32 index = 0;

		// Match with MAX's
		COMPARESEARCH::ArrayPartFirstMatch ( 
			activity, activityLength, sizeof (c8),
			index, 
			ACTIVITIES::MAXS_WITHOUT_ARGUMENTS.size (),
			ACTIVITIES::MAXS_WITHOUT_ARGUMENTS.data ()
		);

		ACTIVITIES::TranslateIndex (activity, activityLength, index, depth, commands);

	} else if (activityLength > ACTIVITIES::MAX_LENGTH) {

		ERROR ("Invalid argument: %s\n\n", activity);

	} else {

		u32 index = 0;

		// Match with MIN's
		COMPARESEARCH::ArrayPartFirstMatch ( 
			activity, activityLength, sizeof (c8),
			index, 
			ACTIVITIES::MINS_WITHOUT_ARGUMENTS.size (),
			ACTIVITIES::MINS_WITHOUT_ARGUMENTS.data ()
		);
		
		ACTIVITIES::TranslateIndex (activity, activityLength, index, depth, commands);
		
	}
	
}

}