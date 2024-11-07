1. COMPARESEARCH::ArrayPartFirstMatchVector inside `open\main.hpp` reads outside memory!
	It does not check for EOS sign it simply goes through. Those string do have EOS.
	But when we're checking against a string thats longer we simply bypass it. (and it might memory allocated by us?)
2. True DEBUG and RUNTIME build (with an ability to disable and activate logs)
    DEBUG_FLAG_LOGGING, DEBUG_FLAG_MEMMORY, DEBUG_FLAG_CLOCKS, DEBUG_FLAG_POSTLOGGING - file/buffor logging
3. Free and Malloc preprocessor functions + allocation iterations checkers
4. Clocks (debug utility to check times of specific actions)
5. Display sub/projects of the (main config / project) in the style of old system.
6. Second read for constants
7. Cascading Constatants

// LATER. secrets, variables, comamnds, queues