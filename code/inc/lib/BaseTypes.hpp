#pragma once

#include <inttypes.h>

/* Function types */
#define callback void
#define block __forceinline void
#define getter [[nodiscard]] __forceinline

/* Building */
#ifdef DEBUG_LEVEL
#define DEF_DEBUG DEBUG_LEVEL > 0
#define DEF_RELEASE DEBUG_LEVEL == 0
#define DEBUG if constexpr ( DEF_DEBUG )
#define RELEASE if constexpr ( DEF_RELEASE )
#endif

/* SAL annotations */
#ifndef SUCCESS
#define SUCCESS _Success_		// You can say what success for a function looks like.
#endif
#ifndef IN
#define IN _In_					// Input parameter.
#endif
#ifndef IN
#define OUT _Out_				// Output parameter.
#endif
#ifndef INOUT
#define INOUT _Inout_			// Input and output parameter.
#endif
#ifndef INREADS
#define INREADS _In_reads_		// Mark the size of a buffor we're reading from.
#endif
#ifndef OUTWRITES
#define OUTWRITES _Out_writes_	// Mark the size of a buffor we're writting to.
#endif
#ifndef INOPT
#define INOPT _In_opt_			// Optional parameter IN
#endif
#ifndef OUTOPT
#define OUTOPT _Out_opt_		// Optional parameter OUT
#endif

/* Base types */
using any = void*;

using size = size_t;
using wchar = wchar_t;

using uint8 = uint8_t;
using uint16 = uint16_t;
using uint32 = uint32_t;
using uint64 = uint64_t;

using int8 = int8_t;
using int16 = int16_t;
using int32 = int32_t;
using int64 = int64_t;

using flag8 = uint8_t;
using flag16 = uint16_t;
using flag32 = uint32_t;
using flag64 = uint64_t;