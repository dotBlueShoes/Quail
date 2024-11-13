1. COMPARESEARCH::ArrayPartFirstMatchVector inside `open\main.hpp` reads outside memory!
	It does not check for EOS sign it simply goes through. Those string do have EOS.
	But when we're checking against a string thats longer we simply bypass it. (and it might memory allocated by us?)
4. Clocks (debug utility to check times of specific actions)
5. DEBUG_FLAG_POSTLOGGING implementation - file/buffor logging
6. Display sub/projects of the (main config / project) in the style of old system.
7. Second read for constants
8. Cascading Constatants


// LATER. secrets, variables, comamnds, queues