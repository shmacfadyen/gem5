// CSI 5640 Final Project
// Two-level Adaptive GAg Path Predictor - Implementation File

#include "cpu/pred/2level_gag.hh"

#include "base/bitfield.hh"
#include "base/intmath.hh"

namespace gem5
{

namespace branch_prediction
{

GAgBP::GAgBP(const GAgBPParams &params)
  : BPredUnit(params),
    globalHistoryReg(params.numThreads, 0),
    historyRegisterSize(1ULL << params.historyRegBits),
    historyRegisterMask(historyRegisterSize - 1),
    predCounters(historyRegisterSize, gem5::SatCounter8(params.predCtrBits)),
    predThreshold((1ULL << (params.predCtrBits - 1)) - 1)
{

}

bool GAgBP::lookup(ThreadID tid, Addr pc, void * &bp_history)
{
  unsigned predIdx = globalHistoryReg[tid] & historyRegisterMask;

  assert(predIdx < historyRegisterSize);

  bool pred = predCounters[predIdx] > predThreshold;

  GAgHistory* hist = new GAgHistory;
  hist->globalHistoryReg = globalHistoryReg[tid];
  hist->pred = pred;

  bp_history = static_cast<void*>(hist);

  return pred;
}

void GAgBP::updateHistories(ThreadID tid, Addr pc, bool uncond, bool taken,
                            Addr target,  void * &bp_history)
{
  assert(uncond || bp_history);
  if (uncond)
  {
    GAgHistory* hist = new GAgHistory;
    hist->globalHistoryReg = globalHistoryReg[tid];
    hist->pred = true;
    bp_history = static_cast<void*>(hist);
  }
  globalHistoryReg[tid] = taken ? (globalHistoryReg[tid] << 1) | 1 :
                               (globalHistoryReg[tid] << 1);
  globalHistoryReg[tid] &= historyRegisterMask;
}

void GAgBP::squash(ThreadID tid, void * &bp_history)
{
  assert(bp_history);
  
  GAgHistory *history = static_cast<GAgHistory*>(bp_history);
  globalHistoryReg[tid] = history->globalHistoryReg;

  delete history;
  bp_history = nullptr;
}

void GAgBP::update(ThreadID tid, Addr pc, bool taken,
                   void * &bp_history, bool squashed,
                   const StaticInstPtr & inst, Addr target)
{
  assert(bp_history);

  GAgHistory *history = static_cast<GAgHistory*>(bp_history);

  if (squashed) {
    globalHistoryReg[tid] = taken ? (history->globalHistoryReg << 1) | 1 :
                               (history->globalHistoryReg << 1);
    globalHistoryReg[tid] &= historyRegisterMask;
    return;
  }

  unsigned predIdx = history->globalHistoryReg & historyRegisterMask;

  assert(predIdx < historyRegisterSize);

  if (taken)
  {
    predCounters[predIdx]++;
  }
  else
  {
    predCounters[predIdx]--;
  }

  delete history;
  bp_history = nullptr;
}

}  // namespace branch_prediction

}  // namespace gem5
