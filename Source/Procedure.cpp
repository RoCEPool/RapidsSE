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
  
  Token* pTkn1_ProcNm = pTokenVec->pGet(1);
  
  s32 sProcNm = pTkn1_ProcNm->szTerm();
  
  vTrace("vAddProcedure: adding %s\n"_Stz,sProcNm.szStz());

  if (! ( pTokenVec->pGet(2)->bSame("parameters"_Stz) ||
          pTokenVec->pGet(2)->bSame("begin"_Stz) ) ) {
    sProcNm.vConcat("__"_Stz);
    sProcNm.vConcat(pTokenVec->pGet(2)->szTerm());
    vTrace("vAddProcedure: two part name %s\n"_Stz,sProcNm.szStz());
    
    pTokenVec->vCutToken(2);
    
    pTkn1_ProcNm->vCopy(sProcNm);
    
    pTokenVec->vDumpTokens();
  }
  
  
  s4K sProcText;
  pTokenVec->vVecToString(sProcText,0,false);

  
  Dic* pRootDic = AM::pRootDic();

  Word* pProcCatg = pRootDic->pFindWord("procedure"_Stz);
  
  Word* pProcWord = pRootDic->pFindWord(sProcNm);
  if (pProcWord)
    return;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  
  pProcWord = new (pRootDic) Word(sProcNm);
  
  pProcWord->vCategory(pProcCatg);
  
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
/* translate stored procedure into a pipeline command                         */
/*----------------------------------------------------------------------------*/
bool Procedure::bTranslateProcedure(String* pPipelineStmt, TokenVec* pTokenVec)
{
  _static_

  Dic* pRootDic = AM::pRootDic();

  Word* pProcCatg = pRootDic->pFindWord("procedure"_Stz);
  if (! pProcCatg)
    return false;
 
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  s4K sOriginalCmd;
  
  pTokenVec->vVecToString(sOriginalCmd,0,false);
  vTrace("bTranslateProcedure: %s\n"_Stz,sOriginalCmd.szStz());

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  i8 nTokens = pTokenVec->nTokens();

  Token* pTkn_ProcNm = pTokenVec->pGet(0);

  s4K sProcNm = pTkn_ProcNm->szTerm();
  
  Word* pProcWord = pRootDic->pFindWord(sProcNm);
  
  if (! pProcWord) {
    //return false;

    if (1 == nTokens)
      return false;

    Token* pTkn1_ProcNmTwo = pTokenVec->pGet(1);

    sProcNm.vConcat("__"_Stz);
    sProcNm.vConcat(pTkn1_ProcNmTwo->szTerm());
    vTrace("bTranslateProcedure: two part name: %s\n"_Stz,sProcNm.szStz());

    pProcWord = pRootDic->pFindWord(sProcNm);
    if (! pProcWord)
      return false;
    
    
    pTokenVec->vCutToken(1);
    
    pTkn_ProcNm->vCopy(sProcNm);
    
    pTokenVec->vDumpTokens();

    Correctness::vDebugPoint();
  }
  
  if (pProcWord->pCategory() != pProcCatg)
    return false;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  EnameSet* pEnameSet = pProcWord->pEnameSet();
  
  Stz* szProcText = pEnameSet->szValue("procText"_Stz);
  
  TokenVec* pProcVec = Tokeniser::pTokenise(szProcText);
  
  //pProcVec->vDumpTokens();
  
  Token* pTkn0 = pProcVec->pGet(0);

  pTkn0->vCopy("pipeline"_Stz);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  TokenVec* pParamsVec = Tokeniser::pSubVec(pProcVec,"parameters"_Stz,"begin"_Stz);
//  if (pParamsVec)
//    pParamsVec->vDumpTokens();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  TokenVec* pArgsVec = Tokeniser::pSubVec(pTokenVec,1,nTokens-1);
//  if (pArgsVec)
//    pArgsVec->vDumpTokens();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  Tokeniser::vReplace(pProcVec,pParamsVec,pArgsVec);
  //pProcVec->vDumpTokens();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  i8 i_Cut = pProcVec->iFindToken("parameters"_Stz);
  if (C_UNKNOWN != i_Cut) {
    for (;;) {
      pProcVec->vCutToken(i_Cut);
      Token* pTest = pProcVec->pGet(i_Cut);
      if (pTest->bSame("begin"_Stz))
        break;
    }
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  pProcVec->vVecToString(pPipelineStmt->szStz(),0,true);
  //vTrace(" => %s\n"_Stz,sPipelineSmt.szStz());
  
  delete pProcVec;
  delete pParamsVec;
  delete pArgsVec;

  return true;
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
