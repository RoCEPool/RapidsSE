/*============================================================================*/
/* Prompt_floats.cpp -- prompt floats function                                */
/*============================================================================*/

#include "Prompt.h"

PromptNote mPromptNote_floats (
  PF_FLOATS,             // prompt function
  "Prompt_floats"_Stz,   // name of the prompt function
  "comment"_Stz,         // comment about the function
  "floats"_Stz,          // the prompt command
  nullptr,               // the second word
  bPrompt_floats,        // prompt function
  true,                  // enabled

  true,                  // visible
  "floats"_Stz,          // help
  ""_Stz                 // description
);

extern PromptCatalog gxPromptCatalog;

extern PromptNote mPromptNote_actionSequence;

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool Prompt::bPrompt_floats(SePrompt* pPrompt)
{

  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
