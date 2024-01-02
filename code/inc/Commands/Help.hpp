#pragma once
#include "lib/Framework.hpp"

#include "Arguments/Arguments.hpp"

namespace Commands::Help {

	block Display() {

		// Shorts
		auto&& sOpen = Arguments::shorts[0];
		auto&& sHelp = Arguments::shorts[1];

		// Fulls
		const uint8* const open = Arguments::fulls.Pointer() + Arguments::START_POSITIONS[0];
		const uint8* const help = Arguments::fulls.Pointer() + Arguments::START_POSITIONS[1];

		// DESCRIPTIONS
		array<charFile, 3 + 45 + 40 + 2> descOpen { 
			":\n\tOpens config file, shows config file contents and shows specific project subcommands.\n "
		};

		array<charFile, 44> descHelp { 
			":\n\tDisplays information about program use.\n "
		};

		{ // DISPLAY FILES
			fwrite("\n ", sizeof(charFile), 2, stdout);

			// HELP
			fwrite(sHelp.Pointer(), sizeof(charFile), sHelp.Length(), stdout);
			fwrite(", ", sizeof(charFile), 2, stdout);
			fwrite(help, sizeof(charFile), Arguments::ARG_HELP_LENGTH, stdout);
			fwrite(descHelp.Pointer(), sizeof(charFile), descHelp.Length(), stdout);

			// OPEN
			fwrite(sOpen.Pointer(), sizeof(charFile), sOpen.Length(), stdout);
			fwrite(", ", sizeof(charFile), 2, stdout);
			fwrite(open, sizeof(charFile), Arguments::ARG_OPEN_LENGTH, stdout);
			fwrite(descOpen.Pointer(), sizeof(charFile), descOpen.Length(), stdout);

			fputc('\n', stdout);
		}

	}

}