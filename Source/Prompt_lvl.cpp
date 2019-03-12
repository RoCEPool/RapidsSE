/*============================================================================*/
/* Prompt_lvl.cpp -- random number helper                                     */
/*============================================================================*/

#include "Prompt.h"

PromptNote mPromptNote_lvl (
  PF_LVL,           // prompt function
  "Prompt_lvl"_Stz, // name of the prompt function
  "comment"_Stz,    // comment about the function
  "lvl"_Stz,        // the prompt command
  nullptr,          // the second word
  bPrompt_lvl,      // prompt function
  true,             // enabled

  true,             // visible
  "lvl"_Stz,        // help
  ""_Stz            // description
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool Prompt::bPrompt_lvl(SePrompt* pPrompt)
{
  i8 i_Next = 0;
  
  TokenVec* pTokenVec = pPrompt->pTknVec();
  i8 iNumTokens = pTokenVec->nItems();
  
  if (2 == iNumTokens) {
    Token* pTkn0 = pTokenVec->pGet(1);
    if (pTkn0->aText()->bIsSomeTrue())
      i_Next = C_UNKNOWN;
  }
  
  for (i8 i = 0; i < 128; i++) {
    i8 iLevel = Random::iLogLevel(3,C_FLAT_FREQ + 1,i_Next) + 1;
    vTrace("%lld "_Stz,iLevel);
    if (0 == (i+1)%8)
      vTrace("\n"_Stz);
  }
  
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
