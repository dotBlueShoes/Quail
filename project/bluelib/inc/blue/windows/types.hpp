// Created 2024.12.29 by Matthew Strumiłło (dotBlueShoes)
//
#pragma once
#include <Windowsx.h>
#include <Windows.h>

#ifdef ERROR
	#undef ERROR
#endif

#define GetProcMemory(procedureType, module, procedureName) { \
	procedureName = (procedureType) GetProcAddress (GetModuleHandleA (#module), #procedureName); \
	if (procedureName == nullptr) ERROR ("Function not created. Failed to load `%s` method: `%s`\n", #module, #procedureName); \
}