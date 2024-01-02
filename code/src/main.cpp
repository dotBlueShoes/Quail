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
// - Global constants !!!
// - Read project commands


// When using 1 heap buffor theres 2 things i need to take care of
//  with commands i've read from file. 
//
// - COMMAND LENGTH
// - OFFSET FROM 1 COMMAND TO OTHER

// ByPFME wymaga przesłania array'a wartości do których będziemy porównywać naszą wartość.
//  To jest problematyczne, ponieważ nasze wartości są o różnych wielkościach i w różnych odstępach między sobą.
//  Mamy, więc 2 problemy:
//		1. Różne wielkości komend.
//		2. Różne Odstępy między komendami.
// Ad1.
//  Różne wielkości nie są problemem dopóki są one większe od wielkości wartości szukanej.
//   Ponieważ najpierw parsujemy argument później komendy.
//	 a) Można najpierw określić minimalny rozmiar, a później z każdą alokacją {command_name} upewniać się że min jest zaalokowane.
//   b) Określić rozmiar min, i dokleić go do ostatniej {command_name}
//   do SEARCH'a będziemy musieli wysłać główny buffor, tablice pozycji startów wszystkich {command_name} po której będziemy iterować.
//
// Ad2.
//	Odstępy między sobą możemy zczytać już z tablicy


int32 main (
	IN 							const int32 argumentsCount, 
	INREADS (argumentsCount) 	const char** arguments
) {

	DEBUG printf ("%s\n", strings::STRING_DEBUG_MODE_INFO);

	// Initialize Buffers with program start. They'll automaticlly deallocate.
	memoryBlockA.INITIALIZE(1024);
	memoryBlockB.INITIALIZE(512);
	
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
			return ExitCode::SUCCESSFULL_COMMAND_EXECUTION;
		}

		case VALID_COMMAND: {
			//auto&& commandName = arguments[1];
			//auto&& projectName = arguments[2];
			//auto&& subcmmdName = arguments[3];
			return ExitCode::SUCCESSFULL_COMMAND_EXECUTION;
		}

		default: {
			printf("TOO_MANY_ARGUMENTS");
			return ExitCode::FAILURE_TOO_MANY_ARGUMENTS;
		}

	}
	
}