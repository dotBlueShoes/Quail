#include "project/open/ParsingStages.hpp"

namespace Commands::Open::ParsingStages {

        struct SubCommand currentSubCommand { { '\0' }, CommandType::Normal, { '\0' } };
        struct MainCommand currentMainCommand { { '\0' }, {  } };
        vector<struct MainCommand> mainCommands;

        Stage currentStage = Main;
        uint8 bufforIndex = 0; // 0 element is set by Main Stage.
        char stashedCharacter;
        char* buffor;

        CommandName currentVariable;
        uint8 currentVariableIndex = 0;

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
            currentMainCommand.variables.clear();
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
                case SIGN_PIPE: {
                    currentSubCommand.type = CommandType::Pipe;
                    currentStage = SubCommandName;
                    //printf("\nc:%c\n", args.current);
                } break;

                case SIGN_VAR: {
                    //printf("Variable!");
                    currentSubCommand.type = CommandType::Pipe;
                    currentStage = VariableName;
                } break;

                default: {
                    // SETS
                    buffor[bufforIndex] = args.current;
                    ++bufforIndex;

                    currentSubCommand.type = CommandType::Normal;
                    currentStage = SubCommandName;
                }
            }
        }

        block VariableName(
            const StageArgs& args
        ) {
            switch (args.current) {

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

                    currentStage = VariableContext;
                } break;

                case ' ':
                case '\t':
                case '\n':
                case '\0':
                case '}': {
                    printf("%s", "Syntax Error expected '=' sign!");
                } break;

                default: {
                    buffor[bufforIndex] = args.current;
                    ++bufforIndex;
                }
            }
        }

        block VariableContext(
            const StageArgs& args
        ) {
            switch (args.current) {

                case '\0': {
                    printf("%s", "Syntax Error! '\0' cannot be placed inside variables!");
                } break;

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
                    currentMainCommand.variables.push_back(currentSubCommand);
                    currentStage = SubCommandEntry;
                } break;

                default: {
                    buffor[bufforIndex] = args.current;
                    ++bufforIndex;
                }
            }
        }

        block SubCommandName (
            const StageArgs& args
        ) {
            switch (args.current) {

                case ' ':
                case '\t':
                case '\n':
                case '\0':
                case '}': {
                    printf("%s", "Syntax Error expected '=' sign!");
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

                case SIGN_REPLACE_START: {
                    //printf("replace start");
                    currentStage = SubCommandReplace;
                } break;

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

        block SubCommandReplace (
            const StageArgs& args
        ) {
            switch (args.current) {
                case SIGN_REPLACE_END: {
                    //printf("\nreplace end");

                    // ACQUIRE
                    currentVariable[currentVariableIndex] = '\0';
                    //printf("\nvar:%s", currentVariable.Pointer());

                    //printf("\n");
					//for (auto& variable : currentMainCommand.variables) {
					//	printf("\n%s", variable.name.Pointer());
					//}
					//printf("\n%s", currentVariable.Pointer());

                    // FIND
                    uint8 foundIndex = NOT_FOUND;
                    Search::KnownLength<char>(
                        foundIndex,             								// OUT index in array
                        currentVariableIndex,									// IN length of 1 comperable
                        currentVariable.Pointer(),   							// IN data of 1 comperable
						currentMainCommand.variables.size(),					// IN array length of 2 comperables
						(const void**)(currentMainCommand.variables.data()),	// IN array data of 2 comperables
						sizeof(SubCommand), SUB_COMMAND_OFFSET									// Struct offset
                    );

					//1 // THIS WONT EVER TRIGGER DUE to KnownLength construction!
					//1 if (foundIndex == NOT_FOUND) {
					//1 	printf("%s", "Replace sign '@' detected but variable '$' was not found in the hierarchy!");
					//1 	currentStage = SubCommandContext;
					//1 }

					auto& replaceValue = currentMainCommand.variables[foundIndex].context;
					uint8 replaceLength = 0;

					// Get Variable Context Length.
            		for (; replaceValue[replaceLength] != '\0'; ++replaceLength);

					// EMPLACE FOUNDED STRING
					for (uint8 i = 0; i < replaceLength; ++i) {
                        buffor[bufforIndex + i] = currentMainCommand.variables[foundIndex].context[i];
                    }
					bufforIndex += replaceLength;

					// CLEAR
					currentVariableIndex = 0;
                    
                    // RETURN
                    currentStage = SubCommandContext;
                } break;

                default: {
                    currentVariable[currentVariableIndex] = args.current;
                    ++currentVariableIndex;

                    //printf("%c", args.current);
                }
            }
        }

    }