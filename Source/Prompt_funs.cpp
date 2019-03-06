/*============================================================================*/
/* Prompt_funs.cpp -- prompt funs function                                    */
/*============================================================================*/

#include "Prompt.h"

PromptNote mPromptNote_funs (
  PF_NONE,                     // prompt function
  "Prompt_funs"_Stz,     // name of the prompt function
  "comment"_Stz,         // comment about the function
  "funs"_Stz,            // the prompt command
  nullptr,               // the second word
  bPrompt_funs,          // prompt function
  true,                  // enabled

  true,                  // visible
  "fun help"_Stz         // help
);

extern PromptCatalog gxPromptCatalog;

extern PromptNote mPromptNote_actionSequence;

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool Prompt::bPrompt_funs(SePrompt* pPrompt)
{
  
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
