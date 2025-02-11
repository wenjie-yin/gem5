#include "arch/arm/dynbranchtracer.hh"
#include "cpu/o3/dyn_inst.hh"
#include "arch/arm/regs/int.hh"
#include "debug/BranchTrace.hh"

namespace gem5
{

namespace o3
{



static char table_cols[] = "inst_addr," 
                           "pred_taken,mispredicted,"; 

ArmDynBranchTracer::ArmDynBranchTracer(const Params& params)
    : DynBranchTracer(params) {
}

void
ArmDynBranchTracer::traceDynBranch(const DynInstPtr& inst) {
     
    if (!debug::BranchTrace || !inst->staticInst->isCondCtrl())
        return;
    
    // print header
    if(!m_printed_header && debug::BranchTrace) {
        trace::getDebugLogger()->getOstream() << table_cols << std::endl;
        m_printed_header = true;
    }

    std::unique_ptr<PCStateBase> next_pc(inst->pcState().clone());
    inst->staticInst->advancePC(*next_pc);
    
    std::stringstream outs;
    
    // Instruction address, Instruction relative address
    outs << inst->pcState().instAddr() << ',';
    
    // Predicted taken, Mispredicted
    outs << inst->readPredTaken() << ',' << (*next_pc!=*inst->predPC) ;

    outs << std::endl;

    trace::getDebugLogger()->dprintf(::gem5::curTick(), std::string(), "%s", outs.str().c_str());
    
}


} // namespace o3

} // namespace gem5
