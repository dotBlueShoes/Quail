#pragma once
#include "lib/Framework.hpp"

namespace Arguments {

	const uint8 COMMAND_SHORT_LENGTH = 2;
	const uint8 COMMANDS_COUNT = 2;

	using CommandShort = array<const charConsole, COMMAND_SHORT_LENGTH, uint8>;
	
	struct CommandFull {
		uint8 length;
		const charConsole* name;
	};

	// Opens a specified config for project { quail -o 'PROJECT_NAME' }.
	const CommandShort shortOpen { "-o" };
	//const array<const charConsole, 6, uint8> open { "--open" };

	// HELP
	const CommandShort shortHelp { "-h" };
	//const array<const charConsole, 6, uint8> help { "--help" };

	// FOR VALIDATION PURPOSES ONLY
	const CommandShort shortInvalid { "--" };

	// Array with shorts for better manipulation with data.
	const array<CommandShort, COMMANDS_COUNT + 1> shorts {
		shortOpen, 
		shortHelp, 
		shortInvalid
	};


	const uint8 ARG_OPEN_LENGTH = 6;
	#define ARG_OPEN '-'_i8, '-'_i8, 'o'_i8, 'p'_i8, 'e'_i8, 'n'_i8

	const uint8 ARG_HELP_LENGTH = 6;
	#define ARG_HELP '-'_i8, '-'_i8, 'h'_i8, 'e'_i8, 'l'_i8, 'p'_i8

	// Maximum argument length needed for checking against inside array.
	// We dont initialize that space because we don't need to. If its wrong its gonna be wrong no matter what.
	const uint8 COMMAND_MAX_LENGTH = 16; 
	const uint8 COMMAND_BUFFOR_EXTRA_LENGTH = COMMAND_MAX_LENGTH - ARG_HELP_LENGTH;

	// We add COMMAND_BUFFOR_EXTRA_LENGTH to it. So that when we're doing a check against passed argument we don't get outside this array. 
	const uint8 ARG_FULLS_LENGTH = ARG_OPEN_LENGTH + ARG_HELP_LENGTH + COMMAND_BUFFOR_EXTRA_LENGTH;


	// We add 0 between to make it imposible to write something like --open--help and get --open result.
	const array<const uint8, ARG_FULLS_LENGTH> fulls {
		ARG_OPEN, 0_i8, ARG_HELP
	};

	// Helper data structure to point at proper buffer elements.
	const array<uint8, COMMANDS_COUNT> START_POSITIONS { 
		0_i8, 							// First element with length in front
		(uint8)(1_i8 + ARG_OPEN_LENGTH) // Second element ...
	};


	enum MATCH : int16 {
		IMPOSIBLE = 0,
		OPEN = 1,
		HELP = 2,
		INVALID = 3,
	};

}