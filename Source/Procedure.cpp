/*============================================================================*/
/* Procedure.cpp -- Procedure Helper                                          */
/*============================================================================*/

#include "Pipelines.h"

/*----------------------------------------------------------------------------*/
/* add word for procedure in root dictionary                                  */
/*----------------------------------------------------------------------------*/
void Procedure::vAddProcedure(TokenVec* pTokenVec)
{
  _static_
  
  s32 sProcNm = (*pTokenVec)[1]->szTerm();
  
  s4K sProcText;
  pTokenVec->vVecToString(sProcText,0,false);

  
  Dic* pRootDic = AM::pRootDic();

  Word* pProceCatg = pRootDic->pFindWord("procedure"_Stz);
  
  Word* pProcWord = pRootDic->pFindWord(sProcNm);
  if (pProcWord)
    return;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  pProcWord = new (pRootDic) Word(sProcNm);
  
  pProcWord->vCategory(pProceCatg);
  
  if (! pProcWord->pEnameSet())
    pProcWord->vEnameSet(new EnameSet);
  
  pProcWord->pEnameSet()->vAdd("procText"_Stz,sProcText);
}

/*----------------------------------------------------------------------------*/
/* find word for procedure matching given name                                */
/*----------------------------------------------------------------------------*/
Word* Procedure::pFindProcedure(Stz* szProcNm, DicStack* pDicStack)
{
  _static_
  
  Word* pProcedureWord = pDicStack->pFindWord(szProcNm);
  
  if (! pProcedureWord)
    return nullptr;
  
  Word* pProcedureCatg = pDicStack->pFindWord("procedure"_Stz);

  if (pProcedureWord->pCategory() != pProcedureCatg)
    return nullptr;

  return pProcedureWord;
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
