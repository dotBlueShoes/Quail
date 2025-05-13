# FUTURE

--- NOTES

01. Working on Path env variable remember to change values for both installer and uninstaller!

--- QUAIL

00. BUGS

	01. Not dangerous. COMPARESEARCH::ArrayPartFirstMatchVector inside `open\open.hpp` reads outside it's memory. 
		Still within my program scope. When we're checking against a string with a key that has more characters we are
		bypassing the EOS sign and reading outside of the variable memory (possibly, ... its std::vector).
	02. Not dangerous. Configuration files (main, global, include, +project files) being still in use. 
		Its an edge case but it happens sometimes. Making uninstaller not uninstalling everything properly.
    03. include space in path bug fix via "" or `` or ''.
    04. queue `,` bug fix.

01. Have a better look at: Making projects in global.txt. There might be potential bugs and unchecked errors.
02. Differentiate between "topConfigsFolder" and "installedBinaryFolder". This will help in making 
	top-level-configs located outside Quail directory.
03. Make it so global config values can be overridden by normal config values.
04. Customizable styling for command display -> registry keys and values.
05. Adding Quail configuration data using --add command: like `quail -a module key "value"`.
06. Edit Quail configuration data using --edit command: like `quail -e module key "value"`.
07. `--optimize`, `-x` to test the memory allocations, the amount of required data that is needed 
	Store that in Windows Registry and we reuse it as the first allocation size the open buffer.
08. Replace std::vector's with one buffer only.
09. Quail setting - coloring.
10. Quail setting - spacing - registry value.
11. Quail setting - spacing - 0 means no limiting.
12. ???. Maybe directory as `-d` or `--directory` to open directory of a defined project.
13. Replace "IsForceC8Display" with flags variable.
14. Add a `--show`, `-s` with an allias. It prints the WHOLE command-value in one line just like version is.
15. Add HIDDEN - new feature 'tags' now befors '=' sign a tag can be created. Tags can be defined by quail itself or by the user (but thats a far future). `!cmd HIDDEN = start "" "something"`. HIDDEN tag makes the command or queue not visible in view.
16. Add GROUP, MODULE, VARIABLE, SPECIAL - new feature 'tags' for module syntax. This highlights text according to tag.
17. Argument passing via "" or `` or ''. Detect parenthasis and remove it from quail processing but add it to the command parsed at the very end.
18. ? Quail HotKey via RegisterHotKey() with either MOD_ALT or MOD_CONTROL. This would run the quail -o <rest_of_the_command>. However an input box app would be neccessery for providing the rest_of_the_command.
19. Argument passing for queues. `quail -o queue "arg, arg arg 'value'"`. ',' means the next string belongs to next command in queue.
20. .qtf - quail text file, give an option to use such extension reqistring in as openable via notepad/++. This would allow an easier way of identifing quail files maybe even support creating syntax highlight for them in future.

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
04. Provide information on how to build quail without quail.

--- OTHER

01. Make script for automatic releases.
02. Examples:
- gimp
- blender
03. Git tag and rebuild and release
04. Create a roadmap
05. DEBUG_FLAG_POSTLOGGING implementation - file/buffer logging


# REMOVED PLANS

01. New type Secrets / Defines - How to tell the new user what specific constant he is missing?
> Instead provide a system for dimming highlight meaning that a constant is missing. This also means that no error have to be thrown during that operation as it is doing now.

02. New feature 'groups' e.g. "cmake_remove" -> make it so user can match with "cma rem".
> Instead provide new feature `tags`. tagging a module will force displaying it in a different color. Tags are: GROUP, MODULE, VARIABLE. Syntax: `^-name GROUP = location`.
