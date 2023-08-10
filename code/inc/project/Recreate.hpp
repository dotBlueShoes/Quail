#pragma once
#include "Framework.hpp"

namespace Commands::Recreate {

	callback Action ( Tokens::ActionArgs& ) {
        DEBUG std::cout << "Entered action 'Recreate'\n";
    }

}