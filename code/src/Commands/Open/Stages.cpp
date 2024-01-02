#include "Commands/Open/Stages.hpp"

#include "Buffors.hpp"

namespace Commands::Open::Stages {

	Stage Current, Next; // Next is rarely used.

	const size bufforFilesCountIndex = 0;
	size bufforSizeNameIndex = 1;
	size bufforSizeContextIndex = 2;
	size bufforIndex = 3;

	//uint8 filesCount = 0;
	uint8 lengthTemp = 0;

	void Initialize() {
		memoryBlockA.data[bufforFilesCountIndex] = 0;
	}

	void FileReset() {
		memoryBlockA.data[bufforFilesCountIndex] = 0;
	}
	
	//ptr<uint8> projectsBuffor = ptr<uint8>(memoryBlockA, sizeof(size) /* offset */ );
	//uint8* projectsBuffor = memoryBlockA.data;

	StageProc MainBeginStage(const StageParams& stage) {

		//printf("%s", "MAIN\n");

		switch (stage.current) {

			case COMMENT: {
				Next = MainBeginStage;
				Current = Comment;
			} break;

			case NEW_LINE:
			case SPACE:
			case TAB: {
			} break;

			case EOF: {
				// ERROR FILE IS EMPTY!
				printf("%s%s", "ERROR:", "1\n");
			} break;

			default: {
				//printf("%s", "CALL\n");
				//projectsBuffor[bufforIndex] = stage.current;
				memoryBlockA.data[bufforIndex] = stage.current;
				++bufforIndex; ++lengthTemp;
				Current = File::Begin;
			}

		}
	}


	StageProc BeginStage(const StageParams& stage) {
		switch (stage.current) {

			case COMMENT: {
				Next = BeginStage;
				Current = Comment;
			} break;

			case NEW_LINE:
			case SPACE:
			case TAB: {} break;

			case EOF: {
				// ERROR FILE IS EMPTY!
				printf("%s%s", "ERROR:", "2\n");
			} break;

			default: {
				memoryBlockA.data[bufforIndex] = stage.current;
				++bufforIndex; ++lengthTemp;
				Current = Command::Begin;
			}

		}
	}


	StageProc Comment(const StageParams& stage) {

		//printf("%s", "Comment\n");

		switch (stage.current) {
            case NEW_LINE: {
				//printf("%s", "NEWLINE\n");
                Current = Next;
            } break;
			
			case EOF:
			default: {
				//printf("%c", stage.current);
			}
		}
	}

}


namespace Commands::Open::Stages::File {

	StageProc Begin(const StageParams& stage) {

		//printf("%s", "FileBegin\n");

		switch (stage.current) {

			case SECTION_SRT:
            case SECTION_END:
			case COMMENT: {
				// ERROR FILE IS ILL-FORMED!
				printf("%s%s", "ERROR:", "4\n");
			} break;

			case NEW_LINE:
			case SPACE:
			case TAB: {} break;
            
			case ASSIGN: {
				memoryBlockA.data[bufforSizeNameIndex] = lengthTemp;
				lengthTemp = 0; // RESET
				Current = Assign;
			} break;

			// We've READ thought all. ALL good - sort off.
        	case EOF: {} break;

            default: {
                memoryBlockA.data[bufforIndex] = stage.current;
				++bufforIndex; ++lengthTemp;
            }
		}
	}


	StageProc Assign(const StageParams& stage) {

		//printf("%s", "Assign\n");

		switch (stage.current) {
			case NEW_LINE:
			case SPACE:
			case TAB: {} break;

			case EOF: {
				// ERROR FILE IS ILL-FORMED!
				printf("%s%s", "ERROR:", "5\n");
			} break;

			default: {
                memoryBlockA.data[bufforIndex] = stage.current;
				++bufforIndex; ++lengthTemp;
				Current = Context;
            }
		}
	}


	StageProc Context(const StageParams& stage) {
		switch (stage.current) {

			case EOF: {
				memoryBlockA.data[bufforSizeContextIndex] = lengthTemp;
				lengthTemp = 0; // RESET (NOT NEEDED THO)

				// Increase the number of files.
				++memoryBlockA.data[bufforFilesCountIndex];
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
				++memoryBlockA.data[bufforFilesCountIndex];

				Current = MainBeginStage; // Look for another
			} break;

			case SPACE:
			case TAB: {
				// ERROR FILE IS ILL-FORMED!
				printf("%s%s", "ERROR:", "6\n");
			}

			default: {
                memoryBlockA.data[bufforIndex] = stage.current;
				++bufforIndex; ++lengthTemp;
            }
		}
	}

}


namespace Commands::Open::Stages::Command {

	StageProc Begin(const StageParams& stage) {
		switch (stage.current) {

		}
	}

}