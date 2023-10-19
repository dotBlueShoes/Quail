#pragma once

#include <inttypes.h>

#define callback void
#define block __forceinline
#define getter [[nodiscard]] __forceinline

#ifdef DEBUG_LEVEL
#define DEF_DEBUG DEBUG_LEVEL > 0
#define DEF_RELEASE DEBUG_LEVEL == 0
#define DEBUG if constexpr ( DEF_DEBUG )
#define RELEASE if constexpr ( DEF_RELEASE )
#endif

// SAL
#define SUCCESS _Success_
#define INOPT _In_opt_
#define INOUT _Inout_
#define INREADS _In_reads_
#define OUTOPT _Out_opt_
#define OUTWRITES _Out_writes_ 

// Templates
//template <typename T>
//using BeInt = std::enable_if_t<std::is_integral_v<T>>;

using Any = const void*;

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