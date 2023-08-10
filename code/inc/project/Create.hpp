#pragma once
#include "../Framework.hpp"

namespace Commands::Create {

	callback Action ( Tokens::ActionArgs& ) {
        DEBUG std::cout << "Entered action 'Create'\n";
    }

}