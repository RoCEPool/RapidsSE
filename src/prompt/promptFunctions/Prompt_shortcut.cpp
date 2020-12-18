/*============================================================================*/
/* Prompt_shortcut.cpp -- shortcut                                            */
/*============================================================================*/

#include "StorageEngine.h"

PromptFun __Prompt_shortcut (
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
  TokenVec* pTknVec = pPrompt->pInpTknVec();

  ASSERT(pTknVec->pGet(0)->bSame("shortcut"_Stz));

  if (1 == pTknVec->nItems()) {
    ShortCutVec* pShortCutVec = pPrompt->pShortCutVec();
    i8 nShortCuts = pShortCutVec->nItems();
    for (i8 i_ShortCut = 0; i_ShortCut < nShortCuts; i_ShortCut++) {
      ShortCut* pShortCut = pShortCutVec->pGet(i_ShortCut);
      vTrace("  %s : %s \n"_Stz,
             pShortCut->m_sName.azChar(),
             pShortCut->m_sCmd.azChar());
    }
    return true;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  s32 sNm;
  s4K sCmd;
  
  sNm = pTknVec->pGet(1)->szChars();
  
  pTknVec->vStripSemicolon();
  
  pTknVec->vVecToString(sCmd.szStz(),2,false);
  
  ShortCut* pShortCut = new ShortCut(sNm,sCmd);
  
  pPrompt->pShortCutVec()->vAdd(pShortCut);

  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
