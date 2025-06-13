// CSI 5640 Final Project
// Two-level Adaptive GAg Path Predictor - Header File

#ifndef __CPU_PRED_2LEVEL_GAG_PRED_HH__
#define __CPU_PRED_2LEVEL_GAG_PRED_HH__

#include "base/sat_counter.hh"
#include "cpu/pred/bpred_unit.hh"
#include "params/GAgBP.hh"

namespace gem5
{

namespace branch_prediction
{

class GAgBP : public BPredUnit
{
  public:
    GAgBP(const GAgBPParams &params);
  
  protected:
    bool lookup(ThreadID tid, Addr pc, void * &bp_history) override;
    void updateHistories(ThreadID tid, Addr pc, bool uncond, bool taken,
                         Addr target,  void * &bp_history) override;
    void squash(ThreadID tid, void * &bp_history) override;
    void update(ThreadID tid, Addr pc, bool taken,
                void * &bp_history, bool squashed,
                const StaticInstPtr & inst, Addr target) override;

  private:
    struct GAgHistory
    {
        unsigned globalHistoryReg;
        bool pred;
    };

    std::vector<unsigned> globalHistoryReg;
    unsigned historyRegisterSize;
    unsigned historyRegisterMask;

    std::vector<SatCounter8> predCounters;
    unsigned predThreshold;
};

}  // namespace branch_prediction

}  // namespace gem5

#endif  // __CPU_PRED_2LEVEL_PRED_GAG_HH__
