/*============================================================================*/
/* Prompt_let.cpp -- let                                                      */
/*============================================================================*/

#include "Prompt.h"

PromptDesc __Prompt_let (
  PF_LET,           // prompt function
  "Prompt_let"_Stz, // name of the prompt function
  "comment"_Stz,    // comment about the function
  "let"_Stz,        // the prompt command
  nullptr,          // the second word
  bPrompt_let,      // prompt function
  true,             // enabled

  true,             // visible
  "let"_Stz,        // help
  ""_Stz            // description
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool Prompt::bPrompt_let(SePrompt* pPrompt)
{
  TokenVec* pTknVec = pPrompt->pTknVec();

  ASSERT((*pTknVec)[0]->bSame("let"_Stz));
  
  i8 nTokens = pTknVec->nItems();

  if (1 == nTokens) {
    if (! pPrompt->pSession()->m_pNmStrLst)
      return true;
    
    NmStrList* pNmStrList = pPrompt->pSession()->m_pNmStrLst;

    for (NmStr* pNmStr = pNmStrList->pHead();
         pNmStr;
         pNmStr = pNmStr->pNextNode()) {
      
      vTrace("  %s = %s\n"_Stz,
             pNmStr->m_sName.aStz(),
             pNmStr->m_sValue.aStz());
    }
    
    return true;
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  if (nTokens > 2 && pTknVec->pGet(1)->bIsName()) {
    SP_TknVecStr spTknVecStr;
    
    if (! pPrompt->pSession()->m_pNmStrLst)
      pPrompt->pSession()->m_pNmStrLst = new NmStrList;
    
    NmStrList* pNmStrList = pPrompt->pSession()->m_pNmStrLst;
    
    i8 i_From = (pTknVec->pGet(2)->bSame("="_Stz)) ? 3 : 2;
    
    vTrace("  >%s<\n"_Stz,spTknVecStr.sz(pTknVec,i_From));
    
    NmStr* pNmStr = new NmStr(pTknVec->pGet(1)->szText(),
                              spTknVecStr.sz(pTknVec,i_From));
    
    pNmStrList->vAppend(pNmStr);
  }
  
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
