/*============================================================================*/
/* Prompt_waiting.cpp -- waiting queue report                                 */
/*============================================================================*/

#include "StorageEngine.h"


PromptFun __Prompt_waiting (
  PF_WAITING,             // prompt function
  "Prompt_waiting"_Stz,   // name of the prompt function
  "comment"_Stz,          // comment about the function
  "waiting"_Stz,          // the prompt command
  nullptr,                // the second word
  bPrompt_waiting,        // prompt function
  true,                   // enabled

  true,                   // visible
  "waiting"_Stz,          // help
  ""_Stz                  // description
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool Prompt::bPrompt_waiting(SePrompt* pPrompt)
{
  AM::pQueues()->vDumpWaitingSet();

  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
