#include "Framework.hpp"

namespace Search {

	const char* STRING_SEARCH_ARRAY_BYPFM_ERROR = "Array Search BYPFM No match failure!";

	namespace Array {

		/// Example "sample" in string "sample_foobar" returns true.
		/// Example "sample" in string "sample" returns true.
		/// By Part First Match

		template <typename T, typename IntegerType, typename NoMatchCallable>
		void ByPFM (
			IN		NoMatchCallable&& noMatchCallable,
			OUT		IntegerType& resultIndex,
			IN  	const IntegerType& comparableToCount,
			IN		const T* comparableTo,
			IN		const IntegerType& comparableFromCount,
			IN		const Any* comparableFrom,
			INOPT	const size& structSize		= 1,
			INOPT	const size& structOffset	= 0
		) {
			IntegerType collision = 1;

        	for (; resultIndex < comparableFromCount * collision; ++resultIndex) {
            	for (IntegerType i = 0; i < comparableToCount; ++i) {
            	    auto& element = ((T*)((byte*)(comparableFrom) + (structSize * resultIndex) + structOffset))[i];
            	    collision += comparableTo[i] == element;
            	}
            	collision = ((collision - 1) != comparableToCount);
        	}

        	if (collision) noMatchCallable();

        	--resultIndex;
		}

		/// Example "sample" in string "sample_foobar" returns true.
		/// Example "sample" in string "sample" returns true.
		/// By Full First Match

		uint32 ByPFM () {
			return 5;
		}

	}

	

}