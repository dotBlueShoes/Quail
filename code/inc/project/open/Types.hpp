#pragma once
#include "../../Framework.hpp"

namespace Commands::Open { 

    #define STRING_FAILURE_TO_LONG_COMMAND_NAME "Fail commandMainNameLength cannot be more then COMMAND_NAME_LENGTH characters!"

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
    
}