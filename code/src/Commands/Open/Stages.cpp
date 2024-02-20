#include "Commands/Open/Stages.hpp"

#include "Buffors.hpp"


namespace Commands::Open {
	uint8 directoryPath = 0;
}


namespace Commands::Open::Stages {


	getter size GetInitialBufforIndex () {
		return GetHeaderOffset () + SPACE_SIZE_NAME + SPACE_SIZE_CONTEXT;
	}

	// ERORRS!!! THIS MEANS EVERY FOR EVERY INCLUDE FILE WE WILL HAVE IT'S PATH FULLY STORRED INSIDE BUFFOR !!!!!!!
	getter size GetCurrentIndexConstantsCount (const size& currentFileIndex) {
		return currentFileIndex + GetDirectoryOffset ();
	}


	getter size GetCurrentIndexImportsCount (const size& currentFileIndex) {
		return currentFileIndex + GetDirectoryOffset () + SPACE_SIZE_CONSTANTS_COUNT;
	}


	getter size GetCurrentIndexCommandsCount (const size& currentFileIndex) {
		return currentFileIndex + GetDirectoryOffset () + SPACE_SIZE_CONSTANTS_COUNT + SPACE_SIZE_IMPORTS_COUNT;
	}


	getter size GetCurrentIndexQueuesCount (const size& currentFileIndex) {
		return currentFileIndex + GetDirectoryOffset () + SPACE_SIZE_CONSTANTS_COUNT + SPACE_SIZE_IMPORTS_COUNT + SPACE_SIZE_COMMANDS_COUNT;
	}


	// Next is rarely used.
	Stage Current, Next; 


	size bufforSizeNameIndex 	= 0; // Now sets with initialize. ;GetHeaderOffset ();
	size bufforSizeContextIndex = 0; // Now sets with initialize. ;GetHeaderOffset () + SPACE_SIZE_NAME;
	size bufforIndex 			= 0; // Now sets with initialize. ;GetInitialBufforIndex ();


	// Additional variable for temporal storage use.
	uint8 lengthTemp = 0;


	// Varaible to distinguish file imports inside bufforIndex var.
	// When we reed a new file then the current bufforIndex is being saved here.
	size fileIndex = 1;


	void Initialize (
		IN 							const uint8& filePathLength,	
		INREADS (filePathLength) 	const charFilePath* const filePath
	) {
		memoryBlockA.data[INDEX_FILES_COUNT] = 0;

		// It's wide-char!
		memoryBlockA.data[SPACE_SIZE_FILES_COUNT + SPACE_SIZE_DIRECTORY_LENGTH] = filePathLength;
		directoryPath = (filePathLength * 2) + 1;

		// TODO:
		// SAVE wchar filePath after 'SPACE_SIZE_DIRECTORY_LENGTH' index.
		// LOAD it during DisplayFiles()

		// SAVE FilePath
		for (uint8 i = 0; i < filePathLength; ++i) {
			memoryBlockA.data[SPACE_SIZE_FILES_COUNT + SPACE_SIZE_DIRECTORY_LENGTH + ((i * 2) + 1)] = (filePath[i] << 8);
			memoryBlockA.data[SPACE_SIZE_FILES_COUNT + SPACE_SIZE_DIRECTORY_LENGTH + ((i * 2) + 2)] = filePath[i];
		}

		//memoryBlockA.data[SPACE_SIZE_FILES_COUNT + SPACE_SIZE_DIRECTORY_LENGTH + 1] = 20;
		//memoryBlockA.data[SPACE_SIZE_FILES_COUNT + SPACE_SIZE_DIRECTORY_LENGTH + 2] = 54;


		//printf("\n3v: %hhu", memoryBlockA.data[SPACE_SIZE_FILES_COUNT + SPACE_SIZE_DIRECTORY_LENGTH + ((32 * 2) + 2)]);
		//printf("\n2v: %hhu", memoryBlockA.data[SPACE_SIZE_FILES_COUNT + SPACE_SIZE_DIRECTORY_LENGTH + ((33 * 2) + 2)]);
		//printf("\n1: %hhu", memoryBlockA.data[SPACE_SIZE_FILES_COUNT + SPACE_SIZE_DIRECTORY_LENGTH + 2]);
		//printf("\nl: %hhu", memoryBlockA.data[SPACE_SIZE_FILES_COUNT + SPACE_SIZE_DIRECTORY_LENGTH + (((filePathLength-1) * 2) + 2)]);
		//printf("\na: %hhu", filePathLength);
		//printf("\nb: %hhu\n", GetDirectoryOffset ());

		//auto&& bFilePath = memoryBlockA.data + SPACE_SIZE_FILES_COUNT + SPACE_SIZE_DIRECTORY_LENGTH + 1;
		//fwrite (bFilePath, sizeof (wchar), filePathLength, stdout);


		bufforSizeNameIndex		= GetHeaderOffset ();
		bufforSizeContextIndex	= GetHeaderOffset () + SPACE_SIZE_NAME;
		bufforIndex 			= GetInitialBufforIndex ();


		memoryBlockA.data[SPACE_SIZE_FILES_COUNT + GetDirectoryOffset () + GetHeaderIndex (0)]		= 0;
		memoryBlockA.data[SPACE_SIZE_FILES_COUNT + GetDirectoryOffset () + GetHeaderIndex (1)]		= 0;
		memoryBlockA.data[SPACE_SIZE_FILES_COUNT + GetDirectoryOffset () + GetHeaderIndex (2)]		= 0;
		memoryBlockA.data[SPACE_SIZE_FILES_COUNT + GetDirectoryOffset () + GetHeaderIndex (3)]		= 0;
		memoryBlockA.data[SPACE_SIZE_FILES_COUNT + GetDirectoryOffset () + GetHeaderIndex (4) + 0]	= 0;
		memoryBlockA.data[SPACE_SIZE_FILES_COUNT + GetDirectoryOffset () + GetHeaderIndex (4) + 1]	= 0;
	}


	//void Reset () {
	//	Initialize (0, nullptr);
		//bufforIndex 			= GetInitialBufforIndex ();
		//bufforSizeNameIndex 	= GetHeaderOffset ();
		//bufforSizeContextIndex	= GetHeaderOffset () + SPACE_SIZE_NAME;
		// files
		// constants, imports, commands, queues
		// name, context
	//}
	

	void AddImport () {
		// ADD INFORMATION ABOUT INCLUSION OF ANOTHER FILE.
		memoryBlockA.data[INDEX_FILES_COUNT]++;

		// REPOSITION COUNT INDEXES.
		// Because we're adding 2 spaces after each assign for new name and context count variables we need to discard those and add up later.
		bufforIndex -= 2;

		fileIndex = bufforIndex;

		// ADD UP LATER
		bufforSizeNameIndex		= bufforIndex + GetHeaderIndex(4);
		bufforSizeContextIndex	= bufforIndex + GetHeaderIndex(4) + 1;

		// POINT CORRECTLY AT WRITABLE DATA.
		// add offset name_count & context_count
		bufforIndex += 2 + 4;
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