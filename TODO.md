01. COMPARESEARCH::ArrayPartFirstMatchVector inside `open\main.hpp` reads outside memory!
	It does not check for EOS sign it simply goes through. Those string do have EOS.
	But when we're checking against a string thats longer we simply bypass it. (and it might memory allocated by us?)
04. Clocks (debug utility to check times of specific actions)
05. DEBUG_FLAG_POSTLOGGING implementation - file/buffor logging
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
34. Change Installer into Wizard because it's a (Installer, Deinstaller, Fix utility)
35. Make the blp file socked inside .res file or something else.
36. clear installer code 
37. fix-do SelectObject clearing
38. Create another subproject to hold code that is being used by both installer and quail itself. "global" or smth.
39. ? Maybe create a roadmap using that github tool (tables)
40. Move created window objects outside WM_PAINT
41. Make sure everything is being destroyed properly.
48. Make a proper REGISTRY page
50. Make a proper DOWNLOAD page
51. Add left padding to LICENSE control
51. license script -> Make the header include the L"\n"\ (newline) ".
