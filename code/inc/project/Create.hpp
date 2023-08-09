#pragma once
#include "../Framework.hpp"

namespace Project::Create {

	callback Action ( Tokens::ActionArgs& ) {
        DEBUG std::cout << "Entered action 'Create'\n";
    }

}