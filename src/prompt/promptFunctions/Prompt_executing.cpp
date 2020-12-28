/*============================================================================*/
/* Prompt_executing.cpp -- executing queue report                             */
/*============================================================================*/

#include "BASE.h"
#include "STORE.h"
#include "PROMPT.h"



PromptFun __Prompt_executing (
  PF_EXECUTING,           // prompt function
  "Prompt_executing"_Stz, // name of the prompt function
  "comment"_Stz,          // comment about the function
  "executing"_Stz,        // the prompt command
  nullptr,                // the second word
  bPrompt_executing,      // prompt function
  true,                   // enabled

  true,                   // visible
  "executing"_Stz,        // help
  ""_Stz                  // description
);


/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool PROMPT::bPrompt_executing(SePrompt* pPrompt)
{
  AM::pQueues()->vDumpExecutingSet();

  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
