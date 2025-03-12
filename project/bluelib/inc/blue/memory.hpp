// Created 2024.11.13 by Matthew Strumiłło (dotBlueShoes)
//  LICENSE: GNU GENERAL PUBLIC LICENSE Version 3, 29 June 2007
//
#pragma once
#include "debug.hpp"
//
#ifndef MEMORY_EXIT_SIZE
#define MEMORY_EXIT_SIZE 64
#endif

//  'About'
// - Implements a simple wrapper around 'malloc'
// - Implements a custom 'atexit' implementation that allows sending the parameter. 
//

#if DDEBUG (DEBUG_FLAG_MEMORY)

	s32 allocationsCounter = 0;

	#if DDEBUG (DEBUG_FLAG_LOGGING)

		#include "log.hpp"

		#define LOGMEMORY() { \
			LOGWARN ("Missed Deallocations: %d\n", allocationsCounter); \
		}

	#else
		#define LOGMEMORY() {} // dummy
	#endif

	#define INCALLOCCO() ++allocationsCounter;
	#define DECALLOCCO() --allocationsCounter;


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
	void FREE (void* address) { free (address); }
	#define LOGMEMORY() {} // dummy
	#define INCALLOCCO() // dummy
	#define DECALLOCCO() // dummy

#endif



namespace MEMORY {

	template <typename T>
	void Construct2 (
		OUT		T*& 				newArray, 
		IN		const u32& 			aArrayLength, 
		IN		const void* const& 	aArray, 
		IN		const u32& 			bArrayLength, 
		IN		const void* const& 	bArray
	) {
		ALLOCATE (T, newArray, aArrayLength + bArrayLength);
		memcpy (newArray, aArray, aArrayLength);
		memcpy (newArray + aArrayLength, bArray, bArrayLength);
	}

	template <typename T>
	void Construct3 (
		OUT		T*& 				newArray, 
		IN		const u32& 			aArrayLength, 
		IN		const void* const& 	aArray, 
		IN		const u32& 			bArrayLength, 
		IN		const void* const& 	bArray,
		IN		const u32& 			cArrayLength, 
		IN		const void* const& 	cArray
	) {
		ALLOCATE (T, newArray, aArrayLength + bArrayLength + cArrayLength);
		memcpy (newArray, aArray, aArrayLength);
		memcpy (newArray + aArrayLength, bArray, bArrayLength);
		memcpy (newArray + aArrayLength + bArrayLength, cArray, cArrayLength);
	}

}

namespace MEMORY::EXIT {

	using JUMPTABLE = void(*) (void*);

	u8 memoryCounter = 0;
	JUMPTABLE jumps[MEMORY_EXIT_SIZE];
	void* memory[MEMORY_EXIT_SIZE];

	void ATEXIT () {
		
		for (u8 i = memoryCounter; i != 0; --i) {
			auto jump = jumps[i - 1];
			auto address = memory[i - 1];
			jump (address);
		}

		//LOGINFO("%d \n", memoryCounter);
		//LOGINFO ("%lld, %lld\n", (u64)jump, (u64)address);
	}

	void PUSH (void* address, JUMPTABLE jumpTable) {
		jumps[memoryCounter] = jumpTable;
		memory[memoryCounter] = address;
		++memoryCounter;
	}

	void POP () {
		--memoryCounter;
	}

}
