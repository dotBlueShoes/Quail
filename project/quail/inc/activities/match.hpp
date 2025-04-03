// Created 2024.10.28 by Matthew Strumiłło (dotBlueShoes)
//  LICENSE: GNU GENERAL PUBLIC LICENSE Version 3, 29 June 2007
//
#pragma once
#include "blue/comparesearch.hpp"
#include "activities.hpp"

namespace ACTIVITIES::MATCH {

	void Activity (
		const u32& activityLength,
		c8*& activity,
		const u32& depth,
		c8** const& commands
	) {

		if (activityLength < MIN_LENGTH) {

			ERROR ("Invalid argument: %s\n\n", activity);

		} else if (activityLength > MIN_LENGTH && activityLength <= MAX_LENGTH) {

			ToLowCase (activity, activityLength); // Conversion

			u32 index = 0;

			// Match with MAX's
			COMPARESEARCH::ArrayPartFirstMatch ( 
				activity, activityLength, sizeof (c8),
				index, 
				MAXS_WITHOUT_ARGUMENTS.size (),
				MAXS_WITHOUT_ARGUMENTS.data ()
			);

			TranslateIndex (activity, activityLength, index, depth, commands);

		} else if (activityLength > MAX_LENGTH) {

			ERROR ("Invalid argument: %s\n\n", activity);

		} else {

			ToLowCase (activity, activityLength); // Conversion

			u32 index = 0;

			// Match with MIN's
			COMPARESEARCH::ArrayPartFirstMatch ( 
				activity, activityLength, sizeof (c8),
				index, 
				MINS_WITHOUT_ARGUMENTS.size (),
				MINS_WITHOUT_ARGUMENTS.data ()
			);

			TranslateIndex (activity, activityLength, index, depth, commands);

		}

	}

}
