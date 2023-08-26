#pragma once

#include "BaseTypes.hpp"
#include "Array.hpp"

#include <cstring>

namespace Executable {

    using namespace mst::array_n;

    #define STRING_DATA_FILE_PATH_DEBUG R"(D:\ProgramFiles\dotBlueShoes\quail\configs)"
    
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
        //dataFilePathLength = strlen(dataFilePath);// wchar wcslen(dataFilePath);

        // Turncate leaving characters after '\0' sign.
        //dataFilePathLength -= 9; // Quail.exe
        //dataFilePath[dataFilePathLength] = '\0';

        // "configs\" [ this works because that string is 8 is less then 9 ! ]
        const auto& catalogPointer = strlen(dataFilePath) - 9;
        dataFilePath[catalogPointer + 0] = 'c';
        dataFilePath[catalogPointer + 1] = 'o';
        dataFilePath[catalogPointer + 2] = 'n';
        dataFilePath[catalogPointer + 3] = 'f';
        dataFilePath[catalogPointer + 4] = 'i';
        dataFilePath[catalogPointer + 5] = 'g';
        dataFilePath[catalogPointer + 6] = 's';
        dataFilePath[catalogPointer + 7] = '\\';
        dataFilePath[catalogPointer + 8] = '\0';
        dataFilePathLength = catalogPointer + 8; // without '\0'. 

        //printf("%llu", dataFilePathLength);
        //printf("\n%s\n", dataFilePath);

        #endif
    }

}