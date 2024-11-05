// Created 2024.11.02 by Matthew Strumiłło (dotBlueShoes)
//
#pragma once
#include "base/types.hpp"
#include "../data.hpp"
#include "types.hpp"

namespace OPEN::INTERPRETER {

	struct Interpreter {
		c8 current;
		c8 last;
	};

	using Stage = void (*)( const Interpreter& );
	Stage parsingstage = nullptr;

}

	// BUFFORS
	//  1. Rules
	//   CONSTANT (for every machine)
	//   - Constants can be encapsulated in constants
	//   - Constants can be encapsulated in variables
	//   - Constants can be encapsulated in secrets
	//   - Constants can be encaspulated in commands
	//	 // NOPE // - Constants can be encapsulated in includes
	//	 // NOPE // - Constants can be encapsulated in projects
	//   SECRET (for single machine)
	//   - Secrets can be encapsulated in constants
	//   - Secrets can be encapsulated in variables
	//   - Secrets can be encapsulated in secrets
	//   - Secrets can be encapsulated in commands
	//   // NOPE // - Secrets can be encapsulated in includes
	//	 // NOPE // - Secrets can be encapsulated in projects
	//   VARIABLE (can be changed via command)
	//   - Variables can be encapsulated in variables
	//   - Variables can be encapsulated in commands
	//   // NOPE // - Variables can be encapsulated in includes
	//	 // NOPE // - Variables can be encapsulated in projects
	//   PROJECT (modules - subprojects)
	//   - Project remembers project name. so we can use sufix command like `quail -o project subproject command`
	//   INCLUDE (hold project data in more then 1 file)
	//   - Includes are being stored so that we fight with recurency if possible.
	//   - Includes add values from within different quail config files.
	//   COMMAND (run)
	//   - Lunches a specifc command.
	//   MULTI_COMMAND (run multiple with single input)
	//   - Lunches one or more commands.


namespace OPEN::INTERPRETER::MAIN {
	void GetAllFiles (const Interpreter& interpreter);
	void Main (const Interpreter& interpreter);
}

namespace OPEN::INTERPRETER::MAIN::INCLUDE {
	void Initialize ();
	void Context	(const Interpreter&);
}

namespace OPEN::INTERPRETER::MAIN::PROJECT {
	void Name		(const Interpreter&);
	void Assign		(const Interpreter&);
	void Context	(const Interpreter&);
}

namespace OPEN::INTERPRETER::MAIN {

	void GetAllFiles (const Interpreter& interpreter) {

		switch (interpreter.current) {

			case TYPE_INCLUDE: INCLUDE::Initialize (); break;

			case TYPE_PROJECT: {

			} break;

			default: {

			}
			
		}

	}

	void Main (const Interpreter& interpreter) {

		//putc (interpreter.current, stdout);

		switch (interpreter.current) {

			case TYPE_NEW_LINE:
			case TYPE_SPACE:
			case TYPE_TAB:
			case TYPE_EOF: {
				putc (interpreter.current, stdout);
			} break;

			case TYPE_COMMENT: {

			} break;

			case TYPE_CONSTANT: {

			} break;

			case TYPE_VARIABLE: {

			} break;

			case TYPE_QUEUE: {

			} break;

			case TYPE_COMMAND: {

			} break;

			default: { 
				putc (interpreter.current, stdout);
			}

		}

	}

}

namespace OPEN::INTERPRETER::MAIN::INCLUDE {

	void Initialize () {
		temporaryLength = 0;
		parsingstage = INCLUDE::Context;
	}

	void Context (const Interpreter& interpreter) {

		switch (interpreter.current) {

			case TYPE_PROJECT:
			case TYPE_INCLUDE:
			case TYPE_COMMAND:
			case TYPE_QUEUE:
			case TYPE_CONSTANT:
			case TYPE_VARIABLE:
			case TYPE_SECRET:
			case TYPE_ASSIGN:
			case TYPE_COMMENT:
			case TYPE_EOF: {
				ERROR ("\n\nERROR: Invalid syntax\n\n");
			} break;

			case TYPE_CARRIAGE_RETURN:
			case TYPE_SPACE:
			case TYPE_TAB: {
				// nothing
			} break;

			
			case TYPE_NEW_LINE: {

				AddTempW (TYPE_EOS);

				const u32 includeLength = temporaryLength + mainConfigFolderPathLength;
				u8* include;

				{ // Allocate & create FilePath string.
					include = (u8*) malloc (includeLength);
					memcpy (include, mainConfigFilePath, mainConfigFolderPathLength);
					memcpy (include + mainConfigFolderPathLength, temporary, temporaryLength);
				}

				{ // See if said FilePath already occurs. If not save it.
					for (u16 i = 0; i < includes.size(); ++i) {
						u8 condition = 0;
						IsEqualS3_16 (condition, (u16*)include, (u16*)includes[i]);
						if (condition == 2) goto error;
					}

		success:	includes.push_back (include);
					parsingstage = GetAllFiles;
					break;

		error:		ERROR ("\n\nERROR: Said file is arleady being included: `%ls`\n", (c16*)include);

				}

			} break;

			default: {
				AddTempW (interpreter.current);
				//putc (interpreter.current, stdout);
			}

		}
	}

}
