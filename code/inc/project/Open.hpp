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

    block Initialize();
    
    callback Action ( Tokens::ActionArgs& );

}