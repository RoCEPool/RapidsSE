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
  TokenVec* pTknVec = pPrompt->pTknVec();

  ASSERT((*pTknVec)[0]->bSame("shortcut"_Stz));

  s32 sNm;
  s4K sCmd;
  
  sNm = pTknVec->pGet(1)->aText();
  
  pTknVec->vStripSemicolon();
  pTknVec->vSetTokens();

  
  pTknVec->vToString(sCmd.aStz(),2);
  
  ShortCut* pShortCut = new ShortCut(sNm,sCmd);
  
  pPrompt->pShortCutVec()->vAdd(pShortCut);

  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
