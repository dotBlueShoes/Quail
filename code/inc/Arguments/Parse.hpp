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

		auto onNoMatchFound = []() { 
			printf ("%s\n", Search::Array::STRING_SEARCH_BYPFME_ERROR);
			exit (ExitCode::FAILURE_INVALID_ARGUMENT);
		};

		uint16 resultIndex (0);

		// Helper data structure to point at proper buffer elements.
		const array<uint8, COMMANDS_COUNT> START_POSITIONS { 
			0_i8, 							// First element with length in front
			(uint8)(1_i8 + ARG_OPEN_LENGTH) // Second element ...
		};

		Search::Buffor::ByPFM<charConsole, uint16> (
			onNoMatchFound, resultIndex,
			commandLength, commandName,
			fulls.Pointer(), START_POSITIONS.Length(), START_POSITIONS.Pointer()
		);

		return resultIndex + 1;
	}

}


namespace Arguments::Parse {

	getter int32 CommandIncomplete(const char* const commandName) {

		// GET COMMAND LENGTH
		uint8 commandLength = 0; 
		for (; commandName[commandLength] != '\0'; ++commandLength);

		// POSSIBLE SHORT COMMAND
		if (commandLength == COMMAND_SHORT_LENGTH) {

			switch (Validate::Short(commandName)) {

				case MATCH::IMPOSIBLE: {
					printf("FAILRE: IMPOSIBLE_CASE?");
				} return ExitCode::FAILURE_UNKNOWN;

				case MATCH::OPEN: {
					IO::GetProjectsData();
				} return ExitCode::SUCCESSFULL_COMMAND_EXECUTION;

				case MATCH::HELP: {
					printf("SUCCESS: VALID_CASE_SAMPLE");
				} return ExitCode::SUCCESSFULL_COMMAND_EXECUTION;

				case MATCH::INVALID: {
					printf("FAILURE: INVALID_COMMAND_SIGN");
				} return ExitCode::FAILURE_INVALID_ARGUMENT;

				default: {
					printf("FAILURE: INVALID_NO_DASH");
				} return ExitCode::FAILURE_INVALID_ARGUMENT;

			}

		// POSIIBLE FULL COMMAND
		} else if (commandLength > COMMAND_SHORT_LENGTH && commandLength <= COMMAND_MAX_LENGTH) {

			switch (Validate::Full(commandLength, commandName)) {

				case MATCH::IMPOSIBLE: {
					printf("FAILRE: IMPOSIBLE_CASE?");
				} return ExitCode::FAILURE_UNKNOWN;

				case MATCH::OPEN: {
					IO::GetProjectsData();
				} return ExitCode::SUCCESSFULL_COMMAND_EXECUTION;

				case MATCH::HELP: {
					printf("SUCCESS: VALID_CASE_SAMPLE");
				} return ExitCode::SUCCESSFULL_COMMAND_EXECUTION;

				case MATCH::INVALID: {
					printf("FAILURE: INVALID_COMMAND_SIGN");
				} return ExitCode::FAILURE_INVALID_ARGUMENT;

				default: {
					printf("FAILURE: INVALID_NO_DASH");
				} return ExitCode::FAILURE_INVALID_ARGUMENT;

			}

		} else {
			// INVALID COMMAND LENGTH.
			return ExitCode::FAILURE_ARGUMENT_COUNT;
		}

	}

}