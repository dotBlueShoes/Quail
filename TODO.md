# FUTURE

--- QUAIL

00. BUGS

	01. Not dangerous. COMPARESEARCH::ArrayPartFirstMatchVector inside `open\open.hpp` reads outside it's memory. 
		Still within my program scope. When we're checking against a string with a key that has more characters we are
		bypassing the EOS sign and reading outside of the variable memory (possibly, ... its std::vector).
	02. Not dangerous. Configuration files (main, global, include) being still in use. 
		Its an edge case but it happens sometimes. Making uninstaller not uninstalling everything properly.

01. Have a better look at: Making projects in global.txt. There might be potential bugs and unchecked errors.
02. Differentiate between "topConfigsFolder" and "installedBinaryFolder". This will help in making 
	top-level-configs located outside Quail directory.
03. Make "%_quail%" variable to tell location of Quail top-level-configs directory. 
04. Make it so global config values can be overridden by normal config values.
05. New type Secrets / Defines - Or how to tell the new user what specific constant he is missing should point to?
06. New feature 'groups' e.g. "cmake_remove" -> make it so user can match with "cma rem".
07. Customizable styling for command display -> registry keys and values.
08. Adding Quail configuration data using --add command: like `quail -a module key "value"`.
09. Edit Quail configuration data using --edit command: like `quail -e module key "value"`.
10. `--optimize`, `-x` to test the memory allocations, the amount of required data that is needed 
	Store that in Windows Registry and we reuse it as the first allocation size the open buffer.
11. Replace std::vector's with one buffer only.
12. Quail setting - coloring.
13. Quail setting - spacing.
14. ???. Maybe directory as `-d` or `--directory` to open directory of a defined project.
15. Replace "IsForceC8Display" with flags variable.

--- INSTALLER

01. Move created window objects outside WM_PAINT.
02. Last, Browse, Path, Components windows use loses focus of main window. making use of arrows impossible. Fix it.
03. Make the download and installing run on a separate Threat so that GUI wont lag.
04. Make installer display centered.
05. Give an option in installer to rename the batch file.
06. Distinguish data and view layer.
07. Double check for folder creation.
08. BUG. Directory path that is being displayed at confirmation sometimes goes off screen.
09. Shared icon for installer and uninstaller with a pc in the back.

--- UNINSTALLER

01. When folder is being opened during deinstalation it wont remove itself. 
02. Shared icon for installer and uninstaller with a pc in the back.

--- GITHUB

01. More info on Global files.
02. Give info on shell changing and that by default it is batch
03. Add a new build to github with "--settings" feature.

--- OTHER

01. Make script for automatic releases.
02. Examples:
- gimp
- blender
03. Create a changelog.txt
04. Git tag and rebuild and release
05. Create a roadmap
06. DEBUG_FLAG_POSTLOGGING implementation - file/buffer logging
