#include "./Patterns.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;
using namespace ShuffleVectorOptimization;

bool RotationPattern::matches(ShuffleVectorInst *inst) {
    errs() << "rotation pattern";
    return false;
}

bool RotationPattern::optimize(ShuffleVectorInst *inst) {
    return false;
}