// Created 2024.10.28 by Matthew Strumiłło (dotBlueShoes)
//  LICENSE: GNU GENERAL PUBLIC LICENSE Version 3, 29 June 2007
//
#pragma once
#include "blue/comparesearch.hpp"
#include "activities.hpp"
//
#include "locale/error_codes.hpp"
//
#include "activities/settings.hpp"
#include "activities/version.hpp"
#include "activities/help.hpp"
//
#include "open/open.hpp"

namespace ACTIVITIES::MATCH {

	void TranslateIndex (
		IN		const c8* const& activity,
		IN		const u32& activityLength,
		IN		const u32& activityId,
		IN		const u32& stringsCount,
		INOUT	c8** const& strings
	) {
		switch (activityId) {

			case ENUM_OPEN: {
				OPEN::Open (stringsCount, strings);
			} break;

			case ENUM_HELP: {
				printf ("\n%s%s", CONFIG::QUAIL_LINE_OFFSET, HELP_ALL);
			} break;

			case ENUM_VERSION: {
				printf ("%s", VERSION_ALL);
			} break;

			case ENUM_SETTINGS: {
				Settings (stringsCount, strings);
			} break;

			default: {
				ERROR ("`quail %s` Invalid argument: %s\n\n", activity, activity);
			} 

		}
	}

	void Activity (
		IN		u32& activityLength,
		INOUT	c8*& activity,
		IN		const u32& stringsCount,
		IN		c8** const& strings
	) {

		u8 condition;
		condition  = (activityLength == MIN_LENGTH) << 0;
		condition += (activityLength  < MIN_LENGTH) << 1;
		condition += (activityLength  > MAX_LENGTH) << 2;

		switch (condition) {

			case 0 + 0 + 0: { 

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
				TranslateIndex (activity, activityLength, index, stringsCount, strings);

			} break;

			case 1 + 0 + 0: { 

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
				TranslateIndex (activity, activityLength, index, stringsCount, strings);

			} break;

			default: {
				ERROR ("Invalid argument: %s\n\n", activity);
			}

			//case 0 + 2 + 0: { 
			//	LOGINFO ("TO SMALL\n"); 
			//} break;
			//
			//case 0 + 0 + 4: { 
			//	LOGINFO ("TO LONG\n"); 
			//} break;

		}

		//if (activityLength < MIN_LENGTH) {
		//
		//	ERROR ("Invalid argument: %s\n\n", activity);
		//
		//} else if (activityLength > MIN_LENGTH && activityLength <= MAX_LENGTH) {
		//
		//	ToLowCase (activity, activityLength); // Conversion
		//
		//	u32 index = 0;
		//
		//	// Try match with MAXs
		//	COMPARESEARCH::ArrayPartFirstMatch ( 
		//		activity, activityLength, sizeof (c8),
		//		index, 
		//		MAXS_ACTIVITIES.size (),
		//		MAXS_ACTIVITIES.data ()
		//	);
		//
		//	TranslateIndex (activity, activityLength, index, depth, commands);
		//
		//} else if (activityLength > MAX_LENGTH) {
		//
		//	ERROR ("Invalid argument: %s\n\n", activity);
		//
		//} else {
		//
		//	ToLowCase (activity, activityLength); // Conversion
		//
		//	u32 index = 0;
		//
		//	// Try match with MINs
		//	COMPARESEARCH::ArrayPartFirstMatch ( 
		//		activity, activityLength, sizeof (c8),
		//		index, 
		//		MINS_ACTIVITIES.size (),
		//		MINS_ACTIVITIES.data ()
		//	);
		//
		//	TranslateIndex (activity, activityLength, index, depth, commands);
		//
		//}

	}

}
