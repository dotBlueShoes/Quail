#pragma once
#include "../../Framework.hpp"

namespace Commands::Open { 

    #define STRING_FAILURE_TO_LONG_COMMAND_NAME "Fail commandMainNameLength cannot be more then COMMAND_NAME_LENGTH characters!"

    const uint8 NOT_FOUND ( 0 ); // Must be 0 for KnownSearch to work!
    const uint8 SUB_COMMAND_OFFSET ( 8 ); // CommandName name; // 16 bytes + 8 bytes -> uint64 type

    const uint8 COMMAND_CONTEXT_LENGTH ( 255 ); // 254 + null-termination
    const uint8 COMMAND_NAME_LENGTH ( 17 ); // 16 + null-termination

    const char SIGN_PIPE ( '|' );
    const char SIGN_VAR ( '$' );
    const char SIGN_REPLACE_START ( '@' );
    const char SIGN_REPLACE_END ( '@' );
	const char SIGN_SPACE ( ' ' );

    using CommandContext = array<char, COMMAND_CONTEXT_LENGTH>;
    using CommandName = array<char, COMMAND_NAME_LENGTH>;

	const char SS = SIGN_SPACE; 
	const char OFFSET_LOOKUP_TABLE[16][COMMAND_NAME_LENGTH - 1] {
		{ SS, SS, SS, SS, SS, SS, SS, SS, SS, SS, SS, SS, SS, SS, SS, 0 }, 
		{ SS, SS, SS, SS, SS, SS, SS, SS, SS, SS, SS, SS, SS, SS, 0, 0 }, 
		{ SS, SS, SS, SS, SS, SS, SS, SS, SS, SS, SS, SS, SS, 0, 0, 0 }, 
		{ SS, SS, SS, SS, SS, SS, SS, SS, SS, SS, SS, SS, 0, 0, 0, 0 }, 
		{ SS, SS, SS, SS, SS, SS, SS, SS, SS, SS, SS, 0, 0, 0, 0, 0 }, 
		{ SS, SS, SS, SS, SS, SS, SS, SS, SS, SS, 0, 0, 0, 0, 0, 0 }, 
		{ SS, SS, SS, SS, SS, SS, SS, SS, SS, 0, 0, 0, 0, 0, 0, 0 }, 
		{ SS, SS, SS, SS, SS, SS, SS, SS, 0, 0, 0, 0, 0, 0, 0, 0 }, 
		{ SS, SS, SS, SS, SS, SS, SS, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, 
		{ SS, SS, SS, SS, SS, SS, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, 
		{ SS, SS, SS, SS, SS, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, 
		{ SS, SS, SS, SS, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, 
		{ SS, SS, SS, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, 
		{ SS, SS, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, 
		{ SS, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, 
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
	};

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
        vector<SubCommand> variables;
    };
    
}