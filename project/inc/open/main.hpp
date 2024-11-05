// Created 2024.11.05 by Matthew Strumiłło (dotBlueShoes)
//
#pragma once
#include "base/types.hpp"
#include "base/io.hpp"

#include "interpreter/main.hpp"
#include "data.hpp"

namespace OPEN {

	void Open () {

		FILE* mainConfig = nullptr;
		IO::Read (mainConfigFilePath, mainConfig);

		{
			INTERPRETER::Interpreter interpreter { 0 };
			
			INTERPRETER::parsingstage = INTERPRETER::MAIN::GetAllFiles;
			//INTERPRETER::parsingstage = INTERPRETER::MAIN::Start;

			while (interpreter.current != EOF) { // READ
				interpreter.last = interpreter.current;
				interpreter.current = getc (mainConfig);
				INTERPRETER::parsingstage (interpreter);
			}

			printf ("INFO: File read successfully\n");

			for (u32 i = 0; i < includes.size(); ++i) {
				auto&& string = (c16*)(includes[i]);
				wprintf (L"INFO: include [%d]: %s\n", i, string);
			}
		}

		IO::Close (mainConfig);
		free (mainConfigFilePath);
	}

}
