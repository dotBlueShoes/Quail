#pragma once
#include "../../Framework.hpp"

#include "Types.hpp"

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

namespace Commands::Open::ParsingStages {

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

    extern struct MainCommand currentMainCommand;
    extern struct SubCommand currentSubCommand;
    extern vector<struct MainCommand> mainCommands;

}