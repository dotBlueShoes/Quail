#include "lib/Framework.hpp"
#include "lib/Search.hpp"

void NoMatchError () {
	printf ("\n%s", "SEARCH::KnownLength. Fail no match!");
    exit (ExitCode::FAILURE_NO_COMMAND_FOR_SPECIFIED_PROJECT);
}

SUCCESS (return == ExitCode::SUCCESSFULL_COMMAND_EXECUTION)
int32 main (
	IN const int32 argumentsCount, 
	INREADS (argumentsCount) const char** arguments
) {

	DEBUG printf ("DEBUG Entered program in DEBUG mode!\n");

	//Search::ByIncludesFirstMatch();

	array<char, 5> searchedFor { 'a', 'b', 'c', 'd', 'e' };
	array<array<char, 5>, 6> searchedData {
		array<char, 5> { 'f', 'b', 'c', 'd', 'e' },
		array<char, 5> { 'b', 'b', 'c', 'd', 'e' },
		array<char, 5> { 'c', 'b', 'c', 'd', 'e' },
		array<char, 5> { 'g', 'b', 'c', 'd', 'e' },
		array<char, 5> { 'd', 'b', 'c', 'd', 'e' },
		array<char, 5> { 'e', 'b', 'c', 'd', 'e' },
	};

	char searchedDataOther[6][5] {
		{ 'b', 'b', 'c', 'd', 'e' },
		{ 'a', 'b', 'c', 'd', 'e' },
		{ 'c', 'b', 'c', 'd', 'e' },
		{ 'd', 'b', 'c', 'd', 'e' },
		{ 'a', 'b', 'c', 'd', 'e' },
		{ 'b', 'b', 'c', 'd', 'e' }
	};


	//const auto firstElement = (const char**)searchedData.data();
	uint8 result = 0;

	auto lambda = []() { printf("\n%s", "SEARCH::KnownLength. Fail no match!"); };

	Search::Array::ByPFM <char, uint8> (
		lambda,
		result,
		searchedFor.size(),
		searchedFor.data(),
		searchedData.size(),
		//(Any*)searchedDataOther
		(Any*)searchedData.data(),
		sizeof(array<char, 5>), 0
	);

	printf ("Result:%i\n", result);

	return ExitCode::SUCCESSFULL_COMMAND_EXECUTION;
}