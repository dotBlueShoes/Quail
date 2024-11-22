// Created 2024.11.02 by Matthew Strumiłło (dotBlueShoes)
//
#pragma once
#include "base/types.hpp"
#include "base/error.hpp"
#include "../data.hpp"
#include "types.hpp"

#include "comparesearch.hpp"


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

namespace OPEN::INTERPRETER::MAIN::CONSTANT {
	void Initialize ();
	void Name		(const Interpreter&);
	void Value		(const Interpreter&);

	
}

namespace OPEN::INTERPRETER::MAIN::CASCADE {
	void Initialize ();
	void Constant 	(const Interpreter&);

	//void Variable 	(const Interpreter&);
	//void Secret 	(const Interpreter&);
}

namespace OPEN::INTERPRETER::MAIN::COMMAND {
	void Initialize ();
	void Name		(const Interpreter&);
	void Value		(const Interpreter&);
	//void Constant (const Interpreter&);
	//void Variable (const Interpreter&);
	//void Secret 	(const Interpreter&);
}

namespace OPEN::INTERPRETER::MAIN {

	void GetAllFiles (const Interpreter& interpreter) {

		switch (interpreter.current) {

			case TYPE_COMMENT: parsingstage = Comment; specialStage = GetAllFiles; break;
			case TYPE_INCLUDE: INCLUDE::Initialize (); break;
			case TYPE_PROJECT: PROJECT::Initialize (); break;
			default: { }

		}

	}

	void Main (const Interpreter& interpreter) {

		switch (interpreter.current) {

			case TYPE_NEW_LINE:
			case TYPE_SPACE:
			case TYPE_TAB:
			case TYPE_EOF: {
				//putc (interpreter.current, stdout);
			} break;

			case TYPE_INCLUDE: 
			case TYPE_PROJECT: 
			case TYPE_COMMENT: {
				// We're Skipping them and todoso we're simply treating them as comments here.
				parsingstage = Comment; specialStage = Main;
			} break;

			case TYPE_CONSTANT: CONSTANT::Initialize (); break;

			case TYPE_SECRET: {
				//LOGINFO ("Detected Secret!\n");
			} break;

			case TYPE_VARIABLE: {
				//LOGINFO ("Detected Variable!\n");
			} break;

			case TYPE_COMMAND: COMMAND::Initialize (); break;

			case TYPE_QUEUE: {
				//LOGINFO ("Detected Queue!\n");
			} break;

			default: { 
				//putc (interpreter.current, stdout);
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
				ERROR ("Invalid syntax '%s' [%d] \n\n", "main:space8", interpreter.current);
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
			case TYPE_ASSIGN: {
				ERROR ("Invalid syntax '%s' [%d] \n\n", "main:space16", interpreter.current);
			} break;

			// !!! ERROR this also means that '%' character will make us move to COMMAND::Value 
			//  HOWEVER THIS INTERPRETER IS USED IN PROJECT!
			// !!! HACK when working with Queues i will need a new Spacec16 variant as this refers to COMMAND:VALUE
			case TYPE_CONSTANT: specialStage = COMMAND::Value; CASCADE::Initialize (); break;

			case TYPE_CARRIAGE_RETURN:
			case TYPE_SPACE:
			case TYPE_TAB: break; // nothing

			default: {
				SetFirstTempW (interpreter.current);
				parsingstage = specialStage;
			}

		}

	}

	// CASCADING VARIANT
	void SpaceCC16 (const Interpreter& interpreter) {

		switch (interpreter.current) {

			case TYPE_EOF:
			case TYPE_NEW_LINE:
			//case TYPE_PROJECT:
			//case TYPE_INCLUDE:
			//case TYPE_COMMAND:
			//case TYPE_QUEUE:
			//case TYPE_COMMENT:
			case TYPE_ASSIGN: {
				ERROR ("Invalid syntax '%s' [%d] \n\n", "main:space-cascade16", interpreter.current);
			} break;

			case TYPE_CARRIAGE_RETURN:
			case TYPE_SPACE:
			case TYPE_TAB: break; // nothing

			case TYPE_CONSTANT: specialStage = CONSTANT::Value; CASCADE::Initialize (); break;

			//case TYPE_VARIABLE: {} break;
			//case TYPE_SECRET: {} break;

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
				ERROR ("Invalid syntax '%s' [%d] \n\n", "include:context", interpreter.current);
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

			default: ERROR ("Invalid syntax '%s' [%d] \n\n", "project:type", interpreter.current);

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
				ERROR ("Invalid syntax '%s' [%d] \n\n", "name:value", interpreter.current);
			} break;

			case TYPE_CARRIAGE_RETURN:
			case TYPE_SPACE:
			case TYPE_TAB: break; // nothing

			case TYPE_ASSIGN: {

				AddTemp (TYPE_EOS);

				// CPY. So it stays in memmory.
				u8* project; ALLOCATE (u8, project, temporaryLength);
				memcpy (project, temporary, temporaryLength);

				//ToLowCase ((c8*)project, temporaryLength); // Conversion

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
				ERROR ("Invalid syntax '%s' [%d] \n\n", "project:path", interpreter.current);
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
				ERROR ("Invalid syntax '%s' [%d] \n\n", "project:config", interpreter.current);
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

				{ // See if said FilePath already occurs. If not save it.
					for (u16 i = 0; i < projects.configs.size(); ++i) {
						u8 condition = 0;
						IsEqualS3_16 (condition, (u16*)project, (u16*)projects.configs[i]);
						if (condition == 2) goto error;
					}

		success:	current.configLength = temporaryLength - 2; // minus EOS
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

namespace OPEN::INTERPRETER::MAIN::CASCADE {

	// Detection of a cascading constant
	//  we'll be finding the constant from the ones we already read and attach it.
	void Initialize () {
		parsingstage = Constant;
		cascadingLength = 0;
	}

	void Constant (const Interpreter& interpreter) {

		switch (interpreter.current) {

			case TYPE_CARRIAGE_RETURN: break;

			case TYPE_VARIABLE:
			case TYPE_SECRET: 
			case TYPE_EOF:
			case TYPE_NEW_LINE:
			case TYPE_PROJECT:
			case TYPE_INCLUDE:
			case TYPE_COMMAND:
			case TYPE_QUEUE:
			case TYPE_COMMENT:
			case TYPE_ASSIGN:
			case TYPE_TAB: {
				ERROR ("Invalid syntax '%s' [%d] \n\n", "constant:constant", interpreter.current);
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
						ERROR ("Invalid constant: %s\n\n", key);
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
			case TYPE_NEW_LINE:
			case TYPE_EOF: {
				ERROR ("Invalid syntax '%s' [%d] \n\n", "constant:name", interpreter.current);
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
				parsingstage = MAIN::SpaceCC16;

			} break;

			default: {
				AddTemp (interpreter.current);
			}

		}

	}

	void Value (const Interpreter& interpreter) {

		switch (interpreter.current) {

			case TYPE_PROJECT:
			case TYPE_INCLUDE:
			case TYPE_COMMAND:
			case TYPE_QUEUE:
			case TYPE_COMMENT:
			case TYPE_ASSIGN:
			case TYPE_TAB: {
				ERROR ("Invalid syntax '%s' [%d] \n\n", "constant:value", interpreter.current);
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
				ERROR ("Invalid syntax '%s' [%d] \n\n", "command:name", interpreter.current);
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
				parsingstage = MAIN::SpaceC16;

			} break;

			default: {
				AddTemp (interpreter.current);
			}

		}
	}

	void Value (const Interpreter& interpreter) {

		switch (interpreter.current) {

			case TYPE_PROJECT:
			case TYPE_INCLUDE:
			case TYPE_COMMAND:
			case TYPE_QUEUE:
			//case TYPE_COMMENT:
			case TYPE_ASSIGN:
			case TYPE_TAB: {
				ERROR ("Invalid syntax '%s' [%d] \n\n", "command:value", interpreter.current);
			} break;

			case TYPE_CONSTANT: specialStage = Value; CASCADE::Initialize (); break;
			
			case TYPE_EOF:
			case TYPE_CARRIAGE_RETURN:
			case TYPE_NEW_LINE: {

				// Not really needed.
				// AddTempW (' ');
				// AddTempW ('&');

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