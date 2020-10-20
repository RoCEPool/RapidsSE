/*============================================================================*/
/* Prompt_bg.cpp -- start prompt background thread                            */
/*============================================================================*/

#include "Prompt.h"

PromptFun __Prompt_bg (
  PF_BYE,            // prompt function
  "Prompt_bg"_Stz,   // name of the prompt function
  "comment"_Stz,     // comment about the function
  "bg"_Stz,          // the prompt command
  nullptr,           // the second word
  bPrompt_bg,        // prompt function
  true,              // enabled

  true,              // visible
  "bg"_Stz,          // help
  ""_Stz             // description
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool Prompt::bPrompt_bg(SePrompt* pPrompt)
{
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
