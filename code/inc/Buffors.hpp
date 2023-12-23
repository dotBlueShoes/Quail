#pragma once

#include "lib/BaseTypes.hpp"
#include "lib/Buffor.hpp"
#include "lib/Pointer.hpp"

buffor<uint8> memoryBlockA;
buffor<uint8> memoryBlockB;
buffor<uint8> memoryBlockC;

//ptr<uint8> memoryBlockA;
//ptr<uint8> memoryBlockB;
//ptr<uint8> memoryBlockC;

//block DEALLOCATE () {
//	memoryBlockA.DESTROY();
//	memoryBlockB.DESTROY();
//	memoryBlockC.DESTROY();
//}