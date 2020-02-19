/*============================================================================*/
/* Procedure.cpp -- Procedure Helper                                          */
/*============================================================================*/

#include "Pipelines.h"

/*----------------------------------------------------------------------------*/
/* add word for procedure in root dictionary                                  */
/*----------------------------------------------------------------------------*/
void Procedure::vAddProcedure(Stz* szProcNm, Stz* szProcText)
{
  _static_
  
  Dic* pRootDic = AM::pRootDic();

  Word* pProceCatg = pRootDic->pFindWord("procedure"_Stz);
  
  Word* pProcWord = pRootDic->pFindWord(szProcNm);
  if (pProcWord)
    return;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  pProcWord = new (pRootDic) Word(szProcNm);
  
  pProcWord->vCategory(pProceCatg);
  
  if (! pProcWord->pEnameSet())
    pProcWord->vEnameSet(new EnameSet);
  
  pProcWord->pEnameSet()->vAdd("procText"_Stz,szProcText);
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
