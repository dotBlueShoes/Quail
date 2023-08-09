#pragma once
#include "Framework.hpp"

#include "Actions.hpp"

namespace Tokens::Validation {

    const uint8 failure = 0;
    const uint8 success = 1;

    [[nodiscard]] uint8 Main(
		//const int32 argumentsLength, 
		const char** arguments
	) {
		auto& mainCommand = arguments[1];
		//auto& mainCommandArgument = arguments[2];

		if (mainCommand[0] != '-') {
			DEBUG std::cerr << "fail\n";
			return failure;
		}

		if (mainCommand[1] != '-') {

			DEBUG std::cout << "short\n";

			for (uint8 i = 0; i < shorts.Length(); ++i) {
				if (mainCommand[1] == shorts[i][1]) {
					DEBUG std::cout << "command found\n";
                    Actions::AssignMainAction(i);
					return success;
				}
			}

			DEBUG std::cerr << "fail\n";
            return failure;

		} else {

			DEBUG std::cout << "long\n";

			// We're matching by length if theres multiple commands with the same length return fail.
			//  TODO:
			// We don't need to look for collision ...
			//  we know from the begining that there are comamnds with same length.
			//  so we can say that for specific lengths don't compute Length Search
			//  go collision - subbox relation instead.

			uint8 matchedCommandIndex = 0;
			uint8 commandLength = 0;
			uint8 collision = 0;

			// Get Length
			for (; mainCommand[commandLength] != '\0'; ++commandLength);
			
			// Length search
			for (uint8 i = 0; i < commands.Length(); ++i)
				if (commandLength == commands[i].length) {
					collision += commands[i].buffor[2];
					matchedCommandIndex = i; 
				}

			// If collisions is equal to the match it means there was no collision.
			if (collision == commands[matchedCommandIndex].buffor[2]) { // 2 because "--[n]ame" we're colliding first chars!
				DEBUG std::cout << "command found\n";
                Actions::AssignMainAction(matchedCommandIndex);
				return success;
			} else {
				DEBUG std::cerr << "fail\n";
				return failure;
				// Otherwise it means there was a collision. - multiple commands with same size.
				//  therefore now the 'collision' value shoud point us in "subbox" to what commands have collided
				//  and based on that a proper check for both commands should be runned.
			}

			//DEBUG std::cerr << "fail\n";
            //return failure;

		}

	}

}