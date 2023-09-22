#include "project/Open.hpp"

#include "lib/Executable.hpp"

namespace Commands::Open {

    namespace IO {

        // Constructing Full FilePath
        //  In future we'll likely give an option to change their placement and name.
        getter uint64 GetFilePathLength() {
            return Executable::dataFilePathLength + fileName.Length() + 1;
        }

        block CreateFilePath (
            IN wchar* filePath
        ) {
            uint8 index = 0;
            uint8 i = 0;

            for (; index < Executable::dataFilePathLength; ++index)
                filePath[index] = Executable::dataFilePath[index];

            for (; i < fileName.Length(); ++i)
                filePath[index + i] = fileName[i];

            filePath[index + i] = '\0';
        }

        block ReadConfigurationFile() {

            wchar* filePath = new wchar[GetFilePathLength()];
            CreateFilePath(filePath);

            // Open file
            FILE* configurationFile;
            if ((configurationFile = _wfopen(filePath, L"r")) == NULL){
                // Program exits if the file pointer returns NULL.
                printf("ERROR! File could not be opened file");
                exit(ExitCode::FAILURE_NO_CONFIG_FILE); 
            }

            ParsingStages::StageArgs stageArgs { 0 };
            ParsingStages::buffor = new char[256];

            // The actuall read.
            while (stageArgs.current != EOF) {
                stageArgs.last = stageArgs.current;
                stageArgs.current = getc(configurationFile);
                ParsingStages::currentStage(stageArgs);
            }

            fclose(configurationFile);

            delete[] ParsingStages::buffor;
            delete[] filePath;
        }
    }
    

    block ExecutePipeCommand(
        const MainCommand& commandMain,
        const vector<char*>& commands
    ) {
        for (auto& command : commands) {
            uint8 commandLength = 0;
            uint8 commandSubIndex = 0;

            // Get Length. TODO: Ideally we should known that from substringing arleady.
            for (; command[commandLength] != '\0'; ++commandLength);

            // TODO: SEARCH ideally should be: 
            // 1. for multiple results.
            // 2. up to specified index so we don't check with itself and below it.
            // 3. allowing shourcuts.
                    
            // SUB COMMAND SEARCH
            Search::KnownLength<char>(
                commandSubIndex, 
                commandLength, command,
                commandMain.commands.size(), 
                (const void**)(commandMain.commands.data()),
                sizeof(SubCommand), SUB_COMMAND_OFFSET
            );

            std::system(commandMain.commands[commandSubIndex].context.Pointer());
        }
    }

    namespace Pages {

        block IsCommandConfig (
            OUT uint8& collision,
            IN const uint8& commandSubLength,
            IN const char* const commandSub
        ) {
            for (uint8 i = 0; i < commandSubLength; ++i) {
                collision += commandSub[i] == commandConfigName[i];
            }
        }

        block DisplayConfig() {  
            wchar* filePath = new wchar[IO::GetFilePathLength()];
            IO::CreateFilePath(filePath);

            // std::system replacement because potensial wide-character ! 
            _wsystem( filePath );

            delete[] filePath;

            exit(SUCCESSFULL_COMMAND_EXECUTION);
        }

        block IsCommandListMainCommands (
            OUT uint8& collision,
            IN const uint8& commandSubLength,
            IN const char* const commandSub
        ) {
            for (uint8 i = 0; i < commandSubLength; ++i) {
                collision += commandSub[i] == commandListName[i];
            }
        }

        block DisplayListMainCommands() { 
            printf("listing: %s\n", "Commands");
            for (uint8 i = 0; i < ParsingStages::mainCommands.size(); ++i) {
                auto& currentComamnd = ParsingStages::mainCommands[i];
                printf("\t%s\n", currentComamnd.name.Pointer());

                //for (uint8 j = 0; j < currentComamnd.commands.size(); ++j) {
                //    printf("\t%s: %s\n", currentComamnd.commands[j].name.Pointer(), currentComamnd.commands[j].context.Pointer());
                //}
            }

            exit(SUCCESSFULL_COMMAND_EXECUTION);
        }

        /* Collision is equal commandSubLength when true */
        block IsCommandListSubcommands (
            OUT uint8& collision,
            IN const uint8& commandSubLength,
            IN const char* const commandSub
        ) {
            for (uint8 i = 0; i < commandSubLength; ++i) {
                collision += commandSub[i] == commandListName[i];
            }
        }

        block DisplayListSubcommands(
            IN MainCommand& commandMain
        ) {  
            printf("listing: %s\n", commandMain.name.Pointer());
            for (uint8 i = 0; i < commandMain.commands.size(); ++i) {
                printf("\t%s: %s\n", commandMain.commands[i].name.Pointer(), commandMain.commands[i].context.Pointer());
            }

            exit(SUCCESSFULL_COMMAND_EXECUTION);
        }

    }

    block GetMainCommand (
        OUT uint8& commandMainNameLength,
        OUT uint8& commandMainIndex,
        IN const char* const commandMain
    ) {
            // MAIN COMMAND SEARCH
            Search::KnownLength<char>(
                commandMainIndex, 
                commandMainNameLength, commandMain,
                ParsingStages::mainCommands.size(), 
                (const void**)(ParsingStages::mainCommands.data()),
                sizeof(MainCommand), SUB_COMMAND_OFFSET
            );
    }


    callback Action ( Tokens::ActionArgs& actionArgs ) {
        DEBUG printf("DEBUG Entered action 'Open'\n");

        IO::ReadConfigurationFile();    // Get data from the config file.

        if (actionArgs.argumentsLength == 2)  // Was Project Comamnd specified? 
            Pages::DisplayListMainCommands(); // No - Display all defined in config.

        const char* commandMainName = actionArgs.arguments[2]; 
        uint8 commandMainNameLength = 0;
        
        commandMainNameLength = strlen(commandMainName); // Get Length.

        if (commandMainNameLength > COMMAND_NAME_LENGTH) { // Check Length.
            printf("%s", STRING_FAILURE_TO_LONG_COMMAND_NAME);
            exit(FAILURE_TO_LONG_COMMAND_NAME);
        }

        { // Check for `special` maincommands [not-defined-in-config] { list, config }
            using namespace Pages;
            uint8 collision = 0;

            IsCommandConfig(collision, commandMainNameLength, commandMainName);
            if (collision == commandMainNameLength)
                DisplayConfig();

            collision = 0;

            IsCommandListMainCommands(collision, commandMainNameLength, commandMainName);
            if (collision == commandMainNameLength)
                DisplayListMainCommands();
        }

        uint8 commandMainIndex = 0; // Position of matched MainCommand eg. project_name. 
        GetMainCommand(commandMainNameLength, commandMainIndex, commandMainName);
        auto& commandMain = ParsingStages::mainCommands[commandMainIndex]; // Matched MainCommand.

        // No subcommand eg. `quail -o [project_name]`
        if (actionArgs.argumentsLength == 3) {
            Pages::DisplayListSubcommands(commandMain);
            return;
        } 

        auto& commandSub = actionArgs.arguments[3];
        uint8 commandSubLength = 0;

        
        //#pragma GCC diagnostic push
        //#pragma GCC diagnostic ignored "-Wempty-body"
        commandSubLength = strlen(commandSub); // Get subcommand length.
        //#pragma GCC diagnostic pop

        { // Check for `special` subcommands [not-defined-in-config] { list }.
            using namespace Pages;
            uint8 collision = 0;

            IsCommandListSubcommands(collision, commandSubLength, commandSub);
            if (collision == commandSubLength)
                DisplayListSubcommands(commandMain);
        }

        uint8 commandSubIndex = 0;

        // SUB COMMAND SEARCH
        Search::KnownLength<char>(
            commandSubIndex, 
            commandSubLength, commandSub,
            commandMain.commands.size(), 
            (const void**)(commandMain.commands.data()),
            sizeof(SubCommand), SUB_COMMAND_OFFSET
        );

        auto& commandAction = commandMain.commands[commandSubIndex];

        switch (commandAction.type) {
            case Normal: {
                DEBUG printf("%s", "NORMAL COMMAND");
                std::system(commandAction.context.Pointer()); // Execute the command
            } break;

            case Pipe: {
                DEBUG printf("%s", "PIPE COMMAND");

                const auto& data = commandAction.context.Pointer();
                vector<char*> commandNames;

                { // Split to substrings.
                    const char* const delimeter = ",";
                    char *substring = strtok(data, delimeter);

                    do {
                        commandNames.push_back(substring);
                        substring = strtok(NULL, delimeter);
                    } while (substring != nullptr);
                }

                ExecutePipeCommand(commandMain, commandNames); // Execute the command

            } break;

            default: {
                DEBUG printf("%s", "ERROR: UNKNOWN TYPE SET");
                exit(FAILURE_UNKNOWN_COMMAND_TYPE);
            }
            
        }
        
    }

}