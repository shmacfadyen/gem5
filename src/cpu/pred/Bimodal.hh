// CSI 5640 Final Project
// Bimodal Branch Predictor - Header File

#ifndef __CPU_PRED_BIMODAL_PRED_HH__
#define __CPU_PRED_BIMODAL_PRED_HH__

#include "base/sat_counter.hh"
#include "cpu/pred/bpred_unit.hh"
#include "params/BimodalBP.hh"

namespace gem5
{

namespace branch_prediction
{

class BimodalBP : public BPredUnit
{
  public:
    BimodalBP(const BimodalBPParams &params);

  protected:
    bool lookup(ThreadID tid, Addr pc, void * &bp_history) override;
    void updateHistories(ThreadID tid, Addr pc, bool uncond, bool taken,
    Addr target, void * &bp_history) override;
    void squash(ThreadID tid, void * &bp_history) override;
    void update(ThreadID tid, Addr pc, bool taken,
    void * &bp_history, bool squashed,
    const StaticInstPtr & inst, Addr target) override;

  private:

    std::vector<SatCounter8> predCounters;
    unsigned predThreshold;
    unsigned indexMask;
    unsigned predCtrBits;
};

} // namespace branch_prediction

} // namespace gem5

#endif // __CPU_PRED_BIMODAL_PRED_HH__
