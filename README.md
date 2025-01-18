`Quail` is a Command Line Interface application build to help in project management. The idea is to create 'Alias' like commands that are stored within a project which can be then accessed with an ease.

# FEATURES

- Use shortcuts
  > Execute a command specified in a config file by refering to a user-defined command name instead.
- Write even shorter
  > Said user-defined command does not have to be fully written to be matched. Be aware Quail will always take the 1st match.
- Be modular
  > Supports identifing subprojects and allows for command grouping.
- Define constants
  > Variable-like utility to store strings and use them with different Constants or Commands.
- Use command queues
  > Run multiple defined commands as if it was a singular quail command.
- Easly display all commands & queues
  > Simply run `Quail` with a selected project or a subproject to display it's contents. 

# Examples

- To display commands and queues of main config
  > `quail -o`
- To execute a command or a queue
  > `quail -o [project_name] [command]`
- To display project's commands and queues
  > `quail -o [project_name]`
- To display subproject's commands and queues
  > `quail -o [project_name] [subproject_name]`
- To execute subproject's command or a queue
  > `quail -o [project_name] [subproject_name] [command]`
- To execute subproject subsubproject's command or a queue
  > `quail -o [project_name] [subproject_name] [subsubproject_name] [command]`

. . .
