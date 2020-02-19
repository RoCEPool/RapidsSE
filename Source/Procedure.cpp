/*============================================================================*/
/* Procedure.cpp -- Procedure Helper                                          */
/*============================================================================*/

#include "Pipelines.h"

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* add word for procedure in root dictionary                                  */
/*----------------------------------------------------------------------------*/
void Procedure::vAddProcedure(TokenVec* pTokenVec)
{
  _static_
  
  s32 sProcNm = (*pTokenVec)[1]->szTerm();
  
  
  // end test
  
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

  vParseParameters(pProcWord,pTokenVec);

  pProcWord->pEnameSet()->vAdd("procText"_Stz,sProcText);
}

/*----------------------------------------------------------------------------*/
/* find word for procedure matching given name                                */
/*----------------------------------------------------------------------------*/
Word* Procedure::pFindProcedure(Stz* szProcNm, DicStack* pDicStack)
{
  _static_
  
  Word* pProcWord = pDicStack->pFindWord(szProcNm);
  
  if (! pProcWord)
    return nullptr;
  
  Word* pProcCatg = pDicStack->pFindWord("procedure"_Stz);

  if (pProcWord->pCategory() != pProcCatg)
    return nullptr;

  return pProcWord;
}


/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* parse parameters of procedure                                              */
/*----------------------------------------------------------------------------*/
void Procedure::vParseParameters(Word* pProcWord, TokenVec* pTokenVec)
{
  _static_
  
  i8 nTokens = pTokenVec->nTokens();

  Token* pTkn = nullptr;
  
  i8 i_Tkn = 0;
  for (; i_Tkn < nTokens; i_Tkn++) {
    pTkn = pTokenVec->pGet(i_Tkn);
    if (pTkn->bSame("parameters"_Stz))
      break;
  }
  
  if (! pTkn)
    return;
  
  EnameSet* pEnameSet = pProcWord->pEnameSet(true);
  
  for (i_Tkn++; i_Tkn < nTokens; i_Tkn++) {
    pTkn = pTokenVec->pGet(i_Tkn);
    
    if (pTkn->bSame("begin"_Stz))
      break;
    
    vTrace("%s: adding parameter %s\n"_Stz,pProcWord->szWordNm(),pTkn->szTerm());
    pEnameSet->vAdd(pTkn->szTerm());
  }

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
