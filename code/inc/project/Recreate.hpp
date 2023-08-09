#pragma once
#include "Framework.hpp"

namespace Project::Recreate {

	callback Action ( Tokens::ActionArgs& ) {
        DEBUG std::cout << "Entered action 'Recreate'\n";
    }

}