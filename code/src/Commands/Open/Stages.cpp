#include "Commands/Open/Stages.hpp"

#include "Buffors.hpp"

#

namespace Commands::Open::Stages {

	Stage Current, Next; // Next is rarely used.

	size bufforSizeNameIndex = 1 + INDEX_OFFSET;
	size bufforSizeContextIndex = 2 + INDEX_OFFSET;
	size bufforIndex = 3 + INDEX_OFFSET;

	uint8 lengthTemp = 0;


	void Initialize () {
		memoryBlockA.data[INDEX_FILES_COUNT] = 0;
	}


	void ImportReset () {
		memoryBlockA.data[INDEX_FILES_COUNT] = 0;
	}


	StageProc MainFile (const StageParams& stage) {

		switch (stage.current) {

			case NEW_LINE:
			case SPACE:
			case TAB:
			case EOF: {
			} break;

			case COMMENT: {
				Next = MainFile;
				Current = Comment;
			} break;

			case CONSTANT: {
				Next = MainFile;
				Current = Constant::Name;
			} break;

			case QUEUE: {
				Next = MainFile;
				Current = Queue::Name;
			} break;

			case IMPORT: {
				Current = Import::Name;
			} break;

			default: { /* COMMAND does not have a beginning symbol! */
				//memoryBlockA.data[bufforIndex] = stage.current;
				//++bufforIndex; ++lengthTemp;
				break;
			}

		}
	}


	StageProc ProjectFile (const StageParams& stage) {
		switch (stage.current) {

		}
	}


	StageProc Comment (const StageParams& stage) {
		switch (stage.current) {
            case NEW_LINE: {
                Current = Next;
            } break;
			
			case EOF:
			default: {}
		}
	}

}


namespace Commands::Open::Stages::Constant {

	StageProc Name (const StageParams& stage) {

		switch (stage.current) {
			
		}
		
	}


	StageProc Assign (const StageParams& stage) {

		switch (stage.current) {

		}

	}


	StageProc Context (const StageParams& stage) {

		switch (stage.current) {

		}
		
	}

}


namespace Commands::Open::Stages::Import {

	StageProc Name (const StageParams& stage) {
		switch (stage.current) {

			case SECTION_SRT:
            case SECTION_END:
			case COMMENT: 
			case EOF: {
				printf("%s%s", "ERROR: ", "FILE_IS_ILL-FORMED!\n");
			} exit(1);

			case NEW_LINE:
			case SPACE:
			case TAB: {} break;
            
			case ASSIGN: {
				memoryBlockA.data[bufforSizeNameIndex] = lengthTemp;
				lengthTemp = 0; // RESET
				Current = Assign;
			} break;

            default: {
                memoryBlockA.data[bufforIndex] = stage.current;
				++bufforIndex; ++lengthTemp;
            }

		}
	}


	StageProc Assign (const StageParams& stage) {
		switch (stage.current) {
			case NEW_LINE:
			case SPACE:
			case TAB: {} break;

			case EOF: {
				printf("%s%s", "ERROR: ", "FILE_IS_ILL-FORMED!\n");
			} break;

			default: {
                memoryBlockA.data[bufforIndex] = stage.current;
				++bufforIndex; ++lengthTemp;
				Current = Context;
            }
		}
	}


	StageProc Context (const StageParams& stage) {
		switch (stage.current) {

			case EOF: {
				memoryBlockA.data[bufforSizeContextIndex] = lengthTemp;
				lengthTemp = 0; // RESET (NOT NEEDED THO)

				// Increase the number of files.
				++memoryBlockA.data[INDEX_FILES_COUNT];
			} break;

			case NEW_LINE: {
				memoryBlockA.data[bufforSizeContextIndex] = lengthTemp; // SET ON PREVIOUS INDEX
				lengthTemp = 0; // RESET

				// RESERVE SPACE (INDEXES) FOR NEW COMMANDS (THEIR SIZE VALUES)
				bufforSizeNameIndex = bufforIndex;
				++bufforIndex;
				bufforSizeContextIndex = bufforIndex;
				++bufforIndex;

				//printf("AA: %" PRIu64 " %" PRIu64 "\n", bufforSizeNameIndex, bufforSizeContextIndex);

				// Increase the number of files.
				++memoryBlockA.data[INDEX_FILES_COUNT];

				Current = MainFile; // Look for another
			} break;

			case SPACE:
			case TAB: {
				printf("%s%s", "ERROR: ", "FILE_IS_ILL-FORMED!\n");
			} exit(1);

			default: {
                memoryBlockA.data[bufforIndex] = stage.current;
				++bufforIndex; ++lengthTemp;
            }
		}
	}

}


namespace Commands::Open::Stages::Command {

	StageProc Name(const StageParams& stage) {
		switch (stage.current) {

		}
	}

}


namespace Commands::Open::Stages::Queue {

	StageProc Name(const StageParams& stage) {
		switch (stage.current) {

		}
	}

}