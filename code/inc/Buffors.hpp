#pragma once

#include "lib/BaseTypes.hpp"
#include "lib/Buffor.hpp"
#include "lib/Pointer.hpp"

using mst::buffor;

extern buffor<uint8> memoryBlockA;
extern buffor<uint8> memoryBlockB;
extern buffor<uint8> memoryBlockC;

//ptr<uint8> memoryBlockA;
//ptr<uint8> memoryBlockB;
//ptr<uint8> memoryBlockC;

//block DEALLOCATE () {
//	memoryBlockA.DESTROY();
//	memoryBlockB.DESTROY();
//	memoryBlockC.DESTROY();
//}