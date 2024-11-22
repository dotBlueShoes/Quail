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
16. add _project_dir and _name constants as default

// LATER. secrets, variables, comamnds, queues