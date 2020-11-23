/*============================================================================*/
/* Prompt_ready.cpp -- ready queue report                                     */
/*============================================================================*/

#include "Prompt.h"


PromptFun __Prompt_ready (
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

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool Prompt::bPrompt_ready(SePrompt* pPrompt)
{
  AM::pQueues()->vDumpReadyQueue();
  AM::pQueues()->vDumpReadyQueueLP();
  
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
