#include "Framework.hpp"

#include "project/Help.hpp"
#include "Validation.hpp"
	
int32 main (
	const int32 argumentsLength, 
	const char** arguments
) {
	
	DEBUG printf("DEBUG Entered program in DEBUG mode!\n");

	// REDO
	//  I will be supporting multiple main-commands followed by their subcommands
	//   The top system needs to be restructured so it will allow me to stop the execution
	//   once we encounter an error durring processing of a command.

	// 0 arg - from where the program was called
	// 1 arg - action command		[-o]
	// 2 arg - action command param	[project_name/special]
	// 3 arg - subcommand			[command_name/special]

	// 0 arg - from where the program was called
	// 1 arg - action command		[-c]
	// 2 arg - action command param	[type/special]
	// 3 arg - subcommand			[template_name/special]

	using namespace Tokens;

	// Validation should
	//  put command and it's parameters onto a queue.

	if (IsEnoughArgumentsPassedToValidate(argumentsLength)) {
		Actions::Initialize();
		uint8 state = Validation::Main(arguments);

		if (state == Validation::success) {
			ActionArgs actionArguments { argumentsLength, arguments };
			Actions::actions[0]( actionArguments );
			return ExitCode::SUCCESSFULL_COMMAND_EXECUTION;
		} 

		Actions::Destroy();
		Commands::Help::Display();
		return ExitCode::FAILURE_MAIN_VALIDATION;
	} 
		
	Commands::Help::Display();
	return ExitCode::FAILURE_ARGUMENT_COUNT;
}