#include "project/Open.hpp"


namespace Commands::Open {

    vector<MainCommand> mainCommands;

    SubCommand currentSubCommand { { '\0' }, CommandType::Normal, { '\0' } };
    MainCommand currentMainCommand { { '\0' }, {  } };


    namespace IO {

        // Constructing Full FilePath
        //  In future we'll likely give an option to change their placement and name.
        // ... maybe not tho ...
        getter uint64 GetFilePathLength() {
            return dataFilePath.Length() + fileName.Length() + 1;
        }

        block CreateFilePath (
            IN wchar* filePath
        ) {
            uint8 index = 0;
            uint8 i = 0;

            for (; index < dataFilePath.Length(); ++index)
                filePath[index] = dataFilePath[index];

            //filePath[index] = L'/';
            //++index;

            for (; i < fileName.Length(); ++i)
                filePath[index + i] = fileName[i];

            filePath[index + i] = '\0';
            //printf("%ls", filePath);
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
                sizeof(SubCommand), 8
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
            for (uint8 i = 0; i < mainCommands.size(); ++i) {
                auto& currentComamnd = mainCommands[i];
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
                mainCommands.size(), 
                (const void**)(mainCommands.data()),
                sizeof(MainCommand), 8
            );
    }

    callback Action ( Tokens::ActionArgs& actionArgs ) {
        DEBUG std::cout << "DEBUG Entered action 'Open'\n";

        IO::ReadConfigurationFile();

        if (actionArgs.argumentsLength == 2) {
            Pages::DisplayListMainCommands();
            //printf("%s", "Fail no project_name specified!");
            //exit(ExitCode::FAILURE_PROJECT_NAME_NOT_SPECIFIED);
        }

        const char* commandMainName = actionArgs.arguments[2];
        uint8 commandMainNameLength = 0;
        uint8 commandMainIndex = 0;

        // Get Length.
        for (; commandMainName[commandMainNameLength] != '\0'; ++commandMainNameLength);

        // Check Length.
        if (commandMainNameLength > COMMAND_NAME_LENGTH) {
            printf("%s", "Fail commandMainNameLength cannot be more then COMMAND_NAME_LENGTH characters!");
            exit(FAILURE_TO_LONG_COMMAND_NAME);
        }

        { // Check for `special` maincommands [not-defined-in-config]
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

        GetMainCommand(commandMainNameLength, commandMainIndex, commandMainName);
        auto& commandMain = mainCommands[commandMainIndex];

        // No subcommand eg. `quail -o [project_name]`
        if (actionArgs.argumentsLength == 3) {
            Pages::DisplayListSubcommands(commandMain);
            return;
        } 

        auto& commandSub = actionArgs.arguments[3];
        uint8 commandSubLength = 0;

        // Get subcommand length.
        #pragma GCC diagnostic push
        #pragma GCC diagnostic ignored "-Wempty-body"
        for (; commandSub[commandSubLength] != '\0'; ++commandSubLength);
        #pragma GCC diagnostic pop

        { // Check for `special` subcommands [not-defined-in-config]
          // currently only "list"
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
            mainCommands[commandMainIndex].commands.size(), 
            (const void**)(mainCommands[commandMainIndex].commands.data()),
            sizeof(SubCommand), 8
        );

        auto& commandAction = mainCommands[commandMainIndex].commands[commandSubIndex];

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

                ExecutePipeCommand(mainCommands[commandMainIndex], commandNames); // Execute the command

            } break;

            default: {
                DEBUG printf("%s", "ERROR: UNKNOWN TYPE SET");
                exit(FAILURE_UNKNOWN_COMMAND_TYPE);
            }
            
        }
        
    }


    namespace ParsingStages {

        Stage currentStage = Main;
        uint8 bufforIndex = 0; // 0 element is set by Main Stage.
        char stashedCharacter;
        char* buffor;

        block Main (
            const StageArgs& args
        ) {
            switch (args.current) {
                case '#': {
                    currentStage = Comment;
                } break;
                case '\n': {
                    //printf("\n");
                } break;
                case ' ': {
                    //printf("s");
                } break;
                case '\t': {
                    //printf("s");
                } break;
                case EOF: { // '-1'
                    DEBUG printf("%s", "\n0");
                } break;
                default: {
                    buffor[0] = args.current;
                    ++bufforIndex;
                    //printf("%c", buffor[0]);
                    currentStage = MainCommand;
                }
            }
        }

        block Comment (
            const StageArgs& args
        ) {
            switch (args.current) {
                case '\n': {
                    currentStage = Main;
                } break;
                default: {
                    //printf("c");
                }
            }
        }

        block MainCommand (
            const StageArgs& args
        ) {
            switch (args.current) {
                case ' ': {
                    stashedCharacter = ' ';
                    currentStage = MainCommandSpace;
                    //printf("s");
                } break;
                case '{': {
                    //printf("{");
                } break;
                case '}': {
                    //printf("}");
                } break;
                case '\n': {
                    
                } break;
                case EOF: { // '-1'
                    //printf("0");
                } break;
                default: {
                    buffor[bufforIndex] = args.current;
                    ++bufforIndex;
                    //printf("c");
                }
            }
        }

        // To detect the " {" and act acordingly if the space occurs or not.
        // what about well known ["nazwa"] do i commit to the [""] ?
        block MainCommandSpace (
            const StageArgs& args
        ) {
            switch (args.current) {
                case ' ': {
                    stashedCharacter = ' ';
                    //printf("s"); // continue
                } break;
                case '\t': {
                    stashedCharacter = '\t';
                    //printf("s"); // continue
                } break;
                case '\n': {
                    stashedCharacter = '\n';
                    //printf("s"); // continue
                } break;
                case '{': {
                    if (stashedCharacter == ' ' || stashedCharacter == '\t' || stashedCharacter == '\n') {
                        currentStage = MainCommandEnd;
                    }
                } break;
                default: {
                    //printf("%s", "Syntax Error!");
                    // Ending up here means we hit a character again therefore the name had space in it!
                    currentStage = MainCommand;
                    buffor[bufforIndex] = stashedCharacter;
                    ++bufforIndex;
                    buffor[bufforIndex] = args.current;
                    ++bufforIndex;
                } break;
            }
        }

        block MainCommandEnd (
            const StageArgs& args
        ) {
            // ACQUIRE
            currentMainCommand.commands.clear();
            currentMainCommand.name[bufforIndex] = '\0';
            buffor[bufforIndex] = '\0';
            //printf("\n");
            //printf("\n%s", buffor);

            // CLEAR
            for (uint8 i = 0; i < bufforIndex; ++i) {
                currentMainCommand.name[i] = buffor[i];
                buffor[i] = 0;
            }
            bufforIndex = 0;

            //printf("%s", currentMainCommand.name.Pointer()); // SET
            currentStage = SubCommandEntry;
        }

        block SubCommandEntry (
            const StageArgs& args
        ) {
            switch (args.current) {
                case ' ':
                case '\t':
                case '\n':
                    break;

                case '}': {
                    mainCommands.push_back(currentMainCommand);
                    currentStage = Main;
                } break;

                // PIPE COMMAND
                case '$': {
                    currentSubCommand.type = CommandType::Pipe;
                    currentStage = SubCommand;
                    //printf("\nc:%c\n", args.current);
                } break;

                default: {
                    // SETS
                    buffor[bufforIndex] = args.current;
                    ++bufforIndex;

                    currentSubCommand.type = CommandType::Normal;
                    currentStage = SubCommand;
                }
            }
        }

        block SubCommand (
            const StageArgs& args
        ) {
            switch (args.current) {

                case ' ':
                case '\t':
                case '\n':
                case '\0':
                case '}': {
                    printf("%s", "Syntax Error!");
                } break;

                case '=': {
                    // ACQUIRE
                    //printf("%c", '\n');
                    buffor[bufforIndex] = '\0';
                    currentSubCommand.name[bufforIndex] = '\0';

                    // CLEAR
                    for (uint8 i = 0; i < bufforIndex; ++i) {
                        currentSubCommand.name[i] = buffor[i];
                        buffor[i] = 0;
                    }
                    bufforIndex = 0;

                    //printf("%s", currentSubCommand.name.Pointer());

                    currentStage = SubCommandContext;
                } break;

                default: {
                    buffor[bufforIndex] = args.current;
                    //printf("%c", args.current);
                    ++bufforIndex;
                }
            }
        }

        block SubCommandContext (
            const StageArgs& args
        ) {
            switch (args.current) {
                case '\n': {
                    // ACQUIRE
                    buffor[bufforIndex] = '\0';
                    //printf("\n");
                    //printf("%s", buffor);

                    currentSubCommand.context[bufforIndex] = '\0';

                    // CLEAR
                    for (uint8 i = 0; i < bufforIndex; ++i) {
                        currentSubCommand.context[i] = buffor[i];
                        buffor[i] = 0;
                    }
                    bufforIndex = 0;

                    //printf("%s", currentSubCommand.context.Pointer());
                    currentMainCommand.commands.push_back(currentSubCommand);
                    currentStage = SubCommandEntry;
                } break;
                default: {
                    buffor[bufforIndex] = args.current;
                    ++bufforIndex;
                }
            }
        }

    }

}