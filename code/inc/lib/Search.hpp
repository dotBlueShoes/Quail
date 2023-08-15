#pragma once

#include "BaseTypes.hpp"
#include "Array.hpp"

namespace Search {

    template <class T>
    block KnownLength(
        OUT uint8& resultIndex,             // If found resultIndex points to found element if not stays unchainged.
        IN  const uint8& valuesLength,      // Length of string data we're compering/.
        IN  const T* values,                // String data we're compering.
        IN  const uint8& elementsLength,    // Length of elements to compare to.
        IN  const void** elements,          // Array on which we're compering.
        IN  const size& typeSize = 0,       // For variable in possible Struct position in bytes. 
        IN  const size& elementOffset = 0   // For variable in possible Struct position in bytes. 
        /* missing lambda for collision failed execution */
    ) {
        uint8 collision = 1;

        for (; resultIndex < elementsLength * collision; ++resultIndex) {
            for (uint8 i = 0; i < valuesLength; ++i) {
                //collision += (values[i] == (elements[resultIndex].name[i]);
                auto& element = ((T*)((byte*)(elements) + (typeSize * resultIndex) + elementOffset))[i];
                collision += values[i] == element;
            }
            collision = ((collision - 1) != valuesLength);
        }

        if (collision) {
            printf("%s", "Fail No Matching Action!");
            exit(ExitCode::FAILURE_NO_COMMAND_FOR_SPECIFIED_PROJECT);
        }

        --resultIndex;
    }

}