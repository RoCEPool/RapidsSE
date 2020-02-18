/*============================================================================*/
/* Prompt_with.cpp -- compiled with report                                    */
/*============================================================================*/

#include "Prompt.h"


PromptFun __Prompt_with (
  PF_WITH,             // prompt function
  "Prompt_with"_Stz,   // name of the prompt function
  "comment"_Stz,       // comment about the function
  "with"_Stz,          // the prompt command
  nullptr,             // the second word
  bPrompt_with,        // prompt function
  true,                // enabled

  true,                // visible
  "with"_Stz,       // help
  ""_Stz               // description
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool Prompt::bPrompt_with(SePrompt* pPrompt)
{
  Correctness::vTraceWith();

  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
