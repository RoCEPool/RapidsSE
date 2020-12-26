/*============================================================================*/
/* Prompt_sleeping.cpp -- sleeping queue report                               */
/*============================================================================*/

#include "StorageEngine.h"


PromptFun __Prompt_sleeping (
  PF_SLEEPING,            // prompt function
  "Prompt_sleeping"_Stz,  // name of the prompt function
  "comment"_Stz,          // comment about the function
  "sleeping"_Stz,         // the prompt command
  nullptr,                // the second word
  bPrompt_sleeping,       // prompt function
  true,                   // enabled

  true,                   // visible
  "sleeping"_Stz,         // help
  ""_Stz                  // description
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool Prompt::bPrompt_sleeping(SePrompt* pPrompt)
{
  AM::pQueues()->vDumpSleepingQueue();

  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
