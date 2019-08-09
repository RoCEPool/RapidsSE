/*============================================================================*/
/* Prompt_shortcut.cpp -- shortcut                                            */
/*============================================================================*/

#include "Prompt.h"

PromptDesc __Prompt_shortcut (
  PF_SHORTCUT,        // prompt function
  "Prompt_shortcut"_Stz, // name of the prompt function
  "comment"_Stz,      // comment about the function
  "shortcut"_Stz,     // the prompt command
  nullptr,            // the second word
  bPrompt_shortcut,   // prompt function
  true,               // enabled

  true,               // visible
  "shortcut"_Stz,     // help
  ""_Stz              // description
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool Prompt::bPrompt_shortcut(SePrompt* pPrompt)
{
  TokenVec* pTokenVec = pPrompt->pTknVec();

  ASSERT((*pTokenVec)[0]->bSame("shortcut"_Stz));
  

  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
