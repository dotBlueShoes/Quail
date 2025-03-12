// Created 2025.03.12 by Matthew Strumiłło (dotBlueShoes)
//  LICENSE: GNU GENERAL PUBLIC LICENSE Version 3, 29 June 2007
//
#pragma once

//  About
// (SAL) Source-Code Annotation Language.
//
// Natively, C and C++ provide only limited ways for developers to 
//  consistently express intent and invariance. By using SAL 
//  annotations, you can describe your functions in greater detail 
//  so that developers who are consuming them can better understand 
//  how to use them.
// https://learn.microsoft.com/en-us/cpp/code-quality/understanding-sal?view=msvc-170
//

// TODO
// _Out_writes_() & _In_reads_() and other.

// HACK. for now.
#define OSWINDOWS

#ifdef OSWINDOWS

	#include <sal.h>

	// SAL - FUNCTION ARGUMENT IN 
	#define IN _In_
	// SAL - FUNCTION ARGUMENT OUT 
	#define OUT _Out_
	// SAL - FUNCTION ARGUMENT IN & OUT
	#define INOUT _Inout_

#else

	// SAL - FUNCTION ARGUMENT IN 
	#define IN
	// SAL - FUNCTION ARGUMENT OUT 
	#define OUT
	// SAL - FUNCTION ARGUMENT IN & OUT
	#define INOUT

#endif

// SAL - STATING THAT IT WAS TESTED THAT BRANCHLESS
// IMPLEMENTATION HERE IS IN FACT BETTER.
#define BRANCHLESS

// SAL - STATING THAT IT WAS TESTED THAT BRANCH
// IMPLEMENTATION HERE IS IN FACT BETTER.
#define BRANCH
