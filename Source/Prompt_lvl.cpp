/*============================================================================*/
/* Prompt_lvl.cpp -- random number helper                                     */
/*============================================================================*/

#include "Prompt.h"

PromptFun __Prompt_lvl (
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
  
  TokenVec* pInpTknVec = pPrompt->pInpTknVec();
  i8 iNumTokens = pInpTknVec->nItems();
  
  if (2 == iNumTokens) {
    Token* pTkn0 = pInpTknVec->pGet(1);
    if (pTkn0->szChars()->bIsSomeTrue())
      i_Next = C_UNKNOWN;
  }
  
  i8 nLvlLevels = AM::nLvlLevels();
  i8 iLvlRatio  = AM::iLvlRatio();

  for (i8 i = 0; i < 128; i++) {
    i8 iLevel = RndLib::iLogLevel(nLvlLevels,iLvlRatio,i_Next);
    vTrace("%lld "_Stz,iLevel);
    if (0 == (i+1)%8)
      vTrace("\n"_Stz);
  }
  
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
