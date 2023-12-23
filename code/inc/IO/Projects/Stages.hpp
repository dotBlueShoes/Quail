#pragma once
#include "lib/Framework.hpp"

namespace IO::Projects::Stages {

	// Think about other files !
	// quail files !
	// template files !

	// First byte is the number of mainCommands
	// Second byte is the offset to get to the second command
	// Third byte is number of variables
	// Forth byte is the size of variable string
	// ... data
	// Next is byte for number of subcommands
	// Next is byte for the size of subcommand string
	// ... data
	// Next is byte for number of pipes
	// Next is byte for the size of pipe int array
	// ... data (subcommand 1 & 2 & 4)
	// Next byte is the offset to get to the third command
	// ... second command ...

	// Function Type Declaration

	// Function Stages
	void Current() {} // = MainBeginStage / BeginStage // Main for main file as it holds links to other files.

}