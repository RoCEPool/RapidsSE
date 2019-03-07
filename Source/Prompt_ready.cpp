/*============================================================================*/
/* Prompt_ready.cpp -- ready queue report                                     */
/*============================================================================*/

#include "Prompt.h"
#include "StorageEngine.h"

PromptNote mPromptNote_ready (
  PF_READY,               // prompt function
  "Prompt_ready"_Stz,     // name of the prompt function
  "comment"_Stz,          // comment about the function
  "ready"_Stz,            // the prompt command
  nullptr,                // the second word
  bPrompt_ready,          // prompt function
  true,                   // enabled

  true,                   // visible
  "ready"_Stz,            // help
  ""_Stz                  // description
);

extern AttachMap*  gpAttachMap;

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool Prompt::bPrompt_ready(SePrompt* pPrompt)
{
  gpAttachMap->m_pQueues->vDumpReadyQueue();
  gpAttachMap->m_pQueues->vDumpReadyQueueLP();
  
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
