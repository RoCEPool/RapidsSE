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
  
  pTokenVec->vIntegrityCheck();
  
  Token* pTkn1_ProcNm = pTokenVec->pGet(1);
  
  s32 sProcNm = pTkn1_ProcNm->szChars();
  
#ifdef WITH_TRACE_PROCEDURE
  vTrace("vAddProcedure: adding %s\n"_Stz,sProcNm.szStz());
#endif
  
  if (! ( pTokenVec->pGet(2)->bSame("parameters"_Stz) ||
          pTokenVec->pGet(2)->bSame("begin"_Stz) ) ) {
    
    sProcNm.vConcat("__"_Stz);
    sProcNm.vConcat(pTokenVec->pGet(2)->szChars());
    
#ifdef WITH_TRACE_PROCEDURE
    vTrace("vAddProcedure: two part name %s\n"_Stz,sProcNm.szStz());
#endif
    pTokenVec->vRemoveToken(2);
    pTokenVec->vIntegrityCheck();

    pTkn1_ProcNm->vUpdateChars(sProcNm,pTokenVec);
    pTokenVec->vIntegrityCheck();

    //pTokenVec->vDumpTokens();
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
  
  if (! pProcWord->pWrdTagSet())
    pProcWord->vWrdTagSet(new TagSet);

  vParseParameters(pProcWord,pTokenVec);

  pProcWord->pWrdTagSet()->vInsert("procText"_Stz,sProcText);
}

/*----------------------------------------------------------------------------*/
/* find word for procedure matching given name                                */
/*----------------------------------------------------------------------------*/
Word* Procedure::pFindProcedure(TokenVec* pInpTknVec)
{
  _static_
  
  // check we have procedure category defined
  
  Dic* pRootDic = AM::pRootDic();

  Word* pProcCatg = pRootDic->pFindWord(C_PROCEDURE_CATG);
  
  if (! pProcCatg)
    return nullptr;
 
#ifdef WITH_TRACE_PROCEDURE
  vTrace("bTranslateProcedure: %s\n"_Stz,sOriginalCmd.szStz());
#endif

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // try one part name
  
  Token* pTkn0_CalledProcNm_a = pInpTknVec->pGet(0);

  s256 sProcNm_single = pTkn0_CalledProcNm_a->szChars();
  
  Word* pwMatchedProc = pRootDic->pFindWord(sProcNm_single);
  
  if (pwMatchedProc) {
    if (pwMatchedProc->pCategory() != pProcCatg)
      return nullptr;

    // found procedure which matches token0
    return pwMatchedProc;
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // try two-part name

  i8 nTokens = pInpTknVec->nTokens();
  if (1 == nTokens)
    return nullptr;

  Token* pTkn1_CalledProcNm_b = pInpTknVec->pGet(1);

  s256 sProcNm_compound = sProcNm_single;
  sProcNm_compound.vConcat("__"_Stz);
  sProcNm_compound.vConcat(pTkn1_CalledProcNm_b->szChars());

#ifdef WITH_TRACE_PROCEDURE
  vTrace("bTranslateProcedure: two part name: %s\n"_Stz,sProcNm_compound.szStz());
#endif

  pwMatchedProc = pRootDic->pFindWord(sProcNm_compound);

  if (! pwMatchedProc)
    return nullptr;

  if (pwMatchedProc->pCategory() != pProcCatg)
    return nullptr;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // found two-part; correct the command token vector
  
  pInpTknVec->vRemoveToken(1);
  
  pTkn0_CalledProcNm_a->vUpdateChars(sProcNm_compound,pInpTknVec);
  
  if (iJotDebug())
    pInpTknVec->vListTokens();

  // found procedure which matches token0_token1
  return pwMatchedProc;
}

/*----------------------------------------------------------------------------*/
/* translate stored procedure command into a pipeline command                 */
/*----------------------------------------------------------------------------*/
bool Procedure::bTranslateProcedure(String* pToPipelineStmt,
                                    TokenVec* pInpTknVec,
                                    Word* pwMatchedProc)
{
  _static_

  if (AM::bVerbose()) {
    vTrace("Translating Proc: %s\n"_Stz,pwMatchedProc->szWordNm());
    
    if (pwMatchedProc->szWordNm()->bSame("drop__table"_Stz))
      vDebugPoint();
  }
  
  i8 i_Cut;
  i8 nTokens = pInpTknVec->nTokens();

  s4K sOriginalCmd;
  pInpTknVec->vVecToString(sOriginalCmd,0,false);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // get the procedure text and tokneise it
  
  TagSet* pWrdTagSet = pwMatchedProc->pWrdTagSet();
  if (pwMatchedProc->szWordNm()->bSame("drop__table"_Stz)) {
    pWrdTagSet->vDumpSet(true);
    vDebugPoint();
  }
  
  Stz* szProcText = pWrdTagSet->szValue("procText"_Stz);
  
  TokenVec* pProcTextVec = TokenVec::pTokenise(szProcText);
  
  //pProcVec->vDumpTokens();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // convert procedure into pipeline
  
  Token* pTkn0 = pProcTextVec->pGet(0);

  if (pTkn0->bSame("procedure"_Stz))
    pTkn0->vUpdateChars("pipeline"_Stz,pProcTextVec);
  else {
    ASSERT(pTkn0->bSame("procedureLocal"_Stz));
    pTkn0->vUpdateChars("pipelineLocal"_Stz,pProcTextVec);
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // substitute parameters in procedure text with command arguments
  
  TokenVec* pArgsVec = nullptr;

  TokenVec* pParamsVec = TokenVec::pGetSubVec(pProcTextVec,"parameters"_Stz,"begin"_Stz);
  
  if ( pParamsVec ) {
  
    pArgsVec = TokenVec::pGetSubVec(pInpTknVec,1,nTokens-1);
    
    if (pParamsVec->nTokens() != pArgsVec->nTokens()) {
      delete pProcTextVec;
      delete pParamsVec;
      delete pArgsVec;
      return false;
    }
    
    if (iJotDebug())
      pProcTextVec->vListTokens();
    TokenVec::vReplace(pProcTextVec,pParamsVec,pArgsVec);
    if (iJotDebug())
      pProcTextVec->vListTokens();

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // remove the parameters clause from the modified text vector
    
    i_Cut = pProcTextVec->iFindToken("parameters"_Stz);
    
    if (C_UNKNOWN != i_Cut) {
      for (;;) {
        pProcTextVec->vRemoveToken(i_Cut);
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

  if (iJotDebug())
    pProcTextVec->vListTokens();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // stringise the modified text vector into a pipeline statement
  
  pProcTextVec->vVecToString(pToPipelineStmt->szStz(),0,true);
  
  if (AM::bVerbose())
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
  
  TagSet* pTagSet = pProcWord->pWrdTagSet(true);
  
  for (i_Tkn++; i_Tkn < nTokens; i_Tkn++) {
    pTkn = pTokenVec->pGet(i_Tkn);
    
    if (pTkn->bSame("begin"_Stz))
      break;
    
#ifdef WITH_TRACE_PROCEDURE
    vTrace("%s: adding parameter %s\n"_Stz,pProcWord->szWordNm(),pTkn->szChars());
#endif
    
    pTagSet->vInsert(pTkn->szChars());
  }

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
