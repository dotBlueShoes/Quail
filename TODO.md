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
08. Add registry option to display values in c8*. We don't want to break c16* saving just display them in c8*.
09. Make it so global.txt values can be overridden
10. Make "_quail" variable to pass Quail directory - not sure if it's needed
11. Move license.h" & "data.h" to GLOBAL project.

--- INSTALLER

12. ? Maybe create a roadmap using that github tool (tables).
13. Move created window objects outside WM_PAINT.
14. Make sure everything is being destroyed properly.
15. Next button HIGHLIGHT and connection to enter key.
16. Arrows and Highlight with them.
17. Proper "Cancel" handling.
18. Make script for automatic releases.
19. Make it run on a separate Threat if necessary.
20. Make installer display centered.

-- UNINSTALLER

21. Give an option in installer to create a batch file and name it by default 'o'.
> So that instead of writing 'quail -o [...]' people can simply write 'o [...]'.
22. New feature 'groups' eg. cmake_remove -> make it so user can match with "cma rem".
23. Fresh reinstallation should not remove the quail config files.
26. !!! FIX REMOVAL OF CONFIG FILES WHY ARE THEY BEING USED BY SOMETHING !? - It must be quail instance itself...


-- 0.5

24. Disable making projects in global.txt -> see how it works. Maybe if it works fine leave it.

25. Examples:
- unity
- gimp
- windows terminal
- directory
- opening config

-- 0.6
-> Roadmap
-> yt-video
-> ... things from todolist.
