/*============================================================================*/
/* Prompt_dumpTokens.cpp -- dump tokens on/off                                */
/*============================================================================*/

#include "Prompt.h"

PromptFun __Prompt_dumpTokens (
  PF_DUMPTOKENS,           // prompt function
  "Prompt_dumpTokens"_Stz, // name of the prompt function
  "comment"_Stz,           // comment about the function
  "dumpTokens"_Stz,        // the prompt command
  nullptr,                 // the second word
  bPrompt_dumpTokens,      // prompt function
  true,                    // enabled

  true,                 // visible
  "errno"_Stz,          // help
  ""_Stz                // description
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool Prompt::bPrompt_dumpTokens(SePrompt* pPrompt)
{
  TokenVec* pInpTknVec = pPrompt->pInpTknVec();
  i8 iNumTokens = pInpTknVec->nItems();
  
  if (2 == iNumTokens) {
    Token* pTkn1 = pInpTknVec->pGet(1);
    if (pTkn1->szChars()->bIsSomeTrue())
      pPrompt->vDumpTokens(true);
    else
      pPrompt->vDumpTokens(false);
  }
  else {
    if (pPrompt->bDumpTokens())
      vTrace(" on\n"_Stz);
    else
      vTrace(" off\n"_Stz);
  }
  
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
