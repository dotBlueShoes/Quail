// ----------------------------------------------------------------
// TODO:
// -  2. Subcommands in main config - 'list', 'config' keywords use
// -  3. Queues (pipes) in main config
// -  5. Projects inside main config
// -  7. Proper include system.
// -  8. '.' implementation.
// -  9. wchar and maybe other...
// - 10. buffor123s checks...
// ----------------------------------------------------------------

// What do i need to do?
// i want to display directory once as 1st and instead displaying it display '.' sign.  


#include "lib/Framework.hpp"
#include "lib/Search.hpp"

#include "Buffors.hpp"
#include "Arguments/Parse.hpp"

#include "Commands/Help.hpp"
#include "Commands/Open/Open.hpp"


// ----------------------------------------------------------------


enum ARGUMENTS_CHECK : u32 {
	NO_PATH_CALL_HOW = 0,
	NO_PARAMETERS_CALL = 1,
	INCOMPLETE_COMMAND = 2,
	PROJECT_ONLY = 3,
	VALID_COMMAND = 4,
};


// ----------------------------------------------------------------


s32 main (
	IN 							const s32 argumentsCount, 
	INREADS (argumentsCount) 	const char** arguments
) {

	// 1. Initialize heat buffers at program START. 
	//	- buffers will automatically deallocate.
	//	- A: parsed content of files, B: processed command / queue, C: quick count tables.
	// 2. Validate Arguments Count
	// 3. Parse Arguments and return SUCCESS or FAILURE 

	DEBUG printf ("%s\n", strings::STRING_DEBUG_MODE_INFO);

	memoryBlockA.INITIALIZE (2048);
	memoryBlockB.INITIALIZE (256);
	memoryBlockC.INITIALIZE ((32 * 4) + 32 * 2);

	DEBUG printf ("%s\n", "Succesfully initalized heat memory for buffers.");
	
	switch (argumentsCount) {

		case ARGUMENTS_CHECK::NO_PATH_CALL_HOW: {
			printf("%s\n", "Error: 0 Arguments thats impossible?!");
			return ExitCode::FAILURE_TOO_LITTLE_ARGUMENTS;
		} 

		case ARGUMENTS_CHECK::NO_PARAMETERS_CALL: {
			Commands::Help::Display ();
			return ExitCode::FAILURE_TOO_LITTLE_ARGUMENTS;
		} 

		case ARGUMENTS_CHECK::INCOMPLETE_COMMAND: {
			auto&& commandName = arguments[1];
			return Arguments::Parse::CommandIncomplete (commandName);
		}

		case PROJECT_ONLY: {
			auto&& commandName = arguments[1];
			auto&& projectName = arguments[2];
			return Arguments::Parse::CommandProjectOnly (commandName, projectName);
		}

		case VALID_COMMAND: {
			auto&& commandName = arguments[1];
			auto&& projectName = arguments[2];
			auto&& subcmmdName = arguments[3];
			return Arguments::Parse::Command (commandName, projectName, subcmmdName);
		}

		default: {
			printf("%s\n", "Error: Too Many Arguments!");
			return ExitCode::FAILURE_TOO_MANY_ARGUMENTS;
		}

	}
	
}