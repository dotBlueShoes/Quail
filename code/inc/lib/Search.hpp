#include "Framework.hpp"

namespace Search {

	const char* STRING_SEARCH_ARRAY_BYPFM_ERROR = "Array Search BYPFM No match failure!";

	namespace Array {

		/// Example "sample" in string "sample_foobar" returns true.
		/// Example "sample" in string "sample" returns true.
		/// By Part First Match

		template <typename T, typename IntegerType, typename NoMatchCallable>
		void ByPFM (
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
            	    collision += comparableTo[i] == element;
            	}
            	collision = ((collision - 1) != comparableToCount);
        	}

        	if (collision) NoMatchCallableFunc();

        	--resultIndex;
		}

	}

	

}