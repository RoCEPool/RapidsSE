/*============================================================================*/
/* Prompt_ops.cpp -- prompt op list function                                  */
/*============================================================================*/

#include "Prompt.h"

PromptDesc __Prompt_ops (
  PF_FUNS,               // prompt function
  "Prompt_ops"_Stz,      // name of the prompt function
  "comment"_Stz,         // comment about the function
  "ops"_Stz,             // the prompt command
  nullptr,               // the second word
  bPrompt_ops,           // prompt function
  true,                  // enabled

  true,                  // visible
  "ops"_Stz,             // help
  ""_Stz                 // description
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool Prompt::bPrompt_ops(SePrompt* pPrompt)
{

  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
