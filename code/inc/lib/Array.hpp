#pragma once

#include "BaseTypes.hpp"

#include <type_traits>
#include <utility>

namespace mst {

	using std::make_index_sequence;
	using std::index_sequence;
	using std::conjunction_v;
	using std::is_same;

	template <class T, const size length, typename IntegerType = size>
	class array {

		IntegerType nonTemporaryLength { length };
		T value[length];

		// Forwarded Constructor. used by "Initialization list Constructor via Forwarding Constructor" constructor.
		template <size ... index>
		constexpr array(const T* const pointer, index_sequence<index...>) : value { pointer[index]... } {}

	public:

		/// Constructors

		// Empty Constructor. Value is not being initialized. ( e.g. array (); )
		constexpr array() {}

		// Single value Constructor. ( e.g. array ( 'A' ); )
		// constexpr array(const T& singleValue) : value { singleValue } {}

		// (const T (&array)[N])

		// Initialization list Constructor via Forwarding Constructor. ( e.g. array ( "Sample" ); )
		constexpr array(const T* const pointer) : array(pointer, make_index_sequence<length>()) {}

		// Parameter pack Constructor. ( e.g. array { 'A', 'B' }; )
		template <class TypeFirst, class... TypesRest> // Because TypesRest can be expressed as 0 params.
		constexpr array(const TypeFirst& newFirstValue, const TypeFirst& newSecondValue, const TypesRest&... newRestValue) 
			: value { newFirstValue, newSecondValue, newRestValue... } {}

		/// [] Operator

		getter const T& operator[](size index) const noexcept { return value[index]; }
		getter T& operator[](size index) noexcept { return value[index]; }

		/// Iteration

		getter const T* begin() const noexcept { return &value[0]; }
		getter const T* end() const noexcept { return &value[nonTemporaryLength]; }

		/// Getters

		getter const IntegerType& Length() const noexcept { return nonTemporaryLength; }
		getter const T* const Pointer() const noexcept { return &value[0]; }
		getter T* const Pointer() noexcept { return &value[0]; } // to making old werid code work.
	};
}