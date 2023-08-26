#pragma once

#include "BaseTypes.hpp"
#include "Array.hpp"

namespace Executable {

    using namespace mst::array_n;

    #define STRING_DATA_FILE_PATH_DEBUG R"(D:\ProgramFiles\dotBlueShoes\quail\)"
    
    extern array<char, 35> debugDataFilePath;
    extern uint64 dataFilePathLength;
    extern char* dataFilePath;

    block InitializeStrings() {

        #if DEF_DEBUG

        dataFilePath = debugDataFilePath.Pointer();
        dataFilePathLength = 35;

        #elif DEF_RELEASE

        // Get exe path.
        _get_pgmptr(&dataFilePath); // wchar _get_pgmptr(); // char _get_wpgmptr(&dataFilePath);
        dataFilePathLength = strlen(dataFilePath);// wchar wcslen(dataFilePath);

        // Turncate leaving characters after '\0' sign.
        dataFilePathLength -= 9; // Quail.exe
        dataFilePath[dataFilePathLength] = '\0';

        #endif
    }

}