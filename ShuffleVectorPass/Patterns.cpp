#include "./Patterns.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;

bool RotationPattern::matches(ShuffleVectorInst *inst) {
    errs() << "rotation pattern\n";
    return false;
}

bool RotationPattern::optimize(ShuffleVectorInst *inst) {
    return false;
}