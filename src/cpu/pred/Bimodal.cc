// CSI 5640 Final Project 
// Bimodal Branch Predictor - Implementation File

#include "cpu/pred/Bimodal.hh"

namespace gem5
{

namespace branch_prediction
{

BimodalBP::BimodalBP(const BimodalBPParams &params)
  : BPredUnit(params),
    predCounters(1ULL << params.indexBits, gem5::SatCounter8(params.predCtrBits)),
    predThreshold((1ULL << (params.predCtrBits - 1)) - 1),
    indexMask((1ULL << params.indexBits) - 1),
    predCtrBits(params.predCtrBits)
{
}

bool BimodalBP::lookup(ThreadID tid, Addr pc, void * &bp_history)
{
  unsigned idx = (pc >> 2) & indexMask;
  bool pred = predCounters[idx] > predThreshold;

  bp_history = nullptr;
  return pred;
}

void BimodalBP::updateHistories(ThreadID tid, Addr pc, bool uncond,
  bool taken, Addr target, void * &bp_history)
{
}

void BimodalBP::squash(ThreadID tid, void * &bp_history)
{
}

void BimodalBP::update(ThreadID tid, Addr pc, bool taken,
  void * &bp_history, bool squashed,
  const StaticInstPtr & inst, Addr target)
{
  unsigned idx = (pc >> 2) & indexMask;

  if (taken)
    predCounters[idx]++;
  else
    predCounters[idx]--;

  bp_history = nullptr;
}

}  // namespace branch_prediction

}  // namespace gem5
