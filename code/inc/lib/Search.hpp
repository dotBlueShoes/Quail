#include "Framework.hpp"

namespace Search::Array {

	const char* STRING_SEARCH_BYPFME_ERROR = "Array Search BYPFME No match failure!";

	/// Example "sample" in string "sample_foobar" returns true.
	/// Example "sample" in string "sample" returns true.
	/// By Part First Match (Equal Element Size in Array)
	///  (this will work with table[], but not with table* inside a struct...)

	template <typename T, typename IntegerType, typename NoMatchCallable>
	void ByPFME (
		IN								NoMatchCallable&& 	NoMatchCallableFunc,// Execute at no match.
		OUT								IntegerType& 		resultIndex,		// If found resultIndex points to found element if not stays unchainged.
		IN  							const IntegerType& 	comparableToCount,	// Length of array data we're compering to.
		INREADS (comparableToCount) 	const T* 			comparableTo,		// Data we're compering to.
		IN								const IntegerType& 	comparableFromCount,// Length of elements to compare from.
		INREADS (comparableFromCount) 	const any* 			comparableFrom,		// Array on which we're compering from.
		INOPT							const size& 		structSize = 1,		// For variable in possible Struct position in bytes. 
		INOPT							const size& 		structOffset = 0	// For variable in possible Struct position in bytes. 
	) {
		IntegerType collision = 1;

    	for (; resultIndex < comparableFromCount * collision; ++resultIndex) {
        	for (IntegerType i = 0; i < comparableToCount; ++i) {
        	    auto& element = ((T*)((byte*)(comparableFrom) + (structSize * resultIndex) + structOffset))[i];
				// Add up signs that match. Later compare that to comparable length to see if successfuly matched.
        	    collision += comparableTo[i] == element;
        	}
			// 1 - Signs are not equal, 0 - Signs are equal
        	collision = ((collision - 1) != comparableToCount);
    	}

    	if (collision) NoMatchCallableFunc();

    	--resultIndex;
	}

}

namespace Search::Buffor {

	const char* STRING_SEARCH_BYPFM_ERROR = "Buffor Search BYPFM No match failure!";

	// but
	//  We pass our pointer to our buffor,
	//  We pass table of {command_name} start positions
	// before call
	//  We need to ensure that there are elements in the buffor that wont cause overflow
	//  "After last {start_position} there has to be space for {looking_for_command_length}"

	template <typename T, typename IntegerType, typename NoMatchCallable>
	void ByPFM (
		IN							NoMatchCallable&& 	NoMatchCallableFunc,// Execute at no match.
		OUT							IntegerType& 		resultIndex,		// If found resultIndex points to found element if not stays unchainged.
		//IN							const byte&			elementSize,		// wchar vs char and such.
		IN  						const IntegerType& 	searchedCount,		// Length of array data we're compering to.
		INREADS (searchedCount) 	const T* const		searched,			// Data we're compering to.
		IN							const byte*	const	buffor,				// Buffor that holds all data.
		IN							const IntegerType&	indicatorsCount,	// Length of pointers - pointing at right buffor positions.
		INREADS	(indicatorsCount)	const byte*	const	indicators			// Pointers telling where to look for inside buffor.
	) {
		IntegerType collision = 1;

		// For each element pointed by indicator + When collision hit break.
		for (; resultIndex < indicatorsCount * collision; ++resultIndex) {
			// For each element in searched array.
			for (IntegerType i = 0; i < searchedCount; ++i) {
				auto&& origin = (T*)(buffor + indicators[resultIndex]);
				auto&& element = origin[i];
				collision += searched[i] == element;
			}
			//printf("%i\n", collision);
			collision = ((collision - 1) != searchedCount);
			//printf("%i\n", collision);
		}

		if (collision) NoMatchCallableFunc();

    	--resultIndex;
	}

}