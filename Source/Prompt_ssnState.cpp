/*============================================================================*/
/* Prompt_ssnState.hpp -- Print Session State Information                     */
/*============================================================================*/

#include "Prompt.h"
#include "AM.hpp"


PromptDesc __Prompt_ssnState (
  PF_SSNSTATE,            // prompt function
  "Prompt_ssnState"_Stz,  // name of the prompt function
  "comment"_Stz,          // comment about the function
  "ssnState"_Stz,         // the prompt command
  nullptr,                // the second word
  bPrompt_ssnState,       // prompt function
  true,                   // enabled

  true,                   // visible
  "ssnState"_Stz,         // help
  ""_Stz                  // description
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool Prompt::bPrompt_ssnState(SePrompt* pPrompt)
{
  Token* pArg = nullptr;
  
  TokenVec* pTokenVec = pPrompt->pTknVec();
  i8 nTokens = pTokenVec->nItems();

  if (nTokens == 2)
    pArg = pTokenVec->pGet(1);
  
  AM::pSessionArray()->vDump();
  
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
