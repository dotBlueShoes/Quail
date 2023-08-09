#include "project/Open.hpp"


namespace Project::Open {

    vector<MainCommand> mainCommands;

    SubCommand currentSubCommand { { '\0' }, CommandType::Normal, { '\0' } };
    MainCommand currentMainCommand { { '\0' }, {  } };

    template <class T>
    block KnownLengthSearch(
        OUT uint8& resultIndex,             // If found resultIndex points to found element if not stays unchainged.
        IN  const uint8& valuesLength,      // Length of string data we're compering/.
        IN  const T* values,                // String data we're compering.
        IN  const uint8& elementsLength,    // Length of elements to compare to.
        IN  const void** elements,          // Array on which we're compering.
        IN  const size& typeSize = 0,       // For variable in possible Struct position in bytes. 
        IN  const size& elementOffset = 0   // For variable in possible Struct position in bytes. 
        /* missing lambda for collision failed execution */
    ) {
        uint8 collision = 1;

        for (; resultIndex < elementsLength * collision; ++resultIndex) {
            for (uint8 i = 0; i < valuesLength; ++i) {
                //collision += (values[i] == (elements[resultIndex].name[i]);
                auto& element = ((T*)((byte*)(elements) + (typeSize * resultIndex) + elementOffset))[i];
                collision += (values[i] == element);
            }
            collision = ((collision - 1) != valuesLength);
        }

        if (collision) {
            printf("%s", "Fail No Matching Action!");
            return;
        }

        --resultIndex;
    }
    
    block ReadConfigurationFile() {

        // Constructing Full FilePath
        //  In future we'll likely give an option to change their placement and name.
        wchar* filePath = new wchar[dataFilePath.Length() + fileName.Length() + 2];
        
        {
            uint8 index = 0;
            uint8 i = 0;

            for (; index < dataFilePath.Length(); ++index)
                filePath[index] = dataFilePath[index];

            filePath[index] = L'/';
            ++index;

            for (; i < fileName.Length(); ++i)
                filePath[index + i] = fileName[i];

            filePath[index + i] = '\0';
        }

        // Open file
        FILE* configurationFile;
        if ((configurationFile = _wfopen(filePath, L"r")) == NULL){
            printf("Error! opening file");
            // Program exits if the file pointer returns NULL.
            exit(1);
        }

        // Read files length
        //fseek(configurationFile, 0L, SEEK_END); // GET TO THE FILE'S END
        //uint32 fileSize = ftell(configurationFile);
        //fseek(configurationFile, 0L, SEEK_SET); // GET TO THE FILE's BEGGINING

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

    block MatchCommandSearch ( 
        IN  const char* const commandName, 
        OUT uint8& commandMainNameLength,
        OUT uint8& commandIndex
    ) {

        // SEARCH
        //  all-compare known length search.

        // MAIN COMMAND SEARCH
        //KnownLengthSearch<char>(
        //    mainCommandIndex, 
        //    commandMainNameLength, commandMainName,
        //    mainCommands.size(), (const void**)(mainCommands.data()),
        //    sizeof(MainCommand), 8
        //);

        //DEBUG {
        //    printf("\nm: %s", mainCommands[mainCommandIndex].name.Pointer());
        //}

        // SUB COMMAND SEARCH
        //KnownLengthSearch<char>(
        //    subCommandIndex, 
        //    commandSubNameLength, commandSubName,
        //    mainCommands[mainCommandIndex].commands.size(), (const void**)(mainCommands[mainCommandIndex].commands.data()),
        //    sizeof(SubCommand), 8
        //);

        //DEBUG {
        //    printf("\nn: %s", mainCommands[mainCommandIndex].commands[subCommandIndex].name.Pointer());
        //    printf("\nc: %s", mainCommands[mainCommandIndex].commands[subCommandIndex].context.Pointer());
        //}
    }

    block MainCommandListPage() {

    }

    callback Action ( Tokens::ActionArgs& actionArgs ) {
        DEBUG std::cout << "DEBUG Entered action 'Open'\n";

        ReadConfigurationFile();

        if (actionArgs.argumentsLength == 2) {
            printf("%s", "Fail no project_name specified!");
            return;
        }

        auto& commandMainName = actionArgs.arguments[2];
        uint8 commandMainNameLength = 0;
        uint8 commandMainIndex = 0;

        { // GET MAIN COMMAND
            // Get Length.
            for (; commandMainName[commandMainNameLength] != '\0'; ++commandMainNameLength);

            // Check Length.
            if (commandMainNameLength > COMMAND_NAME_LENGTH) {
                printf("%s", "Fail commandMainNameLength cannot be more then COMMAND_NAME_LENGTH characters!");
                return;
            }

            // MAIN COMMAND SEARCH
            KnownLengthSearch<char>(
                commandMainIndex, 
                commandMainNameLength, commandMainName,
                mainCommands.size(), 
                (const void**)(mainCommands.data()),
                sizeof(MainCommand), 8
            );
        }

        // eg. No subcommand `quil -o [project_name]`
        if (actionArgs.argumentsLength == 3) {
            // In future it will display 'main' subcommand
            MainCommandListPage();
            return;
        } 

        { // MainCommandListPage() // collision allows for shorts which is ideal!
            const char commandListName[] = "list";

            auto& subcommand = actionArgs.arguments[3];
            uint8 subcommandLength = 0;
            uint8 collision = 0;

            // Get Length.
            for (; subcommand[subcommandLength] != '\0'; ++subcommandLength);

            for (uint8 i = 0; i < subcommandLength; ++i) {
                collision += subcommand[i] == commandListName[i];
            }

            if (collision == subcommandLength) {
                //printf("%s", "list!");
                auto& commands = mainCommands[commandMainIndex].commands;
                
                for (uint8 i = 0; i < commands.size(); ++i) {
                    printf("\t%s: %s\n", commands[i].name.Pointer(), commands[i].context.Pointer());
                }

                return;
            }
        }

        {   
            
            auto& commandSubName = actionArgs.arguments[3];
            uint8 commandSubIndex = 0;
            uint8 commandSubNameLength = 0;

            // Get Length.
            
            for (; commandSubName[commandSubNameLength] != '\0'; ++commandSubNameLength);

            // SUB COMMAND SEARCH
            KnownLengthSearch<char>(
                commandSubIndex, 
                commandSubNameLength, commandSubName,
                mainCommands[commandMainIndex].commands.size(), 
                (const void**)(mainCommands[commandMainIndex].commands.data()),
                sizeof(SubCommand), 8
            );

            // Execute the command
            auto& commandMain = mainCommands[commandMainIndex];
            auto& commandAction = commandMain.commands[commandSubIndex];

            switch (commandAction.type) {
                case Normal: {
                    DEBUG printf("%s", "NORMAL COMMAND");
                    std::system(commandAction.context.Pointer());
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

                    for (auto& commandName : commandNames) {
                        //printf("\n%s", commandName);

                        uint8 commandNameLength = 0;
                        uint8 commandSubIndex = 0;

                        // Get Length. TODO: Ideally we should known that from substringing arleady.
                        for (; commandName[commandNameLength] != '\0'; ++commandNameLength);

                        // TODO: SEARCH ideally should be: 
                        // 1. for multiple results.
                        // 2. up to specified index so we don't check with itself and below it.
                        // 3. allowing shourcuts.
                        
                        // SUB COMMAND SEARCH
                        KnownLengthSearch<char>(
                            commandSubIndex, 
                            commandNameLength, commandName,
                            commandMain.commands.size(), 
                            (const void**)(commandMain.commands.data()),
                            sizeof(SubCommand), 8
                        );

                        //printf("\n%s", commandMain.commands[commandSubIndex].context.Pointer());
                        std::system(commandMain.commands[commandSubIndex].context.Pointer());
                    }

                } break;

                default: {
                    DEBUG printf("%s", "ERROR: UNKNOWN TYPE SET");
                }
            }
            
        }
        
    }


    namespace ParsingStages {

        Stage currentStage = Main;
        uint8 bufforIndex = 1; // 0 element is set by Main Stage.
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
            //printf("%s", buffor);

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