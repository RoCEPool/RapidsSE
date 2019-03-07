/*============================================================================*/
/* Prompt_waiting.cpp -- waiting queue report                                 */
/*============================================================================*/

#include "Prompt.h"
#include "StorageEngine.h"

PromptNote mPromptNote_waiting (
  PF_WAITING,             // prompt function
  "Prompt_waiting"_Stz,   // name of the prompt function
  "comment"_Stz,          // comment about the functopm
  "waiting"_Stz,          // the prompt command
  nullptr,                // the second word
  bPrompt_waiting,        // prompt function
  true,                   // enabled

  true,                   // visible
  "waiting"_Stz,          // help
  ""_Stz                  // description
);

extern AttachMap*  gpAttachMap;

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool Prompt::bPrompt_waiting(SePrompt* pPrompt)
{
  gpAttachMap->m_pQueues->vDumpWaitingSet();

  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
