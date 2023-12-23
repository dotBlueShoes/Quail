#include "lib/Framework.hpp"
#include "lib/Search.hpp"

#include "Buffors.hpp"
#include "IO/Projects.hpp"


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


int32 main (
	IN 							const int32 argumentsCount, 
	INREADS (argumentsCount) 	const char** arguments
) {

	DEBUG printf ("%s\n", strings::STRING_DEBUG_MODE_INFO);

	// Initialize Buffers with program start. They'll automaticlly deallocate.
	//memoryBlockA.INITIALIZE(4096);
	memoryBlockA.INITIALIZE(1024);
	memoryBlockB.INITIALIZE(512);
	
	IO::GetProjectsData();
	

	{ // By Part Search Example
		auto onNoMatchFound = []() { 
			printf ("%s\n", Search::STRING_SEARCH_ARRAY_BYPFM_ERROR);
			exit (ExitCode::FAILURE_UNKNOWN_COMMAND_TYPE);
		};

		uint16 resultIndex (0);

		Search::Array::ByPFM <char, uint16> (
			onNoMatchFound, resultIndex,
			searchedFor.Length(), searchedFor.Pointer(),
			searchedData.Length(), (any*)searchedData.Pointer(),
			sizeof (MyString), sizeof (size) /* offset in type */
		);

		printf ("Result: %i\n", resultIndex);
	}

	return ExitCode::SUCCESSFULL_COMMAND_EXECUTION;
}