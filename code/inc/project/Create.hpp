#pragma once
#include "../Framework.hpp"

//  quail -c template "path"
//  quail -c template config_name 

// D:\ProgramFiles\dotBlueShoes\quail\config

namespace Commands::Create {

    #define STRING_FAILURE_TO_LONG_COMMAND_TEMPLATE_NAME "Fail commandMainNameLength cannot be more then COMMAND_TEMPLATE_NAME characters!"

    const uint8 COMMAND_TEMPLATE_NAME_LENGTH ( 64 );
    const char COMMAND_TEMPLATE_PATH_SIGN ( '"' );

    array<char, 9> command_template { "template" };


    namespace IO {

        // Get all files /// nope
        //  instead for future note.. lets do it this way.
        // inside .quail are 3 folders:
        //  - configs
        //  - projects
        //  - templates

        // Quail during instalation can be set to diffrent places.
        //  during installation we set paths for both exe and sources/documents (configs)
        // 1st we set the installation path with wizard.
        // 2nd we set the sources location
        //  - A) placed where executable is being placed (no registry key requaired)
        //  - B) placed in documents directory (registry key)
        //  - C) custom location

        // inside "configs" catalog we have both projects_data.txt and templates_data.txt
        // these follow the style of exsisting markup but only store the following:
        //  "name":"path_to_config_in_filesystem" and comments

        // "projects" is the default location for new projects configurations
        // here only the commands connected to the specified project are.
        // listing of all projects goes through the "configs/projects_data.txt" file read process.
        // listing of a specified porject goes through "project/project_data.txt" file read process.

        // "templates" is the default location for new templates
        // templates and projects can be located somewhere else.
        // their location can be simply added to configs/templates_data.txt" and it would work.
        
        // With such composition we will never be parsing paths to quail as arguments
        //  but only command names specified in selected files.

    }


	callback Action ( Tokens::ActionArgs& actionArgs ) {
        DEBUG printf("Entered action 'Create'\n");

        if (actionArgs.argumentsLength == 2) // Was Project Template specified? 
            DEBUG printf("Template Was not specified !");
            exit(0);
        
        const char* commandMainName = actionArgs.arguments[2]; 
        uint8 commandMainNameLength = 0;

        printf("Template Name: %s", commandMainName);

        if (commandMainName[0] == COMMAND_TEMPLATE_PATH_SIGN) { // Is it path? e.g. "path/to/a"
            DEBUG printf("Passed unsoported yet path type instead template name!");
            exit(0);
        }

        commandMainNameLength = strlen(commandMainName); // Get Length.

        if (commandMainNameLength > COMMAND_TEMPLATE_NAME_LENGTH) { // Check Length.
            printf("%s", STRING_FAILURE_TO_LONG_COMMAND_TEMPLATE_NAME);
            exit(FAILURE_TO_LONG_COMMAND_NAME);
        }
        

    }

}