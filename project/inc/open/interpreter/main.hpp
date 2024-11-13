// Created 2024.11.02 by Matthew Strumiłło (dotBlueShoes)
//
#pragma once
#include "base/types.hpp"
#include "base/error.hpp"
#include "../data.hpp"
#include "types.hpp"


namespace OPEN::INTERPRETER {

	struct Interpreter {
		c8 current;
		u32 special;
	};

	using Stage = void (*)( const Interpreter& );
	Stage parsingstage = nullptr;
	Stage specialStage = nullptr;

}

	// //putc (interpreter.current, stdout);

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
	void GetAllFiles	(const Interpreter&);
	void Main 			(const Interpreter&);
	void SpaceC8 		(const Interpreter&);
	void SpaceC16 		(const Interpreter&);
	void Comment 		(const Interpreter&);
}

namespace OPEN::INTERPRETER::MAIN::INCLUDE {
	void Initialize ();
	void Context	(const Interpreter&);
}

namespace OPEN::INTERPRETER::MAIN::PROJECT {
	void Initialize ();
	void Type		(const Interpreter&);
	void Name		(const Interpreter&);
	void Path		(const Interpreter&);
	void Config		(const Interpreter&);
}

namespace OPEN::INTERPRETER::MAIN {

	void GetAllFiles (const Interpreter& interpreter) {

		switch (interpreter.current) {

			case TYPE_COMMENT: parsingstage = Comment; break;
			case TYPE_INCLUDE: INCLUDE::Initialize (); break;
			case TYPE_PROJECT: PROJECT::Initialize (); break;
			default: { }

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

	void SpaceC8 (const Interpreter& interpreter) {

		switch (interpreter.current) {

			case TYPE_EOF:
			case TYPE_NEW_LINE:
			case TYPE_PROJECT:
			case TYPE_INCLUDE:
			case TYPE_COMMAND:
			case TYPE_QUEUE:
			case TYPE_CONSTANT:
			case TYPE_VARIABLE:
			case TYPE_SECRET:
			case TYPE_COMMENT:
			case TYPE_ASSIGN: {
				ERROR ("Invalid syntax\n\n");
			} break;

			case TYPE_CARRIAGE_RETURN:
			case TYPE_SPACE:
			case TYPE_TAB: break; // nothing

			default: {
				SetFirstTemp (interpreter.current);
				parsingstage = specialStage;
			}

		}

	}

	void SpaceC16 (const Interpreter& interpreter) {

		switch (interpreter.current) {

			case TYPE_EOF:
			case TYPE_NEW_LINE:
			case TYPE_PROJECT:
			case TYPE_INCLUDE:
			case TYPE_COMMAND:
			case TYPE_QUEUE:
			case TYPE_CONSTANT:
			case TYPE_VARIABLE:
			case TYPE_SECRET:
			case TYPE_COMMENT:
			case TYPE_ASSIGN: {
				ERROR ("Invalid syntax\n\n");
			} break;

			case TYPE_CARRIAGE_RETURN:
			case TYPE_SPACE:
			case TYPE_TAB: break; // nothing

			default: {
				SetFirstTempW (interpreter.current);
				parsingstage = specialStage;
			}

		}

	}

	void Comment (const Interpreter& interpreter) {
		switch (interpreter.current) {

			case TYPE_NEW_LINE:
			case TYPE_EOF: {
				parsingstage = GetAllFiles;
			} break;

			default: break; // nothing

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
			case TYPE_COMMENT:  {
				ERROR ("Invalid syntax\n\n");
			} break;

			case TYPE_SPACE:
			case TYPE_TAB: break; // nothing
			
			case TYPE_EOF:
			case TYPE_CARRIAGE_RETURN:
			case TYPE_NEW_LINE: {

				AddTempW (TYPE_EOS);

				u8* include; // Allocate & create FilePath string.
				MEMORY::Construct2<u8> (include, currentConfigFolderLength, currentConfigFolder, temporaryLength, temporary);

				{ // See if said FilePath already occurs. If not save it.
					for (u16 i = 0; i < includes.size(); ++i) {
						u8 condition = 0;
						IsEqualS3_16 (condition, (u16*)include, (u16*)includes[i]);
						if (condition == 2) goto error;
					}

		success:	includes.push_back (include);
					parsingstage = GetAllFiles;
					break;

		error:		ERROR ("Said file is arleady being included: `%ls`\n", (c16*)include);

				}

			} break;

			default: {
				AddTempW (interpreter.current);
			}

		}
	}

}

namespace OPEN::INTERPRETER::MAIN::PROJECT {

	void Initialize () {
		temporaryLength = 0;
		parsingstage = PROJECT::Type;
	}

	void Type (const Interpreter& interpreter) {

		switch (interpreter.current) {

			case TYPE_ABSOLUTE: {
				projects.types.push_back (PATH_ABSOLUTE);
				parsingstage = Name;
			} break;

			case TYPE_RELATIVE: {
				projects.types.push_back (PATH_RELATIVE);
				parsingstage = Name;
			} break;

			default: ERROR ("Invalid syntax\n\n");

		}

	}

	void Name (const Interpreter& interpreter) {

		switch (interpreter.current) {

			case TYPE_PROJECT:
			case TYPE_INCLUDE:
			case TYPE_COMMAND:
			case TYPE_QUEUE:
			case TYPE_CONSTANT:
			case TYPE_VARIABLE:
			case TYPE_SECRET:
			case TYPE_COMMENT:
			case TYPE_NEW_LINE:
			case TYPE_EOF: {
				ERROR ("Invalid syntax\n\n");
			} break;

			case TYPE_CARRIAGE_RETURN:
			case TYPE_SPACE:
			case TYPE_TAB: break; // nothing

			case TYPE_ASSIGN: {

				AddTemp (TYPE_EOS);

				// CPY. So it stays in memmory.
				u8* project; ALLOCATE (u8, project, temporaryLength);
				memcpy (project, temporary, temporaryLength);

				projects.keys.push_back (project);

				specialStage = Path;
				parsingstage = MAIN::SpaceC16;

			} break;

			default: {
				AddTemp (interpreter.current);
			}

		}

	}

	void Path (const Interpreter& interpreter) {

		switch (interpreter.current) {

			case TYPE_PROJECT:
			case TYPE_INCLUDE:
			case TYPE_COMMAND:
			case TYPE_QUEUE:
			case TYPE_CONSTANT:
			case TYPE_VARIABLE:
			case TYPE_SECRET:
			case TYPE_COMMENT:
			case TYPE_ASSIGN:
			case TYPE_EOF:
			case TYPE_NEW_LINE: {
				ERROR ("Invalid syntax\n\n");
			} break;

			case TYPE_CARRIAGE_RETURN:
			case TYPE_SPACE:
			case TYPE_TAB: {

				AddTempW (TYPE_EOS);
				u8* project; 

				// TODO:
				//  Find parent project if there is one and attach it's path to this one if its relative type

				if (projects.types.back() == PATH_RELATIVE) {

					// Get Parent Project
					const auto& upperProjectLength = projects.pathLengths[interpreter.special];
					const auto& upperProject = projects.paths[interpreter.special];
					//printf ("HERE: %ls\n", (c16*) upperProject);

					MEMORY::Construct2<u8> (project, upperProjectLength, upperProject, temporaryLength, temporary);
					projects.pathLengths.push_back (upperProjectLength + temporaryLength - 2); // minus EOS

				} else {

					{ // Allocate & create FilePath string.
						ALLOCATE (u8, project, temporaryLength);
						memcpy (project, temporary, temporaryLength);
					}

					projects.pathLengths.push_back (temporaryLength - 2); // minus EOS

				}
				
				projects.paths.push_back (project);
				
				specialStage = Config;
				parsingstage = MAIN::SpaceC16;

			} break;

			default: {
				AddTempW (interpreter.current);
			}

		}

	}

	void Config	(const Interpreter& interpreter) {

		switch (interpreter.current) {

			case TYPE_PROJECT:
			case TYPE_INCLUDE:
			case TYPE_COMMAND:
			case TYPE_QUEUE:
			case TYPE_CONSTANT:
			case TYPE_VARIABLE:
			case TYPE_SECRET:
			case TYPE_COMMENT:
			case TYPE_ASSIGN:
			case TYPE_SPACE:
			case TYPE_TAB: {
				ERROR ("Invalid syntax '%d'\n\n", interpreter.current);
			} break;

			
			case TYPE_EOF:
			case TYPE_CARRIAGE_RETURN:
			case TYPE_NEW_LINE: {

				AddTempW (TYPE_EOS);

				const auto& pathLength = projects.pathLengths.back();
				const auto& path = projects.paths.back();

				u8* project; // Allocate & create FilePath string.
				MEMORY::Construct2<u8> (project, pathLength, path, temporaryLength, temporary);

				{ // See if said FilePath already occurs. If not save it.
					for (u16 i = 0; i < projects.configs.size(); ++i) {
						u8 condition = 0;
						IsEqualS3_16 (condition, (u16*)project, (u16*)projects.configs[i]);
						if (condition == 2) goto error;
					}

		success:	projects.configLengths.push_back (temporaryLength - 2); // minus EOS
					projects.configs.push_back (project);
					parsingstage = GetAllFiles;
					break;

		error:		ERROR ("Said project file is arleady being loaded: `%ls`\n", (c16*)path);

				}

			} break;

			default: {
				AddTempW (interpreter.current);
			}

		}

	}

}
