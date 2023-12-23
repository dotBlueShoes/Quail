#pragma once
#include "lib/Framework.hpp"

#include "Constants.hpp"
#include "Buffors.hpp"

#include "Projects/Stages.hpp"

namespace IO {

	/// READS the main projects config file. 
	block GetProjectsData () {
		
		{ /* WINDOWS ONLY */
			FILE* fileConfiguration = nullptr;
			//size readLength = 0;

			// IF file pointer return NULL - EXIT THE PROGRAM
			if ((fileConfiguration = _wfopen(FILE_PROJECTS.Pointer(), L"r")) == NULL) {
				printf ("%s\n", strings::STRING_FAILURE_NO_CONFIG_FILE);
				exit (ExitCode::FAILURE_NO_CONFIG_FILE);
			}

			// THIS NEEDS TO BE RETHINKED !
			// // Obtain file size and check with buffor.
  			// fseek (fileConfiguration ,0 ,SEEK_END);
  			// readLength = ftell (fileConfiguration);
  			// rewind (fileConfiguration);
			// 
			// 
			// // Check if our memory buffor doing fine.
			// if (readLength > memoryBlockA.length) {
			// 	printf ("%s\n", strings::STRING_FAILURE_BUFFOR_TO_SMALL);
			// 	exit (ExitCode::FAILURE_BUFFOR_TO_SMALL);
			// }


			int32 character = EOF;
			do { // The actuall read operation.
				character = fgetc (fileConfiguration);
				IO::Projects::Stages::Current();
				// Call to stages extended
			} while (character != EOF);
		}

	};

	/// READS the linked project config file.
	block GetProjectData ( 
		IN	wchar* filePath
	) {

		{ /* WINDOWS ONLY */
			FILE* fileConfiguration = nullptr;

			// IF file pointer return NULL - EXIT THE PROGRAM
			if ((fileConfiguration = _wfopen (filePath, L"r")) == NULL) {
				exit (ExitCode::FAILURE_NO_CONFIG_FILE);
			}

			int32 character = EOF;
			do { // The actuall read operation.
				character = fgetc (fileConfiguration);
				IO::Projects::Stages::Current();
				// Call to stages
			} while (character != EOF);
		}

	}

}