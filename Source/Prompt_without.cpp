/*============================================================================*/
/* Prompt_without.cpp -- compiled without report                              */
/*============================================================================*/

#include "Prompt.h"


PromptFun __Prompt_without (
  PF_WITH,              // prompt function
  "Prompt_without"_Stz, // name of the prompt function
  "comment"_Stz,        // comment about the function
  "without"_Stz,        // the prompt command
  nullptr,              // the second word
  bPrompt_without,      // prompt function
  true,                 // enabled

  true,                 // visible
  "with"_Stz,           // help
  ""_Stz                // description
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool Prompt::bPrompt_without(SePrompt* pPrompt)
{
  Correctness::vTraceWithout();

  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
