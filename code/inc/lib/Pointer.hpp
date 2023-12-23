#pragma once

#include "Buffor.hpp"

namespace mst {

	template <class T>
	class ptr {
	public:
		T* data = nullptr; // std::nullptr_t

		/// CONSTRUCTORS
		template <class BufforT>
		ptr (const buffor<BufforT>& newData, size offset = 0) {
			data = newData.data + offset;
		}

		/// [] Operator

		getter const T& operator[] (size index) const noexcept { return data[index]; }
		getter T& operator[] (size index) noexcept { return data[index]; }
	};

}


