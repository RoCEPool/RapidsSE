/*============================================================================*/
/* Prompt_ssnState.hpp -- Print Session State Information                     */
/*============================================================================*/

#include "StorageEngine.h"



PromptFun __Prompt_ssnState (
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
  
  TokenVec* pInpTknVec = pPrompt->pInpTknVec();
  i8 nTokens = pInpTknVec->nItems();

  if (nTokens == 2)
    pArg = pInpTknVec->pGet(1);
  
  AM::pSessionArray()->vDump();
  
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
