/*============================================================================*/
/* Prompt_sleeping.cpp -- sleeping queue report                               */
/*============================================================================*/

#include "Prompt.h"
#include "StorageEngine.h"

PromptNote mPromptNote_sleeping (
  PF_SLEEPING,            // prompt function
  "Prompt_sleeping"_Stz,  // name of the prompt function
  "comment"_Stz,          // comment about the functopm
  "sleeping"_Stz,         // the prompt command
  nullptr,                // the second word
  bPrompt_sleeping,       // prompt function
  true,                   // enabled

  true,                   // visible
  "sleeping"_Stz,          // help
                                 ""_Stz                       // description
);

extern AttachMap*  gpAttachMap;

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool Prompt::bPrompt_sleeping(SePrompt* pPrompt)
{
  gpAttachMap->m_pQueues->vDumpSleepingQueue();

  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
