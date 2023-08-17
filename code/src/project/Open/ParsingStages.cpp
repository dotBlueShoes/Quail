#include "project/open/ParsingStages.hpp"

namespace Commands::Open::ParsingStages {

        struct SubCommand currentSubCommand { { '\0' }, CommandType::Normal, { '\0' } };
        struct MainCommand currentMainCommand { { '\0' }, {  } };
        vector<struct MainCommand> mainCommands;

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