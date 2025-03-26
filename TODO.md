# 0.5

--- OTHER 

01. Examples:
- unity
- gimp
- windows terminal
- directory
02. gittag and rebuild and release
03. yt-video


# LATER


--- QUAIL

01. COMPARESEARCH::ArrayPartFirstMatchVector inside `open\main.hpp` reads outside memory!
	It does not check for EOS sign it simply goes through. Those string do have EOS.
	But when we're checking against a string thats longer we simply bypass it. (and it might memory allocated by us?)
02. DEBUG_FLAG_POSTLOGGING implementation - file/buffer logging
03. `--optimize`, `-x` to test the memory allocations, the amount of required data that is needed 
	then is being stored in Windows Registry and we reuse it as the first allocation for open command data.
04. proper styling display with -> registry keys and values.
	project-header, command-header, footer, line-format, line-size, key-padding-size
05. Adding Quail configuration data using --add command: like `quail -a module "Name" "ProjectPath"`.
06. Edit Quail configuration data using --edit command: like `quail -e module "Name" "ProjectPath"`.
07. Secrets vs Defines
08. Make it so global.txt values can be overridden
09. Make "_quail" variable to pass Quail directory - not sure if it's needed
10. ? Maybe create a roadmap using that github tool (tables).
11. New feature 'groups' eg. cmake_remove -> make it so user can match with "cma rem".
12. diffrentiate between topConfigsFolder and installedBinFolder
13. Make it so queues can reference prefious queues too.
14. Disable making projects in global.txt -> see how it works. Maybe if it works fine leave it.
15. Fix bug with files (main, global, include) being still in use. Its an edge case but it happends sometimes.

--- INSTALLER

01. Move created window objects outside WM_PAINT.
02. Last, Browse, Path, Components windows use loses focus of main window. making use of arrows imposible. Fix it.
03. Make the download and installing run on a separate Threat so that GUI wont lag.
04. Make installer display centered.
05. Give an option in installer to rename the batch file.
06. Distinguish data and view layer.

--- OTHER

01. Make script for automatic releases.
02. gittag and rebuild and release
