// Created 2024.11.02 by Matthew Strumiłło (dotBlueShoes)
//  LICENSE: GNU GENERAL PUBLIC LICENSE Version 3, 29 June 2007
//
#pragma once
#include "blue/types.hpp"
#include "blue/error.hpp"
#include "blue/comparesearch.hpp"

#include "../data.hpp"

namespace OPEN::INTERPRETER {

	struct Interpreter {
		c8 current;
		u32 special;
	};

	using Stage = void (*)( const Interpreter& );
	Stage parsingstage = nullptr;
	Stage specialStage = nullptr;


	#define ERROR_INTERPRETER(typeStr, character) { \
		ERROR ("Invalid syntax '%s' ['%c'-%d]" ERROR_NEW_LINE, typeStr, character, character); \
	}

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
	void Comment 		(const Interpreter&);
}

namespace OPEN::INTERPRETER::MAIN::INCLUDE {
	void Initialize ();
	void Context	(const Interpreter&);
}

namespace OPEN::INTERPRETER::MAIN::PROJECT {
	void Initialize ();
	void Type		(const Interpreter&);
	void SpaceC16 	(const Interpreter&);
	void Name		(const Interpreter&);
	void Path		(const Interpreter&);
	void Config		(const Interpreter&);
}

namespace OPEN::INTERPRETER::MAIN::CONSTANT {
	void Initialize ();
	void Name		(const Interpreter&);
	void Value		(const Interpreter&);
}

namespace OPEN::INTERPRETER::MAIN::CASCADE {
	void Initialize ();
	void SpaceC16 	(const Interpreter&);
	void Constant 	(const Interpreter&);
}

namespace OPEN::INTERPRETER::MAIN::COMMAND {
	void Initialize ();
	void SpaceC16 	(const Interpreter&);
	void Name		(const Interpreter&);
	void Value		(const Interpreter&);
}

namespace OPEN::INTERPRETER::MAIN::QUEUE {
	void Initialize ();
	void SpaceC16 	(const Interpreter&);
	void Name		(const Interpreter&);
	void Value		(const Interpreter&);
}

namespace OPEN::INTERPRETER::MAIN {

	void GetAllFiles (const Interpreter& interpreter) {

		// We're basically cheking only 1st character after each new line
		//  If its something we want we read the line the way we want otherwise
		//  we're treating it as whitespace or comments.

		switch (interpreter.current) {

			case TYPE_INCLUDE: INCLUDE::Initialize (); break;
			case TYPE_PROJECT: PROJECT::Initialize (); break;

			case TYPE_CARRIAGE_RETURN:
			case TYPE_NEW_LINE:
			case TYPE_SPACE:
			case TYPE_TAB: 
			case TYPE_EOF: break;

			// If the line does not start with an indicator treat it like a TYPE_COMENT line.
			default: parsingstage = Comment; specialStage = GetAllFiles;

		}

	}

	void Main (const Interpreter& interpreter) {

		switch (interpreter.current) {

			case TYPE_CARRIAGE_RETURN:
			case TYPE_NEW_LINE:
			case TYPE_SPACE:
			case TYPE_TAB:
			case TYPE_EOF: break;

			case TYPE_CONSTANT: CONSTANT::Initialize (); break;
			case TYPE_SECRET: LOGINFO ("Detected Secret!\n"); break;
			case TYPE_VARIABLE: LOGINFO ("Detected Variable!\n"); break;
			case TYPE_COMMAND: COMMAND::Initialize (); break;
			case TYPE_QUEUE: QUEUE::Initialize (); break;

			// We're also skipping TYPE_INCLUDE, TYPE_PROJECT by doing so.
			// If the line does not start with an indicator treat it like a TYPE_COMENT line.
			default: parsingstage = Comment; specialStage = Main;

		}

	}

	void Comment (const Interpreter& interpreter) {
		switch (interpreter.current) {

			case TYPE_NEW_LINE:
			case TYPE_EOF: {
				parsingstage = specialStage;
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
				ERROR_INTERPRETER ("include:context", interpreter.current);
			} break;

			case TYPE_CARRIAGE_RETURN:
			case TYPE_SPACE:
			case TYPE_TAB: break; // nothing
			
			case TYPE_EOF:
			case TYPE_NEW_LINE: {

				AddTempW (TYPE_EOS);

				//LOGINFO ("1 Temporary include: %ls, %lld\n", (c16*)temporary, includes.size())

				u8* include; // Allocate & create FilePath string.
				MEMORY::Construct2<u8> (include, currentConfigFolderLength, currentConfigFolder, temporaryLength, temporary);

				//LOGINFO ("2 Temporary include: %ls, %lld\n", (c16*)include, includes.size())

				// TODO: 
				// Previously this code was cheching whether a said file was arleady included and if so returned an error.
				//  However we've created an abstract structure that holds includes to a specific project therefore
				//  the error was abandoned. Because of that an include file if specified more then once will be read more then once
				//  that is an issue that should be solved in the future. For now no matter what we need to save that inlucde
				//  so that sub projects that are using said include file just as their parent project did can work correctly.

				{ 
					for (u16 i = 0; i < includes.size(); ++i) {
						u8 condition = 0;
						IsEqualS3_16 (condition, (u16*)include, (u16*)includes[i]);
						if (condition == 2) {
							LOGWARN ("Said file is arleady being included: `%ls`\n", (c16*)include);
							goto success; //goto error;
						}
					}

		success:	includes.push_back (include);
					parsingstage = GetAllFiles;
					//
					//exit (1);
					//
					break;

		//error:		ERROR ("Said file is arleady being included: `%ls`\n", (c16*)include);

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

		// ADDs new CAPE with each project found.
		projects.capes.push_back ( OPEN::ProjectsCape { 0 } );
	}

	void Type (const Interpreter& interpreter) {

		switch (interpreter.current) {

			case TYPE_ABSOLUTE: {
				auto&& current = projects.capes.back();
				current.special.type = PATH_ABSOLUTE;
				parsingstage = Name;
			} break;

			case TYPE_RELATIVE: {
				auto&& current = projects.capes.back();
				current.special.type = PATH_RELATIVE;
				parsingstage = Name;
			} break;

			default: ERROR_INTERPRETER ("project:type", interpreter.current);

		}

	}

	void SpaceC16 (const Interpreter& interpreter) {

		switch (interpreter.current) {

			case TYPE_EOF:
			case TYPE_NEW_LINE:
			case TYPE_ASSIGN:
			case TYPE_CONSTANT:
			case TYPE_VARIABLE:
			case TYPE_SECRET: {
				ERROR_INTERPRETER ("project:spacedot16", interpreter.current);
			} break;

			case TYPE_CARRIAGE_RETURN:
			case TYPE_SPACE:
			case TYPE_TAB: break; // nothing

			case '/':		// Skip '/' or '\' sign at the beggining.
			case '\\': { 	//  this also helps with subprojects that are in the same folder as parent project.
				parsingstage = specialStage;
				temporaryLength = 0;
			} break;

			default: {
				SetFirstTempW (interpreter.current);
				parsingstage = specialStage;
			}

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
				ERROR_INTERPRETER ("project:name", interpreter.current);
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
				parsingstage = SpaceC16;

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
				ERROR_INTERPRETER ("project:path", interpreter.current);
			} break;

			case TYPE_CARRIAGE_RETURN:
			case TYPE_SPACE:
			case TYPE_TAB: {

				AddTempW (TYPE_EOS);
				u8* project; 

				// TODO:
				//  Find parent project if there is one and attach it's path to this one if its relative type

				auto&& current = projects.capes.back();

				if (current.special.type == PATH_RELATIVE) {

					// Get Parent Project
					const auto& upperProjectLength = projects.capes[interpreter.special].pathLength;
					const auto& upperProject = projects.paths[interpreter.special];
					//printf ("HERE: %ls\n", (c16*) upperProject);

					MEMORY::Construct2<u8> (project, upperProjectLength, upperProject, temporaryLength, temporary);
					current.pathLength = upperProjectLength + temporaryLength - 2; // minus EOS

				} else {

					{ // Allocate & create FilePath string.
						ALLOCATE (u8, project, temporaryLength);
						memcpy (project, temporary, temporaryLength);
					}

					current.pathLength = temporaryLength - 2; // minus EOS

				}
				
				projects.paths.push_back (project);
				
				specialStage = Config;
				parsingstage = SpaceC16;

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
				ERROR_INTERPRETER ("project:config", interpreter.current);
			} break;

			
			case TYPE_EOF:
			case TYPE_CARRIAGE_RETURN:
			case TYPE_NEW_LINE: {

				AddTempW (TYPE_EOS);

				auto&& current = projects.capes.back();

				const auto& pathLength = current.pathLength;
				const auto& path = projects.paths.back();

				u8* project; // Allocate & create FilePath string.
				MEMORY::Construct2<u8> (project, pathLength, path, temporaryLength, temporary);
				MEMORY::EXIT::PUSH (project, FREE);

				{ // See if said FilePath already occurs. If not save it.
					for (u16 i = 0; i < projects.configs.size (); ++i) {
						u8 condition = 0;
						IsEqualS3_16 (condition, (u16*)project, (u16*)projects.configs[i]);
						if (condition == 2) goto error;
					}

		success:	current.configLength = temporaryLength - 2; // minus EOS
					projects.configs.push_back (project);
					MEMORY::EXIT::POP ();
					parsingstage = GetAllFiles;
					break;

					// TODO. Look into it more.
		error:		ERROR ("Said project file is arleady being loaded: `%ls`" ERROR_NEW_LINE, (c16*)path);

				}

			} break;

			default: {
				AddTempW (interpreter.current);
			}

		}

	}

}

namespace OPEN::INTERPRETER::MAIN::CASCADE {

	// Detection of a cascading constant
	//  we'll be finding the constant from the ones we already read and attach it.
	void Initialize () {
		parsingstage = Constant;
		cascadingLength = 0;
	}

	void SpaceC16 (const Interpreter& interpreter) {

		switch (interpreter.current) {

			case TYPE_EOF:
			case TYPE_NEW_LINE:
			case TYPE_ASSIGN: {
				ERROR_INTERPRETER ("main:space-cascade16", interpreter.current);
			} break;

			case TYPE_CARRIAGE_RETURN:
			case TYPE_SPACE:
			case TYPE_TAB: break; // nothing

			case TYPE_CONSTANT: specialStage = CONSTANT::Value; CASCADE::Initialize (); break;

			default: {
				SetFirstTempW (interpreter.current);
				parsingstage = specialStage;
			}

		}

	}

	void Constant (const Interpreter& interpreter) {

		switch (interpreter.current) {

			case TYPE_CARRIAGE_RETURN: break;

			//case TYPE_VARIABLE:
			//case TYPE_SECRET: 
			//case TYPE_PROJECT:
			//case TYPE_INCLUDE:
			//case TYPE_COMMAND:
			//case TYPE_QUEUE:
			//case TYPE_ASSIGN:
			case TYPE_COMMENT:
			case TYPE_EOF:
			case TYPE_NEW_LINE:
			case TYPE_TAB: {
				ERROR_INTERPRETER ("constant:constant", interpreter.current);
			} break;

			case TYPE_CONSTANT: {

				AddTemp (TYPE_EOS);
				++cascadingLength;

				// Remove key that we stored from the constant so that we can later emplace cascading value.
				temporaryLength = temporaryLength - cascadingLength;
				auto&& key = (u8*)(temporary + temporaryLength);

				{ // Cascading in Action. Match key, get key value, store.
					u32 index = 0;

					COMPARESEARCH::ArrayPartFirstMatchVector ( 
						key, cascadingLength, sizeof (c8),
						index, 
						constants.keys.size (),
						(void**)(constants.keys.data ())
					);

					if (index == constants.keys.size ()) {
						ERROR ("Invalid cascaded constant: %s" ERROR_NEW_LINE, key);
					}

					const auto& foundLength = constants.valueLengths[index];
					const auto& found = constants.values[index];

					memcpy (key, found, foundLength);
					temporaryLength += foundLength;

				}

				parsingstage = specialStage;

			} break;

			default: {
				AddTemp (interpreter.current);
				++cascadingLength;
			}

		}

	}

}

namespace OPEN::INTERPRETER::MAIN::CONSTANT {

	void Initialize () {
		temporaryLength = 0;
		parsingstage = Name;
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
			//
			case TYPE_NEW_LINE:
			case TYPE_EOF: {
				ERROR_INTERPRETER ("constant:name", interpreter.current);
			} break;

			case TYPE_CARRIAGE_RETURN:
			case TYPE_SPACE:
			case TYPE_TAB: break; // nothing

			case TYPE_ASSIGN: {

				AddTemp (TYPE_EOS);

				u8* name; ALLOCATE (u8, name, temporaryLength);
				memcpy (name, temporary, temporaryLength);

				constants.keys.push_back (name);

				temporaryLength = 0; // RESET

				specialStage = Value;
				parsingstage = CASCADE::SpaceC16;

			} break;

			default: {
				AddTemp (interpreter.current);
			}

		}

	}

	void Value (const Interpreter& interpreter) {

		switch (interpreter.current) {

			//case TYPE_PROJECT:
			//case TYPE_INCLUDE:
			//case TYPE_COMMAND:
			//case TYPE_QUEUE:
			//case TYPE_COMMENT:
			//case TYPE_ASSIGN:
			case TYPE_TAB: {
				ERROR_INTERPRETER ("constant:value", interpreter.current);
			} break;

			
			case TYPE_EOF:
			case TYPE_CARRIAGE_RETURN:
			case TYPE_NEW_LINE: {

				AddTempW (TYPE_EOS);

				u8* value; ALLOCATE (u8, value, temporaryLength);
				memcpy (value, temporary, temporaryLength);

				constants.valueLengths.push_back (temporaryLength - 2);
				constants.values.push_back (value);
				parsingstage = Main;

			} break;

			
			case TYPE_CONSTANT: specialStage = Value; CASCADE::Initialize (); break;

			//case TYPE_VARIABLE:
			//case TYPE_SECRET: { 
			//	cascadingLength = 0;
			//	AddTempW (interpreter.current);
			//} break;

			default: {
				AddTempW (interpreter.current);
			}

		}

	}

}

namespace OPEN::INTERPRETER::MAIN::COMMAND {

	void Initialize () {
		temporaryLength = 0;
		parsingstage = Name;
	}

	void SpaceC16 (const Interpreter& interpreter) {

		switch (interpreter.current) {

			case TYPE_EOF:
			case TYPE_NEW_LINE:
			case TYPE_ASSIGN: {
				ERROR_INTERPRETER ("command:space16", interpreter.current);
			} break;

			case TYPE_CONSTANT: specialStage = Value; CASCADE::Initialize (); break;

			case TYPE_CARRIAGE_RETURN:
			case TYPE_SPACE:
			case TYPE_TAB: break; // nothing

			default: {
				SetFirstTempW (interpreter.current);
				parsingstage = specialStage;
			}

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
			//
			case TYPE_NEW_LINE:
			case TYPE_EOF: {
				ERROR_INTERPRETER ("command:name", interpreter.current);
			} break;

			case TYPE_CARRIAGE_RETURN:
			case TYPE_SPACE:
			case TYPE_TAB: break; // nothing

			case TYPE_ASSIGN: {

				AddTemp (TYPE_EOS);

				u8* name; ALLOCATE (u8, name, temporaryLength);
				memcpy (name, temporary, temporaryLength);

				commands.keys.push_back (name);

				temporaryLength = 0; // RESET

				specialStage = Value;
				parsingstage = SpaceC16;

			} break;

			default: {
				AddTemp (interpreter.current);
			}

		}
	}

	void Value (const Interpreter& interpreter) {

		switch (interpreter.current) {

			//case TYPE_PROJECT:
			//case TYPE_INCLUDE:
			//case TYPE_COMMAND:
			//case TYPE_QUEUE:
			//case TYPE_ASSIGN:
			case TYPE_TAB: {
				ERROR_INTERPRETER ("command:value", interpreter.current);
			} break;

			case TYPE_CONSTANT: specialStage = Value; CASCADE::Initialize (); break;
			
			case TYPE_EOF:
			case TYPE_CARRIAGE_RETURN:
			case TYPE_NEW_LINE: {

				AddTempW (TYPE_EOS);

				u8* value; ALLOCATE (u8, value, temporaryLength);
				memcpy (value, temporary, temporaryLength);

				commands.values.push_back (value);
				parsingstage = Main;

			} break;

			default: {
				AddTempW (interpreter.current);
			}

		}

	}

}

namespace OPEN::INTERPRETER::MAIN::QUEUE {

	void Initialize () {
		temporaryLength = 0;
		parsingstage = Name;
	}

	void SpaceC16 (const Interpreter& interpreter) {

		switch (interpreter.current) {

			case TYPE_EOF:
			case TYPE_NEW_LINE:
			case TYPE_ASSIGN: {
				ERROR_INTERPRETER ("queue:space16", interpreter.current);
			} break;

			case TYPE_CONSTANT: specialStage = Value; CASCADE::Initialize (); break;

			case TYPE_CARRIAGE_RETURN:
			case TYPE_SPACE:
			case TYPE_TAB: break; // nothing

			default: {
				SetFirstTemp (interpreter.current);
				parsingstage = specialStage;
			}

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
			//
			case TYPE_NEW_LINE:
			case TYPE_EOF: {
				ERROR_INTERPRETER ("queue:name", interpreter.current);
			} break;

			case TYPE_CARRIAGE_RETURN:
			case TYPE_SPACE:
			case TYPE_TAB: break; // nothing

			case TYPE_ASSIGN: {

				AddTemp (TYPE_EOS);

				u8* name; ALLOCATE (u8, name, temporaryLength);
				memcpy (name, temporary, temporaryLength);

				queues.keys.push_back (name);

				temporaryLength = 0; // RESET

				specialStage = Value;
				parsingstage = SpaceC16;

			} break;

			default: {
				AddTemp (interpreter.current);
			}

		}
	}

	void Value (const Interpreter& interpreter) {

		switch (interpreter.current) {

			//case TYPE_PROJECT:
			//case TYPE_INCLUDE:
			//case TYPE_COMMAND:
			//case TYPE_QUEUE:
			//case TYPE_ASSIGN:
			case TYPE_TAB: {
				ERROR_INTERPRETER ("queue:value", interpreter.current);
			} break;

			case TYPE_CONSTANT: specialStage = Value; CASCADE::Initialize (); break;
			
			case TYPE_EOF:
			case TYPE_CARRIAGE_RETURN:
			case TYPE_NEW_LINE: {

				AddTemp (TYPE_EOS);

				u8* value; ALLOCATE (u8, value, temporaryLength);
				memcpy (value, temporary, temporaryLength);

				queues.values.push_back (value);
				parsingstage = Main;

			} break;

			default: {
				AddTemp (interpreter.current);
			}

		}

	}

}