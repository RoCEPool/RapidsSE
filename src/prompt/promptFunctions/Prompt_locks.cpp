/*============================================================================*/
/* Prompt_locks.cpp -- locks report                                           */
/*============================================================================*/

#include "StorageEngine.h"


PromptFun __Prompt_locks (
  PF_LOCKS,             // prompt function
  "Prompt_locks"_Stz,   // name of the prompt function
  "comment"_Stz,        // comment about the function
  "locks"_Stz,          // the prompt command
  nullptr,              // the second word
  bPrompt_locks,        // prompt function
  true,                 // enabled

  true,                 // visible
  "locks"_Stz,          // help
  ""_Stz                // description
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool Prompt::bPrompt_locks(SePrompt* pPrompt)
{
  AM::pLockTracker()->vReport();
  
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
