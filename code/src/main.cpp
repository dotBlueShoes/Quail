#include "lib/Framework.hpp"
#include "lib/Search.hpp"

#include "Buffors.hpp"
#include "Arguments/Parse.hpp"

#include "Commands/Help.hpp"
#include "Commands/Open/Open.hpp"


using MyString = array<char, 5, uint8>; // Arguments::CommandFull


MyString searchedFor { "abbde" };
//Arguments::CommandFull searchedFor { 5, "abcde" };
array<MyString, 6> searchedData {
	MyString { "abcde" },
	MyString { "abcde" },
	MyString { "abbde" },
	MyString { "abcde" },
	MyString { "abcde" },
	MyString { "abcde" },
};

Arguments::CommandFull searchedForA { 5, "abbde" };
//Arguments::CommandFull searchedFor { 5, "abcde" };
array<Arguments::CommandFull, 6> searchedDataA {
	Arguments::CommandFull { 5, "abcde" },
	Arguments::CommandFull { 5, "abcde" },
	Arguments::CommandFull { 5, "abbde" },
	Arguments::CommandFull { 5, "abcde" },
	Arguments::CommandFull { 5, "abcde" },
	Arguments::CommandFull { 5, "abcde" },
};


enum ARGUMENTS_CHECK : int32 {
	NO_PATH_CALL_HOW = 0,
	NO_PARAMETERS_CALL = 1,
	INCOMPLETE_COMMAND = 2,
	PROJECT_ONLY = 3,
	VALID_COMMAND = 4,
};


// TODO:
// -  2. Subcommands in main config - 'list', 'config' keywords use
// -  3. Queues (pipes) in main config
// -  5. Projects inside main config
// -  6. Execute Commands
// -  7. Proper include system.
// -  8. '.' implementation.
// -  9. wchar and maybe other...
// - 10. buffor123s checks...


int32 main (
	IN 							const int32 argumentsCount, 
	INREADS (argumentsCount) 	const char** arguments
) {

	DEBUG printf ("%s\n", strings::STRING_DEBUG_MODE_INFO);

	// Initialize Buffers with program start. They'll automaticlly deallocate.
	memoryBlockA.INITIALIZE(2048);					// Stores contents of read files.
	memoryBlockB.INITIALIZE(256);					// Stores processed command / queue.
	// 32 * 4 because we save nextIndex which is llu type.
	memoryBlockC.INITIALIZE((32 * 4) + 32 * 2); 	// Stores quick count tables for Constants, References, Commands, Queues
	
	// 1st. validate arguments count.
	switch (argumentsCount) {

		case ARGUMENTS_CHECK::NO_PATH_CALL_HOW: {
			printf("NO_PATH_CALL_HOW");
			return ExitCode::FAILURE_TOO_LITTLE_ARGUMENTS;
		} 

		case ARGUMENTS_CHECK::NO_PARAMETERS_CALL: {
			Commands::Help::Display();
			return ExitCode::FAILURE_TOO_LITTLE_ARGUMENTS;
		} 

		case ARGUMENTS_CHECK::INCOMPLETE_COMMAND: {
			auto&& commandName = arguments[1];
			return Arguments::Parse::CommandIncomplete(commandName);
		}

		case PROJECT_ONLY: {
			auto&& commandName = arguments[1];
			auto&& projectName = arguments[2];
			return Arguments::Parse::CommandProjectOnly(commandName, projectName);
		}

		case VALID_COMMAND: {
			auto&& commandName = arguments[1];
			auto&& projectName = arguments[2];
			auto&& subcmmdName = arguments[3];
			return Arguments::Parse::Command(commandName, projectName, subcmmdName);
		}

		default: {
			printf("TOO_MANY_ARGUMENTS");
			return ExitCode::FAILURE_TOO_MANY_ARGUMENTS;
		}

	}
	
}