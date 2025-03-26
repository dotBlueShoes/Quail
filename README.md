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

# Software Notes
Why writing my own `Wizard` when WinApi provides a ready to use control with localization support?

[Official Windows Wizard Sample](https://github.com/microsoft/Windows-classic-samples/tree/263dd514ad215d0a40d1ec44b4df84b30ec11dcf/Samples/Win7Samples/winui/shell/appplatform/aerowizards/migratingtoaerowizardssdksample/wizard97)
  > My reasoning: It's a good learning example and it provides more flexibility for future custom optimalizations like darkmode support or making the window dpi-aware. Additionally the Wizard executable created is compressed which then competes well with `.msi` files, that usually weigh less.
