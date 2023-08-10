#pragma once
#include "Framework.hpp"

namespace Tokens {
	
	using CommandBuffor = const char;
	using CommandShort = array<CommandBuffor, 2>;

	struct Command {
		CommandBuffor* const buffor;
		size length;
	};

	using CommandDescription = CommandBuffor* const;

	struct ActionArgs {
		const int32 argumentsLength;
		const char** arguments;
	};

	using Action = void (*)( ActionArgs& );

	
	const CommandShort 
		setupShort 		{ "-s" },
		reprojectShort 	{ "-r" },
		openShort 		{ "-o" },
		createShort 	{ "-c" };
	
	const array<CommandShort, 4> shorts {
		setupShort,
		reprojectShort,
		openShort,
		createShort
	};
	
	const array<CommandBuffor, 7>  setup 	{ "--setup" };
	const array<CommandBuffor, 14> recreate { "--npp-recreate" };
	const array<CommandBuffor, 6>  open 	{ "--open" };
	const array<CommandBuffor, 8>  create 	{ "--create" };
	
	const array<Command, 4> commands {
		Command { setup.Pointer(), 	  setup.Length() 	},
		Command { recreate.Pointer(), recreate.Length() },
		Command { open.Pointer(),	  open.Length() 	},
		Command { create.Pointer(),	  create.Length() 	}
	};
	
	const array<CommandBuffor, 19> descriptionSetup 	{ "Does that and that." };
	const array<CommandBuffor, 19> descriptionRecreate 	{ "Does this and that." };
	const array<CommandBuffor, 25> descriptionOpen 		{ "Opens configured project." };
	const array<CommandBuffor, 43> descriptionCreate 	{ "Creates new project and it's configuration." };
	
	const array<CommandDescription, 4> descriptions {
		descriptionSetup.Pointer(),
		descriptionRecreate.Pointer(),
		descriptionOpen.Pointer(),
		descriptionCreate.Pointer()
	};
	
	namespace Create {
		
		const CommandShort templateShort { "-t" };
		
		const array<CommandShort, 1> shorts {
			templateShort
		};
		
		const array<CommandBuffor, 10> _template { "--template" };
		
		const array<Command, 1> commands {
			{ _template.Pointer(), _template.Length() }
		};
		
	}

	getter uint8 IsEnoughArgumentsPassedToValidate(
		const int32& argumentsLength
	) {
		return (argumentsLength > 1);
	}
	
	/// Sample Use:
	///  : quail -o "project1"
	///  : quail -c "nazwa" -t "template"
	
	/// Theres a main command like -o & -c
	///  and there are subcommands which work with main command.
	///  - there can be same names for subcommands for different commands
	///  - [a subcommand can be specified first ??? ] // no.
	
}