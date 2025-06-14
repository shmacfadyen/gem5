// CSI 5640 Final Project
// Two-level Adaptive PAg Path Predictor - Implementation File

#include "cpu/pred/2level_pag.hh"

namespace gem5
{

namespace branch_prediction
{

// Constructor for the PAgBP branch predictor
PAgBP::PAgBP(const PAgBPParams &params)
  : BPredUnit(params), // Always need to pass params object to superclass
    globalHistoryReg(params.numThreads * (1ULL << params.addressBits), 0),
    // Rest of the below member variables are initialized specific to PAgBP based on param values
    historyRegisterSize(1ULL << params.historyRegBits),
    historyRegisterMask(historyRegisterSize - 1),
    addressBits(params.addressBits),
    addressMask((1ULL << addressBits) - 1),
    predCounters(historyRegisterSize, gem5::SatCounter8(params.predCtrBits)),
    predThreshold((1ULL << (params.predCtrBits - 1)) - 1)
{

}

// This function should lookup and return the next prediction
//
// Inputs:
// tid: The current thread being executed for the lookup
// pc: The current address of the branch instruction
//
// Outputs:
// bp_history: Set to a pointer for the corresponding history structure
//
// Returns true if taking the branch is predicted and false otherwise
bool PAgBP::lookup(ThreadID tid, Addr pc, void * &bp_history)
{
  // Get the history index using the thread and address
  unsigned histIdx = (tid << addressBits) | ((pc >> instShiftAmt) & addressMask);
  
  // Get the prediction index using the global history for this thread
  unsigned predIdx = globalHistoryReg[histIdx] & historyRegisterMask;

  // Ensure the prediction index is always valid
  assert(predIdx < historyRegisterSize);

  // Obtain the prediction by checking if the corresponding counter is above the threshold
  bool pred = predCounters[predIdx] > predThreshold;

  // Create a new PAg history object containing the current history and prediction made
  PAgHistory* hist = new PAgHistory;
  hist->globalHistoryReg = globalHistoryReg[histIdx];
  hist->pc = pc;
  hist->pred = pred;

  // Cast the history to a void pointer to meet the output format
  bp_history = static_cast<void*>(hist);

  // Return the prediction
  return pred;
}

// This function should update the branch history of the thread
//
// Inputs:
// tid: The current thread being executed
// pc: The current address of the branch instruction
// uncond: If the current branch is unconditional, then the bp_history will need to be completed
// taken: Whether or not the branch was taken
// target: The target address of the branch instruction. Currently UNUSED.
// 
// Outputs:
// bp_history: Set to a pointer for the corresponding history structure. 
//             Should not be updated unless uncond is true.
void PAgBP::updateHistories(ThreadID tid, Addr pc, bool uncond, bool taken,
                            Addr target,  void * &bp_history)
{
  // Ensure that either an unconditional branch occurred or the branch history exists
  assert(uncond || bp_history);

  // Get the history index using the thread and address
  unsigned histIdx = (tid << addressBits) | ((pc >> instShiftAmt) & addressMask);

  if (uncond)
  {
    // Initialize a new history structure for unconditional branches
    PAgHistory* hist = new PAgHistory;
    hist->globalHistoryReg = globalHistoryReg[histIdx];
    hist->pc = pc;
    hist->pred = true; // Unconditional branches should always have a true prediction
    bp_history = static_cast<void*>(hist);
  }

  // Update the global history value using a left shift operation and 
  // setting the LSB to 1 if the branch is taken and 0 if it is not
  globalHistoryReg[histIdx] = taken ? (globalHistoryReg[histIdx] << 1) | 1 : 
                              (globalHistoryReg[histIdx] << 1);
  
  // Use the bitwise mask to limit the size of the history register
  globalHistoryReg[histIdx] &= historyRegisterMask;
}

// This function should replace incorrectly predicted branch history 
// and clean up pointers to old history structures
//
// Inputs:
// tid: The current thread being executed
// bp_history: The previous branch history to use
void PAgBP::squash(ThreadID tid, void * &bp_history)
{
  // Ensure a branch history object is provided
  assert(bp_history);
  
  // Cast the history register value
  PAgHistory *history = static_cast<PAgHistory*>(bp_history);

  // Get the history index using the thread and address
  unsigned histIdx = (tid << addressBits) | ((history->pc >> instShiftAmt) & addressMask);

  // Update the history register value
  globalHistoryReg[histIdx] = history->globalHistoryReg;

  // Delete the irrelevant history value and set to a null pointer
  delete history;
  bp_history = nullptr;
}

// This function should update the prediction counters using the provided branch history
//
// Inputs:
// tid: The current thread being executed
// pc: The address of the corresponding branch instruction
// taken: Whether or not the branch was taken
// bp_history: The corresponding branch history for the prediction
// squashed: Whether or not a squash occurred, making this an invalid prediction
// inst: UNUSED
// target: The target address of the branch instruction. Currently UNUSED.
void PAgBP::update(ThreadID tid, Addr pc, bool taken,
                   void * &bp_history, bool squashed,
                   const StaticInstPtr & inst, Addr target)
{
  // Ensure a valid branch history is provided
  assert(bp_history);

  // Cast the branch history to the corresponding structure type
  PAgHistory *history = static_cast<PAgHistory*>(bp_history);

  // If a squash occurred, this is an invalid prediction
  // Therefore, don't update the prediction counter
  if (squashed) {
    // Get the history index using the thread and address
    unsigned histIdx = (tid << addressBits) | ((history->pc >> instShiftAmt) & addressMask);
    
    // Instead update the global history value using a left shift operation and 
    // setting the LSB to 1 if the branch is taken and 0 if it is not.
    // This corrects an earlier call to updateHistories() that was invalid.
    globalHistoryReg[histIdx] = taken ? (history->globalHistoryReg << 1) | 1 :
                                        (history->globalHistoryReg << 1);
    
    // Use the bitwise mask to limit the size of the history register
    globalHistoryReg[histIdx] &= historyRegisterMask;

    // Exit since a squash corresponds to an invalid prediction
    return;
  }

  // Get the prediction index using the provided history register
  unsigned predIdx = history->globalHistoryReg & historyRegisterMask;

  // Ensure the prediction index is valid
  assert(predIdx < historyRegisterSize);

  if (taken)
  {
    // Increment the prediction counter if the branch was taken
    predCounters[predIdx]++;
  }
  else
  {
    // Decrement the prediction counter if the branch was not taken
    predCounters[predIdx]--;
  }

  // Delete the history value since update() is the last function that utilizes it
  delete history;
  bp_history = nullptr;
}

}  // namespace branch_prediction

}  // namespace gem5
