#pragma once
#include "lib/Framework.hpp"

namespace Commands {

	const uint8 COMMAND_SHORT_LENGTH = 2;
	using CommandShort = array<char, COMMAND_SHORT_LENGTH, uint8>;

	// Opens a specified config for project { quail -o 'PROJECT_NAME' }.
	CommandShort shortOpen { "-o" };
	array<char, 6, uint8> open { "--open" };

	// 
	CommandShort shortSample { "-s" };
	array<char, 8, uint8> sample { "--sample" };

	CommandShort invalid { "--" };

	array<CommandShort, 3> shorts {
		shortOpen, 
		shortSample, 
		invalid
	};

}