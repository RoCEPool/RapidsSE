/*============================================================================*/
/* Prompt_semantic.cpp -- semantic context report                             */
/*============================================================================*/

#include "Prompt.h"
#include "AM.hpp"

PromptDesc __Prompt_semantic (
  PF_SEMANTIC,            // prompt function
  "Prompt_semantic"_Stz,  // name of the prompt function
  "comment"_Stz,          // comment about the function
  "semantic"_Stz,         // the prompt command
  nullptr,                // the second word
  bPrompt_semantic,       // prompt function
  true,                   // enabled

  true,                   // visible
  "semantic"_Stz,         // help
  ""_Stz                  // description
);


/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool Prompt::bPrompt_semantic(SePrompt* pPrompt)
{
  Session*  pSession  = pPrompt->pSession();

  Semantic* pSemantic = pSession->m_pSemantic;
  
  vTrace("Semantic:\n"_Stz);

  EnameSet* pEnameSet = pSemantic->pEnmSet();

  pEnameSet->vPrint();

  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
