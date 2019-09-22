/*============================================================================*/
/* Prompt_show.cpp -- show                                                    */
/*============================================================================*/

#include "Prompt.h"

PromptDesc __Prompt_show (
  PF_SHOW,           // prompt function
  "Prompt_show"_Stz, // name of the prompt function
  "comment"_Stz,     // comment about the function
  "show"_Stz,        // the prompt command
  nullptr,           // the second word
  bPrompt_show,      // prompt function
  true,              // enabled

  true,              // visible
  "show"_Stz,        // help
  ""_Stz             // description
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool Prompt::bPrompt_show(SePrompt* pPrompt)
{
  TokenVec* pTkVec = pPrompt->pTknVec();

  ASSERT((*pTkVec)[0]->bSame("show"_Stz));
  
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
