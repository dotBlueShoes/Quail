#pragma once
#include "../Framework.hpp"

#include "Tokens.hpp"

#include <iostream>
#include <fstream>

namespace Commands::Open {

    const uint8 COMMAND_CONTEXT_LENGTH ( 255 );
    const uint8 COMMAND_NAME_LENGTH ( 16 );

    using CommandContext = array<char, COMMAND_CONTEXT_LENGTH>;
    using CommandName = array<char, COMMAND_NAME_LENGTH>;

    enum CommandType : uint8 {
        Normal = 0,
        Pipe = 1,
        Variable = 2,
    };

    struct SubCommand {
        CommandName name; // 16 bytes + 8 bytes
        CommandType type;
        CommandContext context; // 128 bytes + 8 bytes
    };

    struct MainCommand {
        CommandName name;
        vector<SubCommand> commands; // the size of that thing could get read and optimized i guess.
    };

    const array<char, 16> fileName = R"(project_data.txt)";
    const array<char, 6> commandConfigName = "config";
    const array<char, 4> commandListName = "list";

    extern array<char, 35> debugDataFilePath;
    extern uint64 dataFilePathLength;
    extern char* dataFilePath;

    extern vector<MainCommand> mainCommands;

    namespace ParsingStages {

        // Following switch statement should change as
        //  we're reading different segments...
        // [ STATE 1 ]
        // When we've read '#' as first character we're entering [ STATE 2 ]
        // When we've read other character then [' ','\n','\t',EOF] we re entering [ STATE 3 ]
        // [ STATE 2 ]
        // We look for '\n' or 'EOF' so when '\n' is read we get back to [ STATE 1]
        // This way we skip lines that start with # symbol.
        // [ STATE 3 ]
        // read till we hit both " {" and save that as main-command enter [ STATE 4 ]
        // read till '}' is reached. read lines by ' ',command_name,"=",command.
        // when '}' is read enter [ STAGE 1 ]

        struct StageArgs {
            char current;
            char last;
        };

        using Stage = void (*)( const StageArgs& );

        block Main ( const StageArgs& );
        block Comment ( const StageArgs& );
        block MainCommand ( const StageArgs& );
        block MainCommandSpace ( const StageArgs& );
        block MainCommandEnd ( const StageArgs& );
        block SubCommandEntry ( const StageArgs& );
        block SubCommand ( const StageArgs& );
        block SubCommandContext ( const StageArgs& );

        extern Stage currentStage;
        extern uint8 bufforIndex;
        extern char stashedCharacter;
        extern char* buffor;

    }

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