/*============================================================================*/
/* Prompt_locks.cpp -- locks report                                           */
/*============================================================================*/

#include "Prompt.h"
#include "StorageEngine.h"

PromptNote mPromptNote_locks (
  PF_LOCKS,             // prompt function
  "Prompt_locks"_Stz,   // name of the prompt function
  "comment"_Stz,        // comment about the functopm
  "locks"_Stz,          // the prompt command
  nullptr,              // the second word
  bPrompt_locks,        // prompt function
  true,                 // enabled

  true,                 // visible
  "fun help"_Stz        // help
);

extern AttachMap*  gpAttachMap;

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool Prompt::bPrompt_locks(SePrompt* pPrompt)
{
  gpAttachMap->m_oLockTracker.vReport();
  
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
