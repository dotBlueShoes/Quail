#pragma once
#include "lib/Framework.hpp"

namespace IO::Projects::Stages {

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
	constexpr auto SECTION_SRT 	= '{';
	constexpr auto SECTION_END 	= '}';
	constexpr auto ASSIGN 		= '=';

}