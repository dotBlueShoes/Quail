// Created 2024.10.28 by Matthew Strumiłło (dotBlueShoes)
//  LICENSE: GNU GENERAL PUBLIC LICENSE Version 3, 29 June 2007
//
#pragma once
#include "blue/comparesearch.hpp"
#include "activities.hpp"
//
#include "locale/error_codes.hpp"
//
#include "activities/version.hpp"
#include "activities/help.hpp"
//
#include "settings/settings.hpp"
#include "open/open.hpp"

namespace ACTIVITIES::MATCH {

	void TranslateIndex (
		IN		const c8* const&    activity,
		IN		const u32&          activityLength,
		IN		const u32&          activityId,
		IN		const u32&          actionsCount,
		INOUT	c8** const&         actions,
        INOUT	c8* const&          actionsArgs
	) {
		switch (activityId) {

			case ENUM_OPEN: {
				OPEN::Open (actionsCount, actions, actionsArgs);
			} break;

			case ENUM_HELP: {
				printf ("\n%s%s", CONFIG::QUAIL_LINE_OFFSET, HELP_ALL);
			} break;

			case ENUM_VERSION: {
				printf ("%s", VERSION_ALL);
			} break;

			case ENUM_SETTINGS: {
				SETTINGS::Settings (actionsCount, actions);
			} break;

			default: {
				ERROR ("`quail %s` Invalid argument: %s\n\n", activity, activity);
			} 

		}
	}

	
	void Activity (
		IN		u32& activityLength,
		INOUT	c8*& activity,
		IN		const u32& actionsCount,
		IN		c8** const& actions,
        IN		c8* const& actionsArgs
	) {

		u8 condition;
		condition  = (activityLength == MIN_LENGTH) << 0;
		condition += (activityLength  < MIN_LENGTH) << 1;
		condition += (activityLength  > MAX_LENGTH) << 2;

		switch (condition) {

			case 0: { // Use of MAX's activities.

				u32 index = 0;

				// Conversion
				ToLowCase (activity, activityLength); 

				// Try match with MAXs
				COMPARESEARCH::ArrayPartFirstMatch ( 
					activity, activityLength, sizeof (c8),
					index, 
					MAXS_ACTIVITIES.size (),
					MAXS_ACTIVITIES.data ()
				);

				// Progress further
				TranslateIndex (activity, activityLength, index, actionsCount, actions, actionsArgs);

			} break;

			case 1: { // Use of Min's activities.

				u32 index = 0;

				// Conversion
				ToLowCase (activity, activityLength);

				// Try match with MINs
				COMPARESEARCH::ArrayPartFirstMatch ( 
					activity, activityLength, sizeof (c8),
					index, 
					MINS_ACTIVITIES.size (),
					MINS_ACTIVITIES.data ()
				);

				// Progress further
				TranslateIndex (activity, activityLength, index, actionsCount, actions, actionsArgs);

			} break;

			default: ERROR ("Invalid argument: %s\n\n", activity);

		}

	}

}
