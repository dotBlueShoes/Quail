#pragma once
#include "lib/Framework.hpp"

namespace Commands::Open::Stages {

	struct StageParams {
        char current;
        char previous;
    };

	using StageProc = void;
	using Stage = StageProc (*)( const StageParams& );

	constexpr auto COMMENT 		= '#';
	constexpr auto NEW_LINE 	= '\n';
	constexpr auto SPACE 		= ' ';
	constexpr auto TAB 			= '\t';
	constexpr auto ASSIGN 		= '=';
	constexpr auto SECTION_SRT 	= '{';
	constexpr auto SECTION_END 	= '}';
	constexpr auto CONSTANT 	= '%';
	constexpr auto IMPORT 		= '!';
	constexpr auto QUEUE 		= '|';

}