#ifndef COMMONVECTORS_H
#define COMMONVECTORS_H

#include "llvm/IR/Instructions.h"
#include "./lib/bitblock.hpp"
#include "./FieldWidth.h"

using namespace llvm;

namespace ShuffleVectorOptimization {

struct CommonVectors {
    BitBlock maskVector;
    BitBlock indexVector;
    BitBlock lengthVector;
    BitBlock lengthMaskVector;
    BitBlock zeroVector;

    CommonVectors(ShuffleVectorInst *inst);
};

}

#endif
