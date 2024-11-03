// Created 2024.11.02 by Matthew Strumiłło (dotBlueShoes)
//
#pragma once
#include "base/types.hpp"

namespace INTERPRETER::OPENMAIN {

	struct Interpreter {
		c8 current;
		c8 last;
	};

	enum SPECIAL_CHARACTERS : c8 {
		COMMAND_TYPE_INCLUDE 		= '&',
		COMMAND_TYPE_COMMAND 		= '!',
		COMMAND_TYPE_MULTI_COMMAND 	= '|',
		COMMAND_TYPE_CONSTANT 		= '@',
		COMMAND_TYPE_VARIABLE 		= '%',
		COMMAND_TYPE_SECRET 		= '?',
	};

	using Stage = void (*)( const Interpreter& );
	Stage parsingstage = nullptr;

	void Start ( const Interpreter& interpreter) {
		//putc (interpreter.current, std::);
	}

}
