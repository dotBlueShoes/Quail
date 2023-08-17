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
#include <vector>

namespace filesystem = std::filesystem;

using std::ifstream;
using std::ofstream;
using std::vector;
using std::string;

#include "lib/BaseTypes.hpp"
#include "lib/Array.hpp"
#include "lib/Search.hpp"
using namespace mst::array_n;

// RETURN VALUES
enum ExitCode : uint8 {
    SUCCESSFULL_COMMAND_EXECUTION = 0,
    FAILURE_MAIN_VALIDATION = 0,
    FAILURE_ARGUMENT_COUNT = 0,
    FAILURE_NO_CONFIG_FILE = 1,
    /* COMMAND: OPEN */
    FAILURE_TO_LONG_COMMAND_NAME = 1,
    FAILURE_PROJECT_NAME_NOT_SPECIFIED = 1,
    FAILURE_NO_COMMAND_FOR_SPECIFIED_PROJECT = 1,
    FAILURE_UNKNOWN_COMMAND_TYPE = 1,
};


// TODO
// - variable command type eg. `$project_path: "path"`
// - pipe type form '$' to '|'

// 1. re-create tools.npp file so it will contain all the files there are in those folders.
// 2. setup script that will
//  - ask the user for:
//    - project name
//    - build name
//    - libs
//  - will setup project path

// Error checking
// output
// If in file defined variables would be always in the same order 
//  the parse function could work as more then a 1 function that would be 
//  changed constanly in a loop for functions to look for other tokens. 
//  Also yeah that can be easier implemented... 