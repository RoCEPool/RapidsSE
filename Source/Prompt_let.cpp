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
    
    return true;
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  if (nTokens > 2 && pTknVec->pGet(1)->bIsName()) {
    SP_TknVecStr spTknVecStr;
    
    if (! pPrompt->pSession()->m_pNmStrLst)
      pPrompt->pSession()->m_pNmStrLst = new NmStrList;
    
    vTrace("  >%s<\n"_Stz,spTknVecStr.sz(pTknVec,2));
  }
  
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
