#pragma once
#include "BaseTypes.hpp"

#include <utility>
#include <type_traits>
using std::make_index_sequence;
using std::index_sequence;
using std::conjunction_v;
using std::is_same;

namespace mst::array_n {

	template <class T, const size length>
	class array {

		const size nonTemporaryLength { length };
		T value[length];

		// Forwarded Constructor. used by "Initialization list Constructor via Forwarding Constructor" constructor.
		template <size ... index>
		constexpr array(const T* const pointer, index_sequence<index...>) : value { pointer[index]... } {}

	public:

		// Constructors

		// Empty Constructor. Value is not being initialized. ( e.g. array (); )
		constexpr array() {}

		// Single value Constructor. ( e.g. array ( 'A' ); )
		constexpr array(const T& singleValue) : value { singleValue } {}

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

		getter const size& Length() const noexcept { return nonTemporaryLength; }
		getter const T* const Pointer() const noexcept { return &value[0]; }
		getter T* const Pointer() noexcept { return &value[0]; } // to making old werid code work.

		// Dictionary
		// - The second const after the function name is for saying that this certain function doesn't change anything by itself.
		// - variadic template
		// - parameter pack
		// - string literal
		// - forwarding arguments
		// - 

	};
}