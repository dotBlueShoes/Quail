#pragma once

#include "BaseTypes.hpp"

namespace mst {

	template <class T>
	class buffor {
	public:
		size length = 0;
		T* data = nullptr;

		/// CONSTRUCTORS
		
		constexpr buffor () {} // Empty Constructor.

		constexpr buffor (const size& newLength) {
			data = (T*)malloc(newLength * sizeof(T));
			length = newLength;
		}

		constexpr buffor (const size& newLength, const bool& dummy) {
			data = (T*)calloc(newLength, sizeof(T));
			length = newLength;
		}

		/// LATE CONSTRUCTORS

		block INITIALIZE (const size& newLength) {
			data = (T*)malloc(newLength * sizeof(T));
			length = newLength;
		}

		block INITIALIZE_ZERO (const size& newLength) {
			data = (T*)calloc(newLength, sizeof(T));
			length = newLength;
		}

		/// DESTRUCTORS

		~buffor () {
			free(data);
		}

		/// Iteration

		getter const T* begin () const noexcept { return &data[0]; }
		getter const T* end () const noexcept { return &data[length]; }

		/// Getters

		//getter const size& Length () const noexcept { return length; }
		//getter const T* const Pointer () const noexcept { return &data[0]; }
		//getter T* const Pointer () noexcept { return &data[0]; } // to making old werid code work.

	};

}