#pragma once
#include "../Framework.hpp"

#include "open/ParsingStages.hpp"
#include "open/Types.hpp"
#include "Tokens.hpp"

#include <iostream>
#include <fstream>

namespace Commands::Open {

    const array<char, 16> fileName = R"(project_data.txt)";
    const array<char, 6> commandConfigName = "config";
    const array<char, 4> commandListName = "list";

    extern array<char, 35> debugDataFilePath;
    extern uint64 dataFilePathLength;
    extern char* dataFilePath;

    // 1.open file.
    // 2.skip lines with '#' or '\n' as first character.
    // 3. when read different sign read till " {" is read.
    // 4. save only the part without it (as mainCommand).
    //  5. read lines by ' ',command_name,"=",command - loop till '}' is read.

    namespace IO {
        block CreateFilePath( IN wchar* filePath );
        block ReadConfigurationFile();
    } 

    namespace Pages {

        block IsCommandConfig (
            OUT uint8& collision,
            IN const uint8& commandSubLength,
            IN const char* const commandSub
        );

        block DisplayConfig ();

        block IsCommandListMainCommands (
            OUT uint8& collision,
            IN const uint8& commandSubLength,
            IN const char* const commandSub
        );

        block DisplayListMainCommands();

        block IsCommandListSubCommands (
            OUT uint8& collision,
            IN const uint8& commandSubLength,
            IN const char* const commandSub
        );

        block DisplayListSubCommands(
            IN MainCommand& commandMain
        );

    }

    block Initialize() {

        #if DEF_DEBUG

        dataFilePath = debugDataFilePath.Pointer();
        dataFilePathLength = 35;

        #elif DEF_RELEASE

        // Get exe path.
        _get_pgmptr(&dataFilePath); // wchar _get_pgmptr(); // char _get_wpgmptr(&dataFilePath);
        dataFilePathLength = strlen(dataFilePath);// wchar wcslen(dataFilePath);

        // Turncate leaving characters after '\0' sign.
        dataFilePathLength -= 9; // Quail.exe
        dataFilePath[dataFilePathLength] = '\0';

        #endif
    }
    
    callback Action ( Tokens::ActionArgs& );

}