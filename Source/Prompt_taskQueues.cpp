/*============================================================================*/
/* bPrompt_taskQueues.cpp -- taskQueues report                                */
/*============================================================================*/

#include "Prompt.h"
#include "StorageEngine.h"

PromptNote mPromptNote_taskQueues (
  PF_TASKQUEUES,            // prompt function
  "bPrompt_taskQueues"_Stz, // name of the prompt function
  "comment"_Stz,        // comment about the functopm
  "taskQueues"_Stz,     // the prompt command
  nullptr,              // the second word
  bPrompt_taskQueues,   // prompt function
  true,                 // enabled

  true,                 // visible
  "taskQueues"_Stz,     // help
  ""_Stz                // description
);

extern AttachMap*  gpAttachMap;

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool Prompt::bPrompt_taskQueues(SePrompt* pPrompt)
{
  Queues* pQueues = gpAttachMap->m_pQueues;
  pQueues->vDumpExecutingSet();
  pQueues->vDumpReadyQueue();
  pQueues->vDumpReadyQueueLP();
  pQueues->vDumpReadySendQueues();
  pQueues->vDumpReadySlotQueues();
  pQueues->vDumpSleepingQueue();
  pQueues->vDumpWaitingSet();
  
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
