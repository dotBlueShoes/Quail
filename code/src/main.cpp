#include "lib/Framework.hpp"
#include "lib/Search.hpp"

using MyString = array<char, 5>;

MyString searchedFor { 'a', 'b', 'c', 'd', 'e' };
array<MyString, 6> searchedData {
	MyString { 'f', 'b', 'c', 'd', 'e' },
	MyString { 'b', 'b', 'c', 'd', 'e' },
	MyString { 'a', 'b', 'c', 'd', 'e' },
	MyString { 'g', 'b', 'c', 'd', 'e' },
	MyString { 'd', 'b', 'c', 'd', 'e' },
	MyString { 'e', 'b', 'c', 'd', 'e' },
};

SUCCESS (return == ExitCode::SUCCESSFULL_COMMAND_EXECUTION)
int32 main (
	IN const int32 argumentsCount, 
	INREADS (argumentsCount) const char** arguments
) {

	DEBUG printf ("%s\n", strings::STRING_DEBUG_MODE_INO);

	{ // Search Example
		auto onNoMatchFound = []() { 
			printf ("%s\n", Search::STRING_SEARCH_ARRAY_BYPFM_ERROR);
			exit(ExitCode::FAILURE_UNKNOWN_COMMAND_TYPE);
		};

		uint16 result (0);

		Search::Array::ByPFM <char, uint16> (
			onNoMatchFound, result,
			searchedFor.size(), searchedFor.data(),
			searchedData.size(), (Any*)searchedData.data(),
			sizeof(MyString)
		);

		printf ("Result: %i\n", result);
	}

	return ExitCode::SUCCESSFULL_COMMAND_EXECUTION;
}