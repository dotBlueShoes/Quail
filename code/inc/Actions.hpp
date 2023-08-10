#pragma once
#include "Framework.hpp"

#include "Tokens.hpp"
#include "project/Recreate.hpp"
#include "project/Create.hpp"
#include "project/Help.hpp"
#include "project/Open.hpp"

namespace Tokens::Actions {

    callback ActionSetup( Tokens::ActionArgs& ) {
        DEBUG std::cout << "Entered action 'Setup'\n";
    }

    const array<Action, 4> definedActions {
        ActionSetup,
        Commands::Recreate::Action,
        Commands::Open::Action,
        Commands::Create::Action
    };

    size actionsCount;
    Action* actions;

    block Initialize() {
        actions = new Action[1];
        actionsCount = 1;
    }

    block Destroy() {
        delete[] actions;
    }

    block AssignMainAction(
        const uint8 index
    ) {
        actions[0] = definedActions[index];
    }

}