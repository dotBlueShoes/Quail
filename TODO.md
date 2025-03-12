01. COMPARESEARCH::ArrayPartFirstMatchVector inside `open\main.hpp` reads outside memory!
	It does not check for EOS sign it simply goes through. Those string do have EOS.
	But when we're checking against a string thats longer we simply bypass it. (and it might memory allocated by us?)
04. Clocks (debug utility to check times of specific actions)
05. DEBUG_FLAG_POSTLOGGING implementation - file/buffer logging
09. Wrapper for Projects as to make memmory operations simpler and faster.
10. `--optimize`, `-x` to test the memory allocations, the amount of required data that is needed 
	then is being stored in Windows Registry and we reuse it as the first allocation for open command data.
12. proper styling display with -> registry keys and values.
	project-header, command-header, footer, line-format, line-size, key-padding-size
14. Adding Quail configuration data using --add command: like `quail -a module "Name" "ProjectPath"`.
15. Edit Quail configuration data using --edit command: like `quail -e module "Name" "ProjectPath"`.
17. Installer CMake build + quail configuration ibld, ibbld, irun, irrun
18. Secrets vs Defines
19. Decide on 2-Read path + global.conf - so that we can define both at the very end and the begining constants, secrets and varaibles
21. Add registry option to display values in c8*. We don't want to break c16* saving just display them in c8*.
28. Colored debuglogs
79. Make it so global.txt values can be overriden
80. Make "_quail" variable to pass Quail directory - not sure if it's needed
81. !!! Make it so during an Error memory is properly deallocated !!!

--- INSTALLER

37. fix-do SelectObject clearing.

39. ? Maybe create a roadmap using that github tool (tables).
40. Move created window objects outside WM_PAINT.
41. Make sure everything is being destroyed properly.
53. Next button HIGHLIGHT and connection to eneter key.
54. Arrows and Highlight with them.
67. Add Proper Adding (it's own step) to registry and file creation to download page.
68. Proper "Cancel" handling.
69. Make script for automatic releases.
75. Make it run on a seperate Threat if necessery.

38. Create another subproject to hold code that is being used by both installer, uninstaller and quail itself. "global" or smth.
66. Create and add an Quail Icon.
71. Make installer displayed centered.
72. Clear everyting!

-- UNINSTALLER

74. Make quail scripts for uninstaller to. (upx) 
77. Move license.h" & "data.h" to GLOBAL project.


82. Give an option in installer to create a batch file and name it by default 'o'.
> So that instead of writting 'quail -o [...]' people can simply write 'o [...]'.
83. New feature 'groups' eg. cmake_remove -> make it so user can match with "cma rem".
85. Does fresh reinstallation remove the quail config files? Check it it shouldn't remove them!


-- 0.5
-> Proper deallocation when failure (ISSUE_XYZ1, syntax errors)
-> Disable making projects in global.txt -> see how it works
	Maybe if it works fine leave it.

Examples
- unity
- gimp
- windows terminal
- directory
- opening config

-- 0.6
-> Roadmap
-> yt-video
-> ... things from todolist.
