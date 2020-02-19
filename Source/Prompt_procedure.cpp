/*============================================================================*/
/* Prompt_procedure.cpp -- define a stored procedure                          */
/*============================================================================*/

#include "Prompt.h"

PromptFun __Prompt_procedure (
  PF_PROCEDURE,           // prompt function
  "Prompt_procedure"_Stz, // name of the prompt function
  "comment"_Stz,      // comment about the function
  "procedure"_Stz,    // the prompt command
  nullptr,            // the second word
  bPrompt_procedure,  // prompt function
  true,               // enabled

  true,               // visible
  "procedure"_Stz,    // help
  ""_Stz              // description
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool Prompt::bPrompt_procedure(SePrompt* pPrompt)
{
  TokenVec* pTokenVec = pPrompt->pTokenVec();
  
  ASSERT((*pTokenVec)[0]->bSame("procedure"_Stz));
  
  s4K sProcText;
  s32 sProcNm = (*pTokenVec)[1]->szTerm();
  pTokenVec->vVecToString(sProcText,0,false);
  
  //vTrace("Procedure: %s\n"_Stz,sProcNm.szStz());
  //vTrace("Procedure: %s\n"_Stz,sProcText.szStz());
  
  Dic* pRootDic = AM::pRootDic();

  Word* pProcWord = pRootDic->pFindWord(sProcNm);
  
//  if ( pProcWord  ) {
//    vTrace(" : %s\n"_Stz,pProcWord->pEnameSet()->szValue("procText"_Stz));
//  }
  
  if ( ! pProcWord  ) {
    Word* pProcedureWord = pRootDic->pFindWord("procedure"_Stz);
    
    pProcWord = new (pRootDic) Word(sProcNm);
    
    pProcWord->vCategory(pProcedureWord);
    
    if (! pProcWord->pEnameSet())
      pProcWord->vEnameSet(new EnameSet);
    
    pProcWord->pEnameSet()->vAdd("procText"_Stz,sProcText);
  }

  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
