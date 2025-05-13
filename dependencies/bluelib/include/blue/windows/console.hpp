// Created 2024-12-11 by Matthew Strumiłło (dotBlueShoes)
//  LICENSE: GNU GENERAL PUBLIC LICENSE Version 3, 29 June 2007
//
#pragma once

#include <blue/types.hpp>
#include <blue/error.hpp>

#include <windows.h>

namespace WINDOWS {

	static const WORD MAX_CONSOLE_LINES = 500;

	void AttachConsole () {
		FILE* stream = nullptr;
		u32 errorCode = 0;

		errorCode = AllocConsole ();

		if (errorCode == 0) ERRORWIN ("Could not allocate a new console for window!");

		// Redirect to new console
		errorCode = freopen_s (&stream, "CONIN$" , "r", stdin ); // STDIN
		if (errorCode != 0) ERRORWIN ("Could not redirect 'stdin'");
		errorCode = freopen_s (&stream, "CONOUT$", "w", stdout); // STDOUT
		if (errorCode != 0) ERRORWIN ("Could not redirect 'stdout'");
		//errorCode = freopen_s (&stream, "CONERR$", "w", stderr); // STDERR
		//if (errorCode != 0) ERRORWIN ("Could not redirect 'stderr'");
	}

}
