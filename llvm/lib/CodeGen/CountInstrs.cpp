#include <deque>
#include <vector>

#include "llvm/ADT/BitVector.h"
#include "llvm/ADT/SetVector.h"
#include "llvm/CodeGen/MachineFunctionPass.h"
#include "llvm/CodeGen/MachineRegisterInfo.h"
#include "llvm/CodeGen/Passes.h"
#include "llvm/CodeGen/TargetRegisterInfo.h"
#include "llvm/CodeGen/TargetSubtargetInfo.h"
#include "llvm/InitializePasses.h"
#include "llvm/Pass.h"
#include "llvm/PassRegistry.h"
#include "llvm/Support/Debug.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;

#define DEBUG_TYPE "count-instrs"

namespace {

class CountInstrs : public MachineFunctionPass {
public:
  bool runOnMachineFunction(MachineFunction &MF) override;

  static char ID;
  CountInstrs() : MachineFunctionPass(ID) {}

  StringRef getPassName() const override { return "Count Instructions"; }

  void getAnalysisUsage(AnalysisUsage &AU) const override {
    AU.setPreservesCFG();
    MachineFunctionPass::getAnalysisUsage(AU);
  }
};

} // end anonymous namespace

char CountInstrs::ID = 0;
char &llvm::CountInstrsID = CountInstrs::ID;

INITIALIZE_PASS(CountInstrs, DEBUG_TYPE, "Count Instructions", false, false)

bool CountInstrs::runOnMachineFunction(MachineFunction &MF) {
  int count = 0;
  for (MachineBasicBlock &MBB : MF) {
    for (MachineInstr &MI : MBB) {
      if (MI.isPseudo()) {
        continue;
      }
      count++;
    }
  }
  errs() << "KASPER: {\"" << MF.getName() << "\": " << count << "}\n";
  return false;
}
