#include "lib/Framework.hpp"
#include "lib/Search.hpp"

#include "Buffors.hpp"
#include "IO/Projects.hpp"
#include "Commands.hpp"


using MyString = array<char, 5>;


//MyString searchedFor { 'a', 'b', 'c', 'd', 'e' };
//array<MyString, 6> searchedData {
//	MyString { 'f', 'b', 'c', 'd', 'e' },
//	MyString { 'b', 'b', 'c', 'd', 'e' },
//	MyString { 'a', 'b', 'c', 'd', 'e' },
//	MyString { 'g', 'b', 'c', 'd', 'e' },
//	MyString { 'd', 'b', 'c', 'd', 'e' },
//	MyString { 'e', 'b', 'c', 'd', 'e' },
//};


const int32 NO_PATH_CALL_HOW = 0;
const int32 NO_PARAMETERS_CALL = 1;
const int32 INCOMPLETE_COMMAND = 2;
const int32 VALID_COMMAND = 3;


// TODO:
// - ENUMS for switches
// - Move incomplete command execution somewhere else.
// - Valid command logic
// - Global variables
// - Read project commands


int32 main (
	IN 							const int32 argumentsCount, 
	INREADS (argumentsCount) 	const char** arguments
) {

	DEBUG printf ("%s\n", strings::STRING_DEBUG_MODE_INFO);

	// Initialize Buffers with program start. They'll automaticlly deallocate.
	//memoryBlockA.INITIALIZE(4096);
	memoryBlockA.INITIALIZE(1024);
	memoryBlockB.INITIALIZE(512);
	
	switch (argumentsCount) {

		case NO_PATH_CALL_HOW: {
			printf("NO_PATH_CALL_HOW");
		} break;

		case NO_PARAMETERS_CALL: {
			printf("NO_PARAMETERS_CALL");
		} break;

		case INCOMPLETE_COMMAND: {

			auto&& commandName = arguments[1];
			uint8 validator, length = 0;

			// GET LENGTH
			for (; commandName[length] != '\0'; ++length);

			// SEE IF LENGTH MATCHES
			if (length == Commands::COMMAND_SHORT_LENGTH) {

				// CHECK SIGN. 0 when TRUE, 1 when FALSE
				validator = (commandName[0] != '-');

				const uint8 COMMANDS_SHORTS_LENGTH = Commands::shorts.Length();
				uint8 i = 0, j = COMMANDS_SHORTS_LENGTH;

				// GET COMMAND MATCH, {i} will always grow by 1.
				for (; i < j;) {
					// When x != y iterate next. When x == y break by condition changing. I
					j = (commandName[1] != Commands::shorts[i][1]) * COMMANDS_SHORTS_LENGTH;
					++i;
				}

				// CHECK VALIDATION + COMMAND MATCH. & RUN
				switch (i + (validator * COMMANDS_SHORTS_LENGTH)) {

					case 0: {
						// {i} always points up by 1. 
						printf("IMPOSIBLE_CASE");
					} break;

					case 1: {
						// VALID CASE - OPEN
						IO::GetProjectsData();
					} break;

					case 2: {
						// VALID CASE - SAMPLE
						printf("VALID_CASE_SAMPLE");
					} break;

					case 3: {
						// INVALID COMMAND SIGN
						printf("INVALID_COMMAND_SIGN");
					} break;

					default: {
						// INVALID NO DASH
						printf("INVALID_NO_DASH");
					}

				}
				

			} else {
				// INVALID COMMAND SIGN
				printf("INVALID_COMMAND_LENGTH");
			}

		} break;

		case VALID_COMMAND: {
			IO::GetProjectsData();
		} break;

		default: {
			printf("TOO_MANY_ARGUMENTS");
		}

	}


	
	

	//{ // By Part Search Example
	//	auto onNoMatchFound = []() { 
	//		printf ("%s\n", Search::STRING_SEARCH_ARRAY_BYPFM_ERROR);
	//		exit (ExitCode::FAILURE_UNKNOWN_COMMAND_TYPE);
	//	};
	//
	//	uint16 resultIndex (0);
	//
	//	Search::Array::ByPFM <char, uint16> (
	//		onNoMatchFound, resultIndex,
	//		searchedFor.Length(), searchedFor.Pointer(),
	//		searchedData.Length(), (any*)searchedData.Pointer(),
	//		sizeof (MyString), sizeof (size) /* offset in type */
	//	);
	//
	//	printf ("Result: %i\n", resultIndex);
	//}

	return ExitCode::SUCCESSFULL_COMMAND_EXECUTION;
}