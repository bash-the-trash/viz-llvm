//===---- RISCVISelDAGToDAG.h - A dag to dag inst selector for RISCV ------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file defines an instruction selector for the RISCV target.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_RISCV_RISCVISELDAGTODAG_H
#define LLVM_LIB_TARGET_RISCV_RISCVISELDAGTODAG_H

#include "RISCV.h"
#include "RISCVTargetMachine.h"
#include "llvm/CodeGen/SelectionDAGISel.h"

// RISCV-specific code to select RISCV machine instructions for
// SelectionDAG operations.
namespace llvm {
class RISCVDAGToDAGISel : public SelectionDAGISel {
  const RISCVSubtarget *Subtarget = nullptr;

public:
  explicit RISCVDAGToDAGISel(RISCVTargetMachine &TargetMachine)
      : SelectionDAGISel(TargetMachine) {}

  StringRef getPassName() const override {
    return "RISCV DAG->DAG Pattern Instruction Selection";
  }

  bool runOnMachineFunction(MachineFunction &MF) override {
    Subtarget = &MF.getSubtarget<RISCVSubtarget>();
    return SelectionDAGISel::runOnMachineFunction(MF);
  }

  void PostprocessISelDAG() override;

  void Select(SDNode *Node) override;

  bool SelectInlineAsmMemoryOperand(const SDValue &Op, unsigned ConstraintID,
                                    std::vector<SDValue> &OutOps) override;

  bool SelectAddrFI(SDValue Addr, SDValue &Base);

  bool isUnneededShiftMask(SDNode *N, unsigned Width) const;

  bool MatchSRLIW(SDNode *N) const;
  bool MatchSLOI(SDNode *N) const;
  bool MatchSROI(SDNode *N) const;
  bool MatchSROIW(SDNode *N) const;
  bool MatchSLLIUW(SDNode *N) const;

  bool selectVLOp(SDValue N, SDValue &VL);

  bool selectVSplat(SDValue N, SDValue &SplatVal);
  bool selectVSplatSimm5(SDValue N, SDValue &SplatVal);
  bool selectVSplatUimm5(SDValue N, SDValue &SplatVal);

  void selectVLSEG(SDNode *Node, unsigned IntNo, bool IsStrided);
  void selectVLSEGMask(SDNode *Node, unsigned IntNo, bool IsStrided);
  void selectVLSEGFF(SDNode *Node);
  void selectVLSEGFFMask(SDNode *Node);
  void selectVLXSEG(SDNode *Node, unsigned IntNo);
  void selectVLXSEGMask(SDNode *Node, unsigned IntNo);
  void selectVSSEG(SDNode *Node, unsigned IntNo, bool IsStrided);
  void selectVSSEGMask(SDNode *Node, unsigned IntNo, bool IsStrided);
  void selectVSXSEG(SDNode *Node, unsigned IntNo);
  void selectVSXSEGMask(SDNode *Node, unsigned IntNo);

// Include the pieces autogenerated from the target description.
#include "RISCVGenDAGISel.inc"

private:
  void doPeepholeLoadStoreADDI();
};
}

#endif
