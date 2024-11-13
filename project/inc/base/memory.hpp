// Created 2024.11.13 by Matthew Strumiłło (dotBlueShoes)
//
#pragma once
#include "debug.hpp"

#if DDEBUG (DEBUG_FLAG_MEMORY)

	s32 allocationsCounter = 0;

	#if DDEBUG (DEBUG_FLAG_LOGGING)

		#include "log.hpp"

		#define LOGMEMORY() { \
			LOGERROR ("Missed Dellocations: %d\n", allocationsCounter); \
		}

	#else
		#define LOGMEMORY() {} // dummy
	#endif

	template <typename Type>
	void _ALLOCATE (Type*& address, const u64& size) {
		++allocationsCounter;
		address = (Type*) malloc (size);
	}

	#define ALLOCATE(type, address, size) _ALLOCATE<type> (address, size)

	void FREE (void* address) {
		--allocationsCounter;
		free (address);
	}

#else

	#define ALLOCATE(type, address, size) address = (type*) malloc (size)
	#define FREE(address) free (address)
	#define LOGMEMORY() {} // dummy

#endif



namespace MEMORY {

	template <typename T>
	void Construct2 (
		T*& newArray, 
		const u32& aArrayLength, 
		const void* const& aArray, 
		const u32& bArrayLength, 
		const void* const& bArray
	) {
		ALLOCATE (T, newArray, aArrayLength + bArrayLength);
		memcpy (newArray, aArray, aArrayLength);
		memcpy (newArray + aArrayLength, bArray, bArrayLength);
	}

	template <typename T>
	void Construct3 (
		T*& newArray, 
		const u32& aArrayLength, 
		const void* const& aArray, 
		const u32& bArrayLength, 
		const void* const& bArray,
		const u32& cArrayLength, 
		const void* const& cArray
	) {
		ALLOCATE (T, newArray, aArrayLength + bArrayLength + cArrayLength);
		memcpy (newArray, aArray, aArrayLength);
		memcpy (newArray + aArrayLength, bArray, bArrayLength);
		memcpy (newArray + aArrayLength + bArrayLength, cArray, cArrayLength);
	}

}
