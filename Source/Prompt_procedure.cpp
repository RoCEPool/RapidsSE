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
  
  s4K sSmt;
  s32 sProcNm = (*pTokenVec)[1]->szTerm();
  pTokenVec->vToString(sSmt);
  
  vTrace("Procedure: %s\n"_Stz,sProcNm.szStz());
  vTrace("Procedure: %s\n"_Stz,sSmt.szStz());
  
  Dic* pRootDic = AM::pRootDic();

  Word* pProcWord = pRootDic->pFindWord(sProcNm);
  
  if (pProcWord) {
    
  }
  else {
    Word* pStoredProcedureWord = pRootDic->pFindWord("storedProcedure"_Stz);
    
    pProcWord = new (pRootDic) Word(sProcNm);
  }

  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
