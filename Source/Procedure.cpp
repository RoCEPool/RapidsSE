/*============================================================================*/
/* Procedure.cpp -- Procedure Helper                                          */
/*============================================================================*/

#include "DynamoCompiler.h"

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
  
#ifdef WITH_TRACE_PROCEDURE
  vTrace("vAddProcedure: adding %s\n"_Stz,sProcNm.szStz());
#endif
  
  if (! ( pTokenVec->pGet(2)->bSame("parameters"_Stz) ||
          pTokenVec->pGet(2)->bSame("begin"_Stz) ) ) {
    sProcNm.vConcat("__"_Stz);
    sProcNm.vConcat(pTokenVec->pGet(2)->szTerm());
    
#ifdef WITH_TRACE_PROCEDURE
    vTrace("vAddProcedure: two part name %s\n"_Stz,sProcNm.szStz());
#endif
    pTokenVec->vCutToken(2);
    
    pTkn1_ProcNm->vCopy(sProcNm);
    
    pTokenVec->vDumpTokens();
  }
  
  
  s4K sProcText;
  pTokenVec->vVecToString(sProcText,0,false);

  
  Dic* pRootDic = AM::pRootDic();

  Word* pProcCatg = pRootDic->pFindWord(C_PROCEDURE_CATG);
  
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
  
  Word* pProcCatg = pDicStack->pFindWord(C_PROCEDURE_CATG);

  if (pProcWord->pCategory() != pProcCatg)
    return nullptr;

  return pProcWord;
}

/*----------------------------------------------------------------------------*/
/* translate stored procedure command into a pipeline command                 */
/*----------------------------------------------------------------------------*/
bool Procedure::bTranslateProcedure(String* pToPipelineStmt,
                                    TokenVec* pCmdTokenVec)
{
  _static_

  // check we have prcoedure category defined
  
  Dic* pRootDic = AM::pRootDic();

  Word* pProcCatg = pRootDic->pFindWord(C_PROCEDURE_CATG);
  i8 i_Cut;
  
  if (! pProcCatg)
    return false;
 
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // debug: print the original command typed in by the user
  
  s4K sOriginalCmd;
  pCmdTokenVec->vVecToString(sOriginalCmd,0,false);

#ifdef WITH_TRACE_PROCEDURE
  vTrace("bTranslateProcedure: %s\n"_Stz,sOriginalCmd.szStz());
#endif

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // attempt to find the procedure which matches the command
  
  i8 nTokens = pCmdTokenVec->nTokens();

  Token* pTkn_ProcNm = pCmdTokenVec->pGet(0);

  s4K sProcNm = pTkn_ProcNm->szTerm();
  
  // one-part name
  Word* pProcWord = pRootDic->pFindWord(sProcNm);
  
  if (! pProcWord) {
    //return false;

    if (1 == nTokens)
      return false;

    // try two-part name
    Token* pTkn1_ProcNmTwo = pCmdTokenVec->pGet(1);

    sProcNm.vConcat("__"_Stz);
    sProcNm.vConcat(pTkn1_ProcNmTwo->szTerm());

#ifdef WITH_TRACE_PROCEDURE
    vTrace("bTranslateProcedure: two part name: %s\n"_Stz,sProcNm.szStz());
#endif

    pProcWord = pRootDic->pFindWord(sProcNm);
    if (! pProcWord)
      return false;
    
    // -- -- -- -- -- --
    // found two-part; correct the token vector
    pCmdTokenVec->vCutToken(1);
    
    pTkn_ProcNm->vCopy(sProcNm);
    
    pCmdTokenVec->vDumpTokens();

    Correctness::vDebugPoint();
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // test that the found word is a procedure

  if (pProcWord->pCategory() != pProcCatg)
    return false;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // get the procedure text and toeknise it
  EnameSet* pEnameSet = pProcWord->pEnameSet();
  
  Stz* szProcText = pEnameSet->szValue("procText"_Stz);
  
  TokenVec* pProcTextVec = Tokeniser::pTokenise(szProcText);
  
  //pProcVec->vDumpTokens();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // convert procedure into pipeline
  
  Token* pTkn0 = pProcTextVec->pGet(0);

  pTkn0->vCopy("pipeline"_Stz);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // substitute parameters in procedure text with command arguments
  
  TokenVec* pParamsVec = Tokeniser::pSubVec(pProcTextVec,"parameters"_Stz,"begin"_Stz);
  TokenVec* pArgsVec = nullptr;
  
  if ( pParamsVec ) {
  
    pArgsVec = Tokeniser::pSubVec(pCmdTokenVec,1,nTokens-1);
    
    if (pParamsVec->nTokens() != pArgsVec->nTokens()) {
      delete pProcTextVec;
      delete pParamsVec;
      delete pArgsVec;
      return false;
    }
    
    Tokeniser::vReplace(pProcTextVec,pParamsVec,pArgsVec);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // remove the parameters clause from the modified text vector
    
    i_Cut = pProcTextVec->iFindToken("parameters"_Stz);
    
    if (C_UNKNOWN != i_Cut) {
      for (;;) {
        pProcTextVec->vCutToken(i_Cut);
        Token* pTest = pProcTextVec->pGet(i_Cut);
        if (pTest->bSame("begin"_Stz))
          break;
      }
    }
    
  }
  else {
    i_Cut = pProcTextVec->iFindToken("begin"_Stz);
  }

  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // add the <stmt> tag into the modified text vector
  
  i8 i_InsTkn = i_Cut;
  pProcTextVec->vInsertToken(i_InsTkn++,"tag"_Stz,TN_NAME);
  pProcTextVec->vInsertToken(i_InsTkn++,C_TAG_STMT,TN_NAME);
  pProcTextVec->vInsertToken(i_InsTkn++,"="_Stz,TN_NAME);
  pProcTextVec->vInsertToken(i_InsTkn++,sOriginalCmd.szStz(),TN_NAME);
  pProcTextVec->vInsertToken(i_InsTkn++,"endTag"_Stz,TN_NAME);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // stringise the modified text vector into a pipeline statement
  
  pProcTextVec->vVecToString(pToPipelineStmt->szStz(),0,true);
  vTrace(" => %s\n"_Stz,pToPipelineStmt->szStz());
  
  delete pProcTextVec;
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
    
#ifdef WITH_TRACE_PROCEDURE
    vTrace("%s: adding parameter %s\n"_Stz,pProcWord->szWordNm(),pTkn->szTerm());
#endif
    
    pEnameSet->vAdd(pTkn->szTerm());
  }

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
