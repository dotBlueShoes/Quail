#include "Commands/Open/Stages.hpp"

#include "Buffors.hpp"



namespace Commands::Open::Stages {


	getter constexpr size GetInitialBufforIndex () {
		return SPACE_SIZE_FILES_COUNT + INDEX_OFFSET + SPACE_SIZE_NAME + SPACE_SIZE_CONTEXT;
	}


	getter constexpr size GetCurrentIndexConstantsCount (const size& currentFileIndex) {
		return currentFileIndex;
	}


	getter constexpr size GetCurrentIndexImportsCount (const size& currentFileIndex) {
		return currentFileIndex + SPACE_SIZE_CONSTANTS_COUNT;
	}


	getter constexpr size GetCurrentIndexCommandsCount (const size& currentFileIndex) {
		return currentFileIndex + SPACE_SIZE_CONSTANTS_COUNT + SPACE_SIZE_IMPORTS_COUNT;
	}


	getter constexpr size GetCurrentIndexQueuesCount (const size& currentFileIndex) {
		return currentFileIndex + SPACE_SIZE_CONSTANTS_COUNT + SPACE_SIZE_IMPORTS_COUNT + SPACE_SIZE_COMMANDS_COUNT;
	}


	// Next is rarely used.
	Stage Current, Next; 


	size bufforSizeNameIndex = SPACE_SIZE_FILES_COUNT + INDEX_OFFSET;
	size bufforSizeContextIndex = SPACE_SIZE_FILES_COUNT + SPACE_SIZE_NAME + INDEX_OFFSET;
	size bufforIndex = GetInitialBufforIndex();


	// Additional variable for temporal storage use.
	uint8 lengthTemp = 0;


	// Varaible to distinguish file imports inside bufforIndex var.
	// When we reed a new file then the current bufforIndex is being saved here.
	size fileIndex = 1;


	void Initialize () {
		memoryBlockA.data[INDEX_FILES_COUNT] = 0;
		memoryBlockA.data[SPACE_SIZE_FILES_COUNT + GetHeaderIndex(0)] = 0;
		memoryBlockA.data[SPACE_SIZE_FILES_COUNT + GetHeaderIndex(1)] = 0;
		memoryBlockA.data[SPACE_SIZE_FILES_COUNT + GetHeaderIndex(2)] = 0;
		memoryBlockA.data[SPACE_SIZE_FILES_COUNT + GetHeaderIndex(3)] = 0;
		
		memoryBlockA.data[SPACE_SIZE_FILES_COUNT + GetHeaderIndex(4) + 0] = 0;
		memoryBlockA.data[SPACE_SIZE_FILES_COUNT + GetHeaderIndex(4) + 1] = 0;
	}


	void Reset () {
		Initialize ();
		bufforIndex = GetInitialBufforIndex ();
		bufforSizeNameIndex = SPACE_SIZE_FILES_COUNT + INDEX_OFFSET;
		bufforSizeContextIndex = SPACE_SIZE_FILES_COUNT + SPACE_SIZE_NAME + INDEX_OFFSET;
	}
	

	void AddImport () {
		// ADD INFORMATION ABOUT INCLUSION OF ANOTHER FILE.
		memoryBlockA.data[INDEX_FILES_COUNT]++;

		//printf("call");

		// SIGN BEFORE NEW FILE
		//printf("-:%i\n", memoryBlockA.data[fileIndex-1]);

		// REPOSITION COUNT INDEXES.
		// Because we're adding 2 spaces after each assign for new name and context count variables we need to discard those and add up later.
		bufforIndex -= 2;

		fileIndex = bufforIndex;

		// ADD UP LATER
		bufforSizeNameIndex = bufforIndex + GetHeaderIndex(4);
		bufforSizeContextIndex = bufforIndex + GetHeaderIndex(4) + 1;

		// POINT CORRECTLY AT WRITABLE DATA.
		// add offset name_count & context_count
		bufforIndex += 2 + 4;

		// ZERO-MEMORY
		//memoryBlockA.data[fileIndex] = 0;
		//memoryBlockA.data[fileIndex + 1] = 0;
		//memoryBlockA.data[fileIndex + 2] = 0;
		//memoryBlockA.data[fileIndex + 3] = 0;
		//std::memset
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
				Current = Queue::Name;
			} break;

			case IMPORT: {
				Current = Import::Name;
			} break;

			default: { /* COMMAND does not have a beginning symbol! */
				memoryBlockA.data[bufforIndex] = stage.current;
				++bufforIndex; ++lengthTemp;
				Current = Command::Name;
			}

		}
	}


	StageProc ProjectFile (const StageParams& stage) {
		switch (stage.current) {
			case NEW_LINE:
			case SPACE:
			case TAB:
			case EOF: {
			} break;

			case COMMENT: {
				Next = ProjectFile;
				Current = Comment;
			} break;

			case CONSTANT: {
				Current = Constant::Name;
			} break;

			case QUEUE: {
				Current = Queue::Name;
			} break;

			case IMPORT: {
				Current = Import::Name;
			} break;

			default: { /* COMMAND does not have a beginning symbol! */
				memoryBlockA.data[bufforIndex] = stage.current;
				++bufforIndex; ++lengthTemp;
				Current = Command::Name;
			}
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

			case SECTION_SRT:
            case SECTION_END:
			case CONSTANT:
			case COMMENT:
			case IMPORT:
			case QUEUE:
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

				// Increase the number of constants.
				++memoryBlockA.data[GetCurrentIndexConstantsCount(fileIndex)];
			} break;

			case NEW_LINE: {
				memoryBlockA.data[bufforSizeContextIndex] = lengthTemp; // SET ON PREVIOUS INDEX
				lengthTemp = 0; // RESET

				// RESERVE SPACE (INDEXES) FOR NEW COMMANDS (THEIR SIZE VALUES)
				bufforSizeNameIndex = bufforIndex;
				++bufforIndex;
				bufforSizeContextIndex = bufforIndex;
				++bufforIndex;

				//printf("\n%llu%s", GetCurrentIndexConstantsCount(fileIndex), "CALL!\n");

				// Increase the number of constants.
				//printf("1:%i\n", memoryBlockA.data[GetCurrentIndexConstantsCount(fileIndex)]);
				++memoryBlockA.data[GetCurrentIndexConstantsCount(fileIndex)];
				//printf("2:%i\n", memoryBlockA.data[GetCurrentIndexConstantsCount(fileIndex)]);

				Current = MainFile; // Look for another
			} break;

			//case SPACE:
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



namespace Commands::Open::Stages::Import {

	StageProc Name (const StageParams& stage) {
		switch (stage.current) {

			case SECTION_SRT:
            case SECTION_END:
			case CONSTANT:
			case COMMENT: 
			case IMPORT:
			case QUEUE:
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

				// If theres a file after this file then we need to ensure that space...
				// Thats ugly...
				bufforIndex += 2;

				// Increase the number of files.
				++memoryBlockA.data[GetCurrentIndexImportsCount(fileIndex)];
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
				++memoryBlockA.data[GetCurrentIndexImportsCount(fileIndex)];
				//printf("1:%i\n", memoryBlockA.data[GetCurrentIndexImportsCount(fileIndex)]);

				Current = MainFile; // Look for another
			} break;

			//case SPACE:
			//case TAB: {
			//} break;

			default: {
                memoryBlockA.data[bufforIndex] = stage.current;
				++bufforIndex; ++lengthTemp;
            }
		}
	}

}



namespace Commands::Open::Stages::Command {

	StageProc Name (const StageParams& stage) {
		switch (stage.current) {

			case SECTION_SRT:
            case SECTION_END:
			case CONSTANT:
			case COMMENT: 
			case IMPORT:
			case QUEUE:
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

				// If theres a file after this file then we need to ensure that space...
				// Thats ugly...
				bufforIndex += 2;

				// Increase the number of files.
				++memoryBlockA.data[GetCurrentIndexCommandsCount(fileIndex)];
			} break;

			case NEW_LINE: {
				memoryBlockA.data[bufforSizeContextIndex] = lengthTemp; // SET ON PREVIOUS INDEX
				lengthTemp = 0; // RESET

				// RESERVE SPACE (INDEXES) FOR NEW COMMANDS (THEIR SIZE VALUES)
				bufforSizeNameIndex = bufforIndex;
				++bufforIndex;
				bufforSizeContextIndex = bufforIndex;
				++bufforIndex;

				// Increase the number of files.
				++memoryBlockA.data[GetCurrentIndexCommandsCount(fileIndex)];

				Current = MainFile;
			} break;

			//case SPACE:
			//case TAB: {
			//} break;

			default: {
                memoryBlockA.data[bufforIndex] = stage.current;
				++bufforIndex; ++lengthTemp;
            }
		}
	}

}



namespace Commands::Open::Stages::Queue {

	StageProc Name (const StageParams& stage) {
		switch (stage.current) {

			case SECTION_SRT:
            case SECTION_END:
			case CONSTANT:
			case COMMENT: 
			case IMPORT:
			case QUEUE:
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

				// If theres a file after this file then we need to ensure that space...
				// Thats ugly...
				bufforIndex += 2;

				// Increase the number of files.
				++memoryBlockA.data[GetCurrentIndexQueuesCount(fileIndex)];
			} break;

			case NEW_LINE: {
				memoryBlockA.data[bufforSizeContextIndex] = lengthTemp; // SET ON PREVIOUS INDEX
				lengthTemp = 0; // RESET

				// RESERVE SPACE (INDEXES) FOR NEW COMMANDS (THEIR SIZE VALUES)
				bufforSizeNameIndex = bufforIndex;
				++bufforIndex;
				bufforSizeContextIndex = bufforIndex;
				++bufforIndex;

				// Increase the number of files.
				++memoryBlockA.data[GetCurrentIndexQueuesCount(fileIndex)];

				Current = MainFile;
			} break;

			case SPACE:
			case TAB: {
			} break;

			default: {
                memoryBlockA.data[bufforIndex] = stage.current;
				++bufforIndex; ++lengthTemp;
            }
		}
	}

}