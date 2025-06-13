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
  // Constructor must take in the params object defined based in BranchPredictor.py
  // See GAgBP class in BranchPredictor.py  
  GAgBP(const GAgBPParams &params);
  
  protected:
    // Overridden functions from the base BPredUnit class
    // Should be implemented in every branch prediction class
    bool lookup(ThreadID tid, Addr pc, void * &bp_history) override;
    void updateHistories(ThreadID tid, Addr pc, bool uncond, bool taken,
                         Addr target,  void * &bp_history) override;
    void squash(ThreadID tid, void * &bp_history) override;
    void update(ThreadID tid, Addr pc, bool taken,
                void * &bp_history, bool squashed,
                const StaticInstPtr & inst, Addr target) override;

  private:
    // History class data structure
    // Contains whatever needed information on the current prediction 
    // and associated history values
    struct GAgHistory
    {
        unsigned globalHistoryReg;
        bool pred;
    };

    // Vector of histories for each thread
    std::vector<unsigned> globalHistoryReg;

    // The size of the history register
    unsigned historyRegisterSize;

    // A mask for the history register (1-historyRegisterSize)
    unsigned historyRegisterMask;

    // Prediction counters for each history value
    // Uses a SatCounter8 object to limit the range of the counter
    std::vector<SatCounter8> predCounters;

    // The threshold of the counter value for a successful prediction
    unsigned predThreshold;
};

}  // namespace branch_prediction

}  // namespace gem5

#endif  // __CPU_PRED_2LEVEL_PRED_GAG_HH__
