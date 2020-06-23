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

  if (! ( pTokenVec->pGet(2)->bSame("parameters"_Stz) ||
         pTokenVec->pGet(2)->bSame("begin"_Stz) ) ) {
    
    sProcNm.vConcat("__"_Stz);
    sProcNm.vConcat(pTokenVec->pGet(2)->szChars());
    
#ifdef WITH_TRACE_PROCEDURE
    vTrace("vAddProcedure: three part name %s\n"_Stz,sProcNm.szStz());
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

  if (pwMatchedProc) {

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
  
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // try three-part name
  
  nTokens = pInpTknVec->nTokens();
  if (2 == nTokens)
    return nullptr;
  
  Token* pTkn2_CalledProcNm_b = pInpTknVec->pGet(2);
  
  s256 sProcNm_compound_3 = sProcNm_compound;
  sProcNm_compound_3.vConcat("__"_Stz);
  sProcNm_compound_3.vConcat(pTkn2_CalledProcNm_b->szChars());
  
#ifdef WITH_TRACE_PROCEDURE
  vTrace("bTranslateProcedure: three part name: %s\n"_Stz,sProcNm_compound.szStz());
#endif
  
  pwMatchedProc = pRootDic->pFindWord(sProcNm_compound_3);
  
  if (! pwMatchedProc)
    return nullptr;
  
  if (pwMatchedProc->pCategory() != pProcCatg)
    return nullptr;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // found two-part; correct the command token vector
  pInpTknVec->vRemoveToken(1);
  pInpTknVec->vRemoveToken(1);
  pTkn0_CalledProcNm_a->vUpdateChars(sProcNm_compound_3,pInpTknVec);
  
  if (iJotDebug())
    pInpTknVec->vListTokens();
  
  // found procedure which matches token0_token1
  return pwMatchedProc;
  
}

#define dPV(sNM,pTV) { \
  if (AM::bVerbose()) { \
    s48K sDbgStr; \
    pTV->vDbgStr(sDbgStr); \
    vTrace("%s=%s\n"_Stz,sNM->azChar(),sDbgStr.azChar()); \
  } \
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
    
    if (pwMatchedProc->szWordNm()->bSame("create__unique__index"_Stz))
      vDebugPoint();
  }
  
  i8 i_Cut;
  i8 nInpTokens = pInpTknVec->nTokens();

  sSTMT sOriginalCmd;
  pInpTknVec->vVecToString(sOriginalCmd,0,false);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // get the procedure text and tokenise it
  
  TagSet* pWrdTagSet = pwMatchedProc->pWrdTagSet();
  if (pwMatchedProc->szWordNm()->bSame("drop__table"_Stz)) {
    pWrdTagSet->vDumpSet(true);
    vDebugPoint();
  }
  
  Stz* szPrdText = pWrdTagSet->szValue("procText"_Stz);
  
  TokenVec* pPrdTxtVec = TokenVec::pTokenise(szPrdText);
  
  //pProcVec->vDumpTokens();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // convert procedure into pipeline
  
  Token* pTkn0 = pPrdTxtVec->pGet(0);

  if (pTkn0->bSame("procedure"_Stz))
    pTkn0->vUpdateChars("pipeline"_Stz,pPrdTxtVec);
  else {
    ASSERT(pTkn0->bSame("procedureLocal"_Stz));
    pTkn0->vUpdateChars("pipelineLocal"_Stz,pPrdTxtVec);
  }
  
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // substitute parameters in procedure text with command input arguments
  
  TokenVec* pInpArgVec = nullptr;
  TokenVec* pEtcTknVec = nullptr;

  TokenVec* pPrdPrmVec = TokenVec::pGetSubVec(pPrdTxtVec,"parameters"_Stz,"begin"_Stz);
  
  if (pPrdPrmVec) {
  
    // get the input arguments, e.g. : A B C
    pInpArgVec = TokenVec::pGetSubVec(pInpTknVec,1,nInpTokens-1);
    
    dPV("pInpTknVec"_Stz,pInpTknVec);
    dPV("pInpArgVec"_Stz,pInpArgVec);
    dPV("pPrdTxtVec"_Stz,pPrdTxtVec);
    dPV("pPrdPrmVec"_Stz,pPrdPrmVec);
    
    // test if last parameter is $etc
    bool bHasEtcParam = false;
    i8 i_Etc = pPrdPrmVec->nTokens() - 1;
    Token* pLastPrmTkn = pPrdPrmVec->pGet(i_Etc);
    if (pLastPrmTkn->bSame("$etc"_Stz)) {
      bHasEtcParam = true;
      
      // ... etc stuff ...
      
      // leading-subset of PrcPrmVec is
      //   procedure etcTest ::- [ $arg0 $arg1 ... ] $etc
      
      // leading-subset of InpArgVec is :
      //   etcTest ::- [ arg0 arg1 ... ] tknA tnkB ...
      
      // etc-subset of InpArgVec is :
      //   etcTest ::- arg0 arg1 ... [ tknA tnkB ... ]
      
      // .: collapse InpArgVec [ tknA tnkB ... ] into one string so that
      //    replace of $etc will be replaced with that string
      
      
      // etcTest a b c d e ;
      // substitute ($arg,a) ($etc,b c d e)
      
      pEtcTknVec = TokenVec::pGetSubVec(pInpTknVec,1 + i_Etc,nInpTokens-1);
      dPV("pEtcTknVec"_Stz,pEtcTknVec);
      
      sSTMT sEtcSubStr;
      pEtcTknVec->vToTknString(sEtcSubStr);
      vTrace("sEtcSubStr=%s\n"_Stz,sEtcSubStr.szStz());
      
      pInpArgVec->vTrimTokens(i_Etc);
      dPV("pInpArgVec(t)"_Stz,pInpArgVec);

      pInpArgVec->vAppendToken(sEtcSubStr,TN_STRING);
      dPV("pInpArgVec(+)"_Stz,pInpArgVec);

      vDebugPoint();
    }
    
    if (! bHasEtcParam && pPrdPrmVec->nTokens() != pInpArgVec->nTokens()) {
      delete pPrdTxtVec;
      delete pPrdPrmVec;
      delete pInpArgVec;
      delete pEtcTknVec;
      return false;
    }
    
    if (iJotDebug())
      pPrdTxtVec->vListTokens();
    
    
    TokenVec::vReplace(pPrdTxtVec,pPrdPrmVec,pInpArgVec);
    dPV("pPrdTxtVec(r)"_Stz,pPrdTxtVec);

    
    if (iJotDebug())
      pPrdTxtVec->vListTokens();

    // remove the parameters clause from the modified text vector
    i_Cut = pPrdTxtVec->iFindToken("parameters"_Stz);
    
    if (C_UNKNOWN != i_Cut) {
      for (;;) {
        pPrdTxtVec->vRemoveToken(i_Cut);
        Token* pTest = pPrdTxtVec->pGet(i_Cut);
        if (pTest->bSame("begin"_Stz))
          break;
      }
    }
    
  }
  else {
    i_Cut = pPrdTxtVec->iFindToken("begin"_Stz);
  }

  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // add the <stmt> tag into the modified text vector
  
  i8 i_InsTkn = i_Cut;
  pPrdTxtVec->vInsertToken(i_InsTkn++,"tag"_Stz,TN_NAME);
  pPrdTxtVec->vInsertToken(i_InsTkn++,C_TAG_STMT,TN_NAME);
  pPrdTxtVec->vInsertToken(i_InsTkn++,"="_Stz,TN_NAME);
  pPrdTxtVec->vInsertToken(i_InsTkn++,sOriginalCmd.szStz(),TN_NAME);
  pPrdTxtVec->vInsertToken(i_InsTkn++,"endTag"_Stz,TN_NAME);

  if (iJotDebug())
    pPrdTxtVec->vListTokens();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // stringise the modified text vector into a pipeline statement
  
  pPrdTxtVec->vVecToString(pToPipelineStmt->szStz(),0,true);
  
  if (AM::bVerbose())
    vTrace(" => %s\n"_Stz,pToPipelineStmt->szStz());
  
  delete pPrdTxtVec;
  delete pPrdPrmVec;
  delete pInpArgVec;
  delete pEtcTknVec;

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
