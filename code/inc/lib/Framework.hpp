#pragma once


// To get rid of CRT e.g. ()_s functions being superior..
#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif


#include <filesystem>
#include <windows.h>
#include <iostream>
#include <fstream>
#include <string>
//#include <vector>
//#include <array>
//#include <utility> // std::pair




namespace filesystem = std::filesystem;
using std::ifstream;
using std::ofstream;
using std::vector;
using std::string;
//using std::array;
//using std::pair;


#include "BaseTypes.hpp"
#include "Pointer.hpp"
#include "Array.hpp"
#include "Pair.hpp"


using mst::buffor;
using mst::array;
using mst::pair;
using mst::ptr;


namespace strings {

	constexpr const char* STRING_DEBUG_MODE_INFO = "Entered program in DEBUG mode.";

	constexpr const char* STRING_FAILURE_NO_CONFIG_FILE = "FAILURE no config file!";
	constexpr const char* STRING_FAILURE_BUFFOR_TO_SMALL = "FAILURE buffor to small!";
	//#define STRING_DEBUG_MODE_INFO "Entered program in DEBUG mode."
	//#define STRING_FAILURE_NO_CONFIG_FILE "FAILURE no config file!"
	//#define STRING_FAILURE_BUFFOR_TO_SMALL "FAILURE buffor to small!"
	
}


enum ExitCode : int32 {

	// SUCCESS CODES
	SUCCESSFULL_COMMAND_EXECUTION				= 0,

	// WEIRD CODES
	FAILURE_UNKNOWN								= 1,

	// ARGUMENTS
	FAILURE_TOO_LITTLE_ARGUMENTS				= 1,
	FAILURE_TOO_MANY_ARGUMENTS					= 1,
	FAILURE_ARGUMENT_COUNT						= 1,
	FAILURE_INVALID_ARGUMENT					= 1,

	// I/O
	FAILURE_NO_CONFIG_FILE						= 1,

	/* COMMAND: OPEN */
	//FAILURE_TO_LONG_COMMAND_NAME				= 1,
	//FAILURE_PROJECT_NAME_NOT_SPECIFIED			= 1,
	//FAILURE_NO_COMMAND_FOR_SPECIFIED_PROJECT	= 1,
	//FAILURE_UNKNOWN_COMMAND_TYPE				= 1,
	//FAILURE_BUFFOR_TO_SMALL						= 1,
};