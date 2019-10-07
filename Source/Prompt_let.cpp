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

  s32 sName;
  NmStr* pNmStr;
  SP_TknVecStr spTknVecStr;
  
  NmStrList* pNmStrList = pPrompt->pSession()->m_pNmStrLst;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // list all
  
  if (1 == nTokens) {
    if (! pNmStrList)
      return true;
  
    for (NmStr* pNmStr = pNmStrList->pHead();
         pNmStr;
         pNmStr = pNmStr->pNextItem()) {
      
      vTrace("  %s = %s\n"_Stz,
             pNmStr->m_sName.aStz(),
             pNmStr->m_sValue.aStz());
    }
    
    return true;
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // list one
  
  if (2 == nTokens) {
    
    if (! pNmStrList)
      return true;
    
    sName = pTknVec->pGet(1)->szText();
    pNmStr = pNmStrList->pFind(sName);
    if (pNmStr)
      vTrace("  %s = %s\n"_Stz,
             pNmStr->m_sName.aStz(),
             pNmStr->m_sValue.aStz());

    return true;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // delete all
  
  if (nTokens == 3 && pTknVec->pGet(1)->bSame("/"_Stz)
                   && pTknVec->pGet(2)->bSame("delete"_Stz)) {

    if (! pNmStrList)
      return true;
    
    pNmStrList->vDeleteAll();
    
    return true;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // delete one
  
  if (nTokens == 4 && pTknVec->pGet(1)->bIsName()
                   && pTknVec->pGet(2)->bSame("/"_Stz)
                   && pTknVec->pGet(3)->bSame("delete"_Stz)) {
    
    if (! pNmStrList)
      return true;
    
    sName = pTknVec->pGet(1)->szText();
    
    pNmStrList->vDelete(sName);
    
    return true;
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // add / update
  
  if (nTokens > 2 && pTknVec->pGet(1)->bIsName()) {

    sName = pTknVec->pGet(1)->szText();
    
    if (! pNmStrList) {
      pPrompt->pSession()->m_pNmStrLst = new NmStrList;
      pNmStrList = pPrompt->pSession()->m_pNmStrLst;
    }
    
    i8 i_From = (pTknVec->pGet(2)->bSame("="_Stz)) ? 3 : 2;
    
    vTrace("  >%s<\n"_Stz,spTknVecStr.sz(pTknVec,i_From));
    
    pNmStrList->vAddUpdate(sName,spTknVecStr.sz(pTknVec,i_From));
  }
  
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
