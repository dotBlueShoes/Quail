// Created 2024.10.28 by Matthew Strumiłło (dotBlueShoes)
//  LICENSE: GNU GENERAL PUBLIC LICENSE Version 3, 29 June 2007
//
#pragma once
#include "blue/comparesearch.hpp"
#include "activities.hpp"
//

namespace ACTIVITIES::MATCH {

void Activity (
	const u32& activityLength,
	c8*& activity,
	const u32& depth,
	c8** const& commands
) {

	if (activityLength < ACTIVITIES::MIN_LENGTH) {

		ERROR ("Invalid argument: %s\n\n", activity);

	} else if (activityLength > ACTIVITIES::MIN_LENGTH && activityLength <= ACTIVITIES::MAX_LENGTH) {

		ToLowCase (activity, activityLength); // Conversion

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

		ToLowCase (activity, activityLength); // Conversion

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