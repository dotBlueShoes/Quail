#pragma once
#include "lib/Framework.hpp"

namespace Arguments {

	const uint8 COMMAND_SHORT_LENGTH = 2;
	using CommandShort = array<const charConsole, COMMAND_SHORT_LENGTH, uint8>;
	
	struct CommandFull {
		uint8 length;
		const charConsole* name;
	};

	// Opens a specified config for project { quail -o 'PROJECT_NAME' }.
	const CommandShort shortOpen { "-o" };
	const array<const charConsole, 6, uint8> open { "--open" };

	// SAMPLE
	const CommandShort shortSample { "-s" };
	const array<const charConsole, 8, uint8> sample { "--sample" };

	// FOR VALIDATION PURPOSES ONLY
	const CommandShort shortInvalid { "--" };
	const array<const charConsole, 3, uint8> invalid { "---" };

	// Array with shorts for better manipulation with data.
	const array<CommandShort, 3> shorts {
		shortOpen, 
		shortSample, 
		shortInvalid
	};

	// Array with fulls for better manipulation with data.
	const array<const CommandFull, 3> fulls {
		CommandFull { open.Length(), open.Pointer() },
		CommandFull { sample.Length(), sample.Pointer() },
		CommandFull { invalid.Length(), invalid.Pointer() }
	};

	enum MATCH : int16 {
		POSSIBLE_FULL_COMMAND = -2,
		TOO_LONG = -2,
		TOO_SHORT = -1,
		IMPOSIBLE = 0,
		OPEN = 1,
		SAMPLE = 2,
		INVALID = 3,
	};

	// Check for "-*".
	getter int16 ValidateShort(
		const charConsole* const commandName
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
	getter int16 ValidateFull(
		const charConsole* const commandName, 
		const uint8& commandLength
	) {

		// 0 - TRUE, {1, 2} - FALSE
		//uint8 validator = (commandName[0] != '-');
		//validator += (commandName[1] != '-');

		auto onNoMatchFound = []() { 
			printf ("%s\n", Search::STRING_SEARCH_ARRAY_BYPFM_ERROR);
			exit (ExitCode::FAILURE_INVALID_ARGUMENT);
		};

		uint16 resultIndex (0);
	
		Search::Array::ByPFME <charConsole, uint16> (
			onNoMatchFound, resultIndex,
			commandLength, commandName,
			fulls.Length(), (any*)fulls.Pointer(),
			sizeof (CommandFull), sizeof (uint8) /* offset in type */
		);
	
		printf ("Result: %i\n", resultIndex);

		return 0;
	}

}