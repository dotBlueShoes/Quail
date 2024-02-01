#pragma once
#include "lib/Framework.hpp"

#include "Arguments.hpp"


namespace Arguments::Validate {

	// Check for "-*".
	//
	getter int16 Short (
		IN const charConsole* const commandName
	) {

		const uint8 COMMANDS_SHORTS_LENGTH = shorts.Length();

		uint8 i = 0, j = COMMANDS_SHORTS_LENGTH;

		// CHECK SIGN. 0 when TRUE, 1 when FALSE
		uint8 validator = (commandName[0] != '-');

		// GET COMMAND MATCH, {i} will always grow by 1.
		for (; i < j;) {
			// When x != y iterate next. When x == y break by condition changing. I
			j = (commandName[1] != shorts[i][1]) * COMMANDS_SHORTS_LENGTH;
			++i;
		}

		// CHECK VALIDATION + COMMAND MATCH. & RUN
		return i + (validator * COMMANDS_SHORTS_LENGTH);

	}


	// Check for "--*...".
	//
	getter int16 Full (
		IN 						const uint8& commandLength,
		INREADS (commandLength) const charConsole* const commandName
	) {

		auto onNoMatchFound = [](uint16& resultIndex, const uint8 elementsCount) { 
			printf ("%s\n", Search::Array::STRING_SEARCH_BYPFME_ERROR);
			exit (ExitCode::FAILURE_INVALID_ARGUMENT);
		};

		uint16 resultIndex (0);

		Search::Buffor::ByPFM<charConsole, uint16> (
			onNoMatchFound, resultIndex,
			commandLength, commandName,
			fulls.Pointer(), START_POSITIONS.Length(), START_POSITIONS.Pointer()
		);

		return resultIndex + 1;
	}

}