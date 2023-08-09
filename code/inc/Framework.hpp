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

#include "BaseTypes.hpp"
#include "Array.hpp"
using namespace mst::array_n;


// TODO
//  - command contexts have to be 16bit !
//  - -o [name] help page that displays commands and their contexts
//  - command piping 