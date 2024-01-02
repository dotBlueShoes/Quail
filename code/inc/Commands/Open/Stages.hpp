#pragma once
#include "lib/Framework.hpp"

#include "Types.hpp"

namespace Commands::Open::Stages {

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

	void Initialize();
	void FileReset();

	// Function Stages

	StageProc MainBeginStage(const StageParams& stage);
	StageProc BeginStage(const StageParams& stage);
	StageProc Comment(const StageParams& stage);

	// Will be moved to Commands::Open::Stages::... namespaces
	StageProc Constant(const StageParams& stage);
	StageProc Queue(const StageParams& stage);
	

	extern Stage Current; // = MainBeginStage / BeginStage // Main for main file as it holds links to other files.

}

namespace Commands::Open::Stages::File {

	StageProc Begin(const StageParams& stage);
	StageProc Assign(const StageParams& stage);
	StageProc Context(const StageParams& stage);

}


namespace Commands::Open::Stages::Command {

	StageProc Begin(const StageParams& stage);

}