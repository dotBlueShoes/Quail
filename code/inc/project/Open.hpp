#pragma once
#include "../Framework.hpp"

#include "Tokens.hpp"

#include <iostream>
#include <fstream>

namespace Commands::Open {

    const uint8 COMMAND_NAME_LENGTH ( 16 );
    const uint8 COMMAND_CONTEXT_LENGTH ( 255 );

    using CommandName = array<char, COMMAND_NAME_LENGTH>;
    using CommandContext = array<char, COMMAND_CONTEXT_LENGTH>;

    enum CommandType : uint8 {
        Normal = 0,
        Pipe = 1,
    };

    struct SubCommand {
        CommandName name; // 16 bytes + 8 bytes
        CommandType type;
        CommandContext context; // 128 bytes + 8 bytes
    };

    //struct PipedCommand {
    //    CommandName name;
    //    vector<CommandContext> commands; 
    //};

    struct MainCommand {
        CommandName name;
        vector<SubCommand> commands; // the size of that thing could get read and optimized i guess.
    };

    
    //const array<wchar, 26> dataFilePath = LR"(D:\Storage\Projects\.quail)";
    const array<wchar, 34> dataFilePath = LR"(D:\ProgramFiles\dotBlueShoes\quail)";
    const array<wchar, 16> fileName = LR"(project_data.txt)";

    const array<char, 6> commandConfigName = "config";
    const array<char, 4> commandListName = "list";

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
        block IsCommandList (
            OUT uint8& collision,
            IN const uint8& commandSubLength,
            IN const char* const commandSub
        );

        block DisplayList(
            IN MainCommand& commandMain
        );

        block IsCommandConfig (
            OUT uint8& collision,
            IN const uint8& commandSubLength,
            IN const char* const commandSub
        );

        block DisplayList(
            IN MainCommand& commandMain
        );
    }
    
    callback Action ( Tokens::ActionArgs& );

}