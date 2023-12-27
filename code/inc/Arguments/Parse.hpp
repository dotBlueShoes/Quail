#pragma once
#include "lib/Framework.hpp"

#include "Commands.hpp"

namespace Arguments::Parse {

	getter int32 CommandIncomplete(const char* const commandName) {

		// GET COMMAND LENGTH
		uint8 commandLength = 0; 
		for (; commandName[commandLength] != '\0'; ++commandLength);

		// POSSIBLE SHORT COMMAND
		if (commandLength == COMMAND_SHORT_LENGTH) {

			switch (ValidateShort(commandName)) {

				case MATCH::IMPOSIBLE: {
					printf("FAILRE: IMPOSIBLE_CASE?");
				} return ExitCode::FAILURE_UNKNOWN;

				case MATCH::OPEN: {
					IO::GetProjectsData();
				} return ExitCode::SUCCESSFULL_COMMAND_EXECUTION;

				case MATCH::SAMPLE: {
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
		} else if (commandLength > COMMAND_SHORT_LENGTH) {
			
			//printf ("%i\n", commandLength);
			//fwrite(open.Pointer(), sizeof(char), open.Length(), stdout);
			//fwrite("\n", sizeof(char), 1, stdout);
			//fwrite(sample.Pointer(), sizeof(char), sample.Length(), stdout);
			//fwrite("\n", sizeof(char), 1, stdout);

			switch (ValidateFull(commandName, commandLength)) {

				case MATCH::IMPOSIBLE: {
					printf("FAILRE: IMPOSIBLE_CASE?");
				} return ExitCode::FAILURE_UNKNOWN;

				case MATCH::OPEN: {
					IO::GetProjectsData();
				} return ExitCode::SUCCESSFULL_COMMAND_EXECUTION;

				case MATCH::SAMPLE: {
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