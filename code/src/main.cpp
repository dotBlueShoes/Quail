#include "lib/Framework.hpp"
#include "lib/Search.hpp"

#include "Buffors.hpp"
#include "IO/Projects.hpp"
#include "Arguments/Parse.hpp"


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
	VALID_COMMAND = 3,
};


// TODO:
// - Valid command logic
// - Global variables
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

	
	//fwrite(searchedDataA[2].name, sizeof(char), searchedDataA[2].length, stdout);

	//{ // By Part Search Example
	//	auto onNoMatchFound = []() { 
	//		printf ("%s\n", Search::Array::STRING_SEARCH_BYPFME_ERROR);
	//		exit (ExitCode::FAILURE_INVALID_ARGUMENT);
	//	};
	//
	//	uint16 resultIndex (0);
	//
	//	Search::Array::ByPFME <char, uint16> (
	//		onNoMatchFound, resultIndex,
	//		searchedFor.Length(), searchedFor.Pointer(),
	//		searchedData.Length(), (any*)searchedData.Pointer(),
	//		sizeof (MyString), sizeof (uint8) /* offset in type */
	//	);
	//
	//	printf ("Result: %i\n", resultIndex);
	//	resultIndex = 0;
	//
	//	Search::Array::ByPFME <char, uint16> (
	//		onNoMatchFound, resultIndex,
	//		searchedForA.length, searchedForA.name,
	//		searchedDataA.Length(), (any*)searchedDataA.Pointer()
	//		//sizeof (MyString), sizeof (uint8) /* offset in type */
	//	);
	//
	//	printf ("Result: %i\n", resultIndex);
	//}

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
			printf("NO_PARAMETERS_CALL");
			return ExitCode::FAILURE_TOO_LITTLE_ARGUMENTS;
		} 

		case ARGUMENTS_CHECK::INCOMPLETE_COMMAND: {
			// Match 2nd argument with commands.
			auto&& commandName = arguments[1];
			return Arguments::Parse::CommandIncomplete(commandName);
		}

		case VALID_COMMAND: {
			IO::GetProjectsData();
			return ExitCode::SUCCESSFULL_COMMAND_EXECUTION;
		}

		default: {
			printf("TOO_MANY_ARGUMENTS");
			return ExitCode::FAILURE_TOO_MANY_ARGUMENTS;
		}

	}
	
}