#include "project/Open.hpp"


namespace Project::Open {

    vector<MainCommand> mainCommands;

    SubCommand currentSubCommand { { '\0' }, { '\0' } };
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


    callback Action ( Tokens::ActionArgs& actionArgs ) {
        DEBUG std::cout << "Entered action 'Open'\n";

        //std::system("ls -l >test.txt");
        //char command[] = R"(wt -w 1 nt --title Quail -d "D:/Storage/Projects/Quail")";
        //char command[] = R"(explorer.exe "D:/Storage/Projects/Quail")";
        //char command[] = R"(call "D:/Storage/Projects/Quail/quail.npp")";
        //char command[] = R"(call "D:\Storage/Projects/Quail\quail_vsc.lnk")";
        //std::system(command);

        ReadConfigurationFile();

        //for (auto& mainCommand : mainCommands) {
        //    //printf("%c", '\n');
        //    printf("%s", "\nM: ");
        //    printf("%s", mainCommand.name.Pointer());
        //    for (auto& subCommand : mainCommand.commands) {
        //        //printf("%c", '\n');
        //        printf("%s", "\nN: ");
        //        printf("%s", subCommand.name.Pointer());
        //        printf("%s", "\nC: ");
        //        printf("%s", subCommand.context.Pointer());
        //    }
        //}

        // I cannot do a length search here...
        //  instead i should do the easiest.
        //  all-compare search.

        auto& commandProjectName = actionArgs.arguments[2];
        uint8 commandProjectNameLength = 0;

        auto& commandProjectAction = actionArgs.arguments[3];
        uint8 commandProjectActionLength = 0;
        

        uint8 mainCommandIndex = 0;
        uint8 subCommandIndex = 0;
        //uint8 collision = 1;

        // Get Length.
        for (; commandProjectName[commandProjectNameLength] != '\0'; ++commandProjectNameLength);
        for (; commandProjectAction[commandProjectActionLength] != '\0'; ++commandProjectActionLength);

        // Check Length.
        if (commandProjectNameLength > COMMAND_NAME_LENGTH) {
            printf("%s", "Fail CommandProjectNameLength cannot be more then COMMAND_NAME_LENGTH characters!");
            return;
        }

        //printf("%s", "\nproject: ");
        //printf("%s", commandProjectName);
        //printf("%i", commandProjectNameLength);
        //printf("%i", mainCommands[0].name.Length());
        
        // main commands have fixed size therefore no need to check for which is bigger.
        // collision value is equal to commandProjectNameLength when both strins are in fact equal.
        //  however loop will loop as long as collision is equal 1 therefore
        //  we're iterating with collision that by default is equal 1.
        //  inside the loop it is growing one by one and only is set to 0 when we find the match.
        //  therefore there are some extra processing to make it all working and be optimized.
        
        //for (; mainCommandIndex < mainCommands.size() * collision; ++mainCommandIndex) {
        //    for (uint8 c = 0; c < commandProjectNameLength; ++c) {
        //        collision += (commandProjectName[c] == mainCommands[mainCommandIndex].name[c]);
        //    }
        //    collision = ((collision - 1) != commandProjectNameLength);
        //}
        //
        //if (collision) {
        //    printf("%s", "Fail No Matching Project!");
        //    return;
        //}
        //
        //--mainCommandIndex;

        KnownLengthSearch<char>(
            mainCommandIndex, 
            commandProjectNameLength, commandProjectName,
            mainCommands.size(), (const void**)(mainCommands.data()),
            sizeof(MainCommand), 8
        );

        DEBUG {
            printf("\nm: %s", mainCommands[mainCommandIndex].name.Pointer());
        }
        

        // now the same but for subcommands...
        //collision = 1;

        KnownLengthSearch<char>(
            subCommandIndex, 
            commandProjectActionLength, commandProjectAction,
            mainCommands[mainCommandIndex].commands.size(), (const void**)(mainCommands[mainCommandIndex].commands.data()),
            sizeof(SubCommand), 8
        );
        
        //for (; subCommandIndex < mainCommands[mainCommandIndex].commands.size() * collision; ++subCommandIndex) {
        //    for (uint8 c = 0; c < commandProjectActionLength; ++c) {
        //        collision += (commandProjectAction[c] == mainCommands[mainCommandIndex].commands[subCommandIndex].name[c]);
        //    }
        //    collision = ((collision - 1) != commandProjectActionLength);
        //}
        //
        //if (collision) {
        //    printf("%s", "Fail No Matching Action!");
        //    return;
        //}
        //
        //--subCommandIndex;

        DEBUG {
            printf("\nn: %s", mainCommands[mainCommandIndex].commands[subCommandIndex].name.Pointer());
            printf("\nc: %s", mainCommands[mainCommandIndex].commands[subCommandIndex].context.Pointer());
        }
        
        // Execute the command
        std::system(mainCommands[mainCommandIndex].commands[subCommandIndex].context.Pointer());
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
                    printf("%s", "\n0");
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
            bufforIndex = 1;

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

                default: {
                    // CLEAR
                    //for (uint8 i = 1; i < bufforIndex; ++i) {
                    //    buffor[bufforIndex] = 0;
                    //}
                    //bufforIndex = 1;

                    // SETS
                    buffor[0] = args.current;
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
                    buffor[bufforIndex] = '\0';
                    //printf("\n");
                    //printf("%s", buffor);

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
                    bufforIndex = 1;

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