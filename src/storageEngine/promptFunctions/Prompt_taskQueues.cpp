/*============================================================================*/
/* bPrompt_taskQueues.cpp -- taskQueues report                                */
/*============================================================================*/

#include "StorageEngine.h"


PromptFun __Prompt_taskQueues (
  PF_TASKQUEUES,            // prompt function
  "bPrompt_taskQueues"_Stz, // name of the prompt function
  "comment"_Stz,        // comment about the function
  "taskQueues"_Stz,     // the prompt command
  nullptr,              // the second word
  bPrompt_taskQueues,   // prompt function
  true,                 // enabled

  true,                 // visible
  "taskQueues"_Stz,     // help
  ""_Stz                // description
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool Prompt::bPrompt_taskQueues(SePrompt* pPrompt)
{
  Queues* pQueues = AM::pQueues();
  
  pQueues->vDumpExecutingSet();
  pQueues->vDumpReadyQueue();
  pQueues->vDumpReadyQueueLP();
  pQueues->vDumpSleepingQueue();
  pQueues->vDumpWaitingSet();
  
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
