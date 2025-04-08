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

	// SAL - Function argument IN
	#define IN _In_
	// SAL - Function argument OUT 
	#define OUT _Out_
	// SAL - Function argument IN & OUT
	#define INOUT _Inout_

#else

	// SAL - Function argument IN 
	#define IN
	// SAL - Function argument OUT 
	#define OUT
	// SAL - Function argument IN & OUT
	#define INOUT

#endif

// SAL - Stating that it was tested that branchless
// implementation here is in fact better.
#define BRANCHLESS

// SAL - Stating that it was tested that branch
// implementation here is in fact better.
#define BRANCH
