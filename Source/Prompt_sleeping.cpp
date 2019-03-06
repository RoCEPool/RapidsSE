/*============================================================================*/
/* Prompt_sleeping.cpp -- sleeping queue report                               */
/*============================================================================*/

#include "Prompt.h"
#include "StorageEngine.h"

PromptNote mPromptNote_sleeping (
  PF_NONE,                     // prompt function
  "Prompt_sleeping"_Stz,  // name of the prompt function
  "comment"_Stz,          // comment about the functopm
  "sleeping"_Stz,         // the prompt command
  nullptr,                // the second word
  bPrompt_sleeping,       // prompt function
  true,                   // enabled

  true,                   // visible
  "fun help"_Stz          // help
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
