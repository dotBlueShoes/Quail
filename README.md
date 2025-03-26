`Quail` is a Command Line Interface application build to help in project management. The idea is to create 'Alias' like commands that are stored within a project which can be then accessed with an ease.

# FEATURES

- Use shortcuts
  > Execute a command specified in a config file by referring to a user-defined command name instead.
- Write even shorter
  > Said user-defined command does not have to be fully written to be matched. Be aware Quail will always take the 1st match.
- Be modular
  > Supports identifying subprojects and allows for command grouping.
- Define constants
  > Variable-like utility to store strings and use them with different Constants or Commands.
- Use command queues
  > Run multiple defined commands & ***queues*** as if they were a singular quail command.
- Easily display all subprojects, commands & queues
  > Simply run `Quail` with a selected project or a subproject to display it's contents.

# Examples

For simplicity sake I'll refer to commands, queues, projects and subprojects here as `aliases`.

- To display aliases of main config
  > `quail -o`
- To execute an alias
  > `quail -o [project_name] [command]`
- To display project's aliases
  > `quail -o [project_name]`
- To display subproject's aliases
  > `quail -o [project_name] [subproject_name]`
- To execute subproject's alias
  > `quail -o [project_name] [subproject_name] [command]`
- To execute subproject subsubproject's alias
  > `quail -o [project_name] [subproject_name] [subsubproject_name] [command]`

. . .

## Linking a Project

1. Create a *quail-file* in your project directory. Name it, for example `.quail\quail.txt`.
2. This file now represents your project. Link it to your top-level configuration file with the following command:

- Add a project / module (absolute path)
  > `^+project = C:\Projects\project\ .quail\quail.txt`
- Add a project / module (relative path)
  > `^-subproject = subproject\ .quail\quail.txt`

> Note the space between `key`, `project_drectory` **and** `quail-file`. It is required as Quail predefines `%_name%` & `%_path%` constants for your use inside the *quail-file* of your project.

3. Add the following command to your project configuration file: `!directory = explorer.exe "%_path%"`.
4. Now try executing it via `quail -o project directory`.

## Including a file

1. Includes for now are always relative. Create a `include.txt` next to your *project-main-configuration-file*.
2. At the top of your *project-main-configuration-file*, write the following: `&include.txt`.
3. Now try to create an alias, or include another file or a project.

## Global configuration file

There are **2** different configuration files for **Quail** use:
- `config.txt` -> your top-level configuration file.
- `global.txt` -> your top-level global configuration file.

> **Quail** will first read the global file `global.txt` and all of its includes and keep it for use later. Then Quail reads only the last project / module, and it's includes in a tree of all the modules from the top-level config file `config.txt` down to your project / subproject. This makes referencing a variable from a parent project impossible if not via include. Thus, global files. Use them to define constants and aliases that you want to use in every module.


## More Examples

Where can I find some ready to use commands for software I might be using?
> Look into the `examples` directory.

# Software Notes
Why writing my own `Wizard` when WinApi provides a ready to use control with localization support?

[Official Windows Wizard Sample](https://github.com/microsoft/Windows-classic-samples/tree/263dd514ad215d0a40d1ec44b4df84b30ec11dcf/Samples/Win7Samples/winui/shell/appplatform/aerowizards/migratingtoaerowizardssdksample/wizard97)
  > My reasoning: It's a good learning example and it provides more flexibility for future custom optimalizations like darkmode support or making the window dpi-aware. Additionally the Wizard executable created is compressed which then competes well with `.msi` files, that usually weigh less.
