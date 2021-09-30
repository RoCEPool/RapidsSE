/*============================================================================*/
/* Procedure.cpp -- Procedure Helper                                          */
/*============================================================================*/

#include "BASE.h"

#include "Procedure.hpp"

#include "TagConstants.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE,Procedure,CN_PROCEDURE)
__aNewFunction(Procedure,CN_PROCEDURE)
__vDeleteFunction(Procedure,CN_PROCEDURE)
__aFooterFunction(Procedure)


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Procedure::Procedure(TokenVec* pTokenVec)
: Object(CN_PROCEDURE)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PROCEDURE,_FOOTER_);

  m_pProcWord = nullptr;
  m_pTagSet   = nullptr;
  
  m_pTokenVec = pTokenVec;
  m_nTokens = m_pTokenVec->nItems();

  m_i_Tkn = 0;
  m_pToken = m_pTokenVec->pGet(m_i_Tkn);
  m_pProcNmToken = nullptr;
  
  _checkInitialised_(CN_PROCEDURE);
  
  m_pTokenVec->vIntegrityCheck();


  _postcondition_(_ADDRESS_,_MAGIC_,CN_PROCEDURE,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Procedure::~Procedure(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PROCEDURE,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* add word for procedure in root dictionary                                  */
/*----------------------------------------------------------------------------*/
void Procedure::vAddProcedure(TokenVec* pTokenVec)
{
  _static_
  
  Procedure* pProcedure = new Procedure(pTokenVec);
  
#ifdef WITH_PROCEDURE_2
#else
#endif
  
  //pProcedure->vParseAndAdd();
  pProcedure->bParseAndAdd_2();

  delete pProcedure;
}

#define dPV(sNM,pTV) { \
  if (AM::TRACE::bVerbose(C_VB_PROCEDURE)) { \
    s48K sDbgStr; \
    pTV->vDbgStr(sDbgStr); \
    vTrace("%s=%s\n"_Stz,sNM->azChar(),sDbgStr.azChar()); \
  } \
}

/*----------------------------------------------------------------------------*/
/* translate stored procedure command into a pipeline command                 */
/*----------------------------------------------------------------------------*/
bool Procedure::bTranslateToPipeline(String* pToPipelineStmt,
                                     TokenVec* pTokenVec)
{
  _static_

  
  Word* pwMatchedProc = pFindProcedure(pTokenVec);
  if (! pwMatchedProc)
    return false;

  if (AM::TRACE::bVerbose(C_VB_PROCEDURE)) {
    vTrace("Translating Proc: %s\n"_Stz,pwMatchedProc->szWordNm());
  }

  i8 i_Cut;
  i8 nInpTokens = pTokenVec->nTokens();

  sSTMT sOriginalCmd;
  pTokenVec->vVecToString(sOriginalCmd,0,false);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // get the procedure text and tokenise it
  
  TagSet* pWrdTagSet = pwMatchedProc->pWrdTagSet();
//  if (pwMatchedProc->szWordNm()->bSame("drop__table"_Stz)) {
//    pWrdTagSet->vDumpSet(true);
//    vDebugPoint();
//  }
  
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
    pInpArgVec = TokenVec::pGetSubVec(pTokenVec,1,nInpTokens-1);
    
    dPV("pTokenVec"_Stz,pTokenVec);
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
      
      pEtcTknVec = TokenVec::pGetSubVec(pTokenVec,1 + i_Etc,nInpTokens-1);
      dPV("pEtcTknVec"_Stz,pEtcTknVec);
      
      sSTMT sEtcSubStr;
      pEtcTknVec->vToTknString(sEtcSubStr);
      //vTrace("sEtcSubStr=%s\n"_Stz,sEtcSubStr.szStz());
      
      if (i_Etc < pInpArgVec->nItems())
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

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // add the <lowMemory> tag into the modified text vector
  
  if (pWrdTagSet->bHasName(C_TAG_LOWMEMORY)) {
    pPrdTxtVec->vInsertToken(i_InsTkn++,"tag"_Stz,TN_NAME);
    pPrdTxtVec->vInsertToken(i_InsTkn++,C_TAG_LOWMEMORY,TN_NAME);
    pPrdTxtVec->vInsertToken(i_InsTkn++,"="_Stz,TN_NAME);
    pPrdTxtVec->vInsertToken(i_InsTkn++,"1"_Stz,TN_NAME);
    pPrdTxtVec->vInsertToken(i_InsTkn++,"endTag"_Stz,TN_NAME);
  }
  
  if (pWrdTagSet->bHasName("<include_0>"_Stz)) {
    pPrdTxtVec->vInsertToken(i_InsTkn++,"tag"_Stz,TN_NAME);
    pPrdTxtVec->vInsertToken(i_InsTkn++,"<include_0>"_Stz,TN_NAME);
    pPrdTxtVec->vInsertToken(i_InsTkn++,"="_Stz,TN_NAME);
    pPrdTxtVec->vInsertToken(i_InsTkn++,
                             pWrdTagSet->szValue("<include_0>"_Stz),
                             TN_NAME);
    pPrdTxtVec->vInsertToken(i_InsTkn++,"endTag"_Stz,TN_NAME);
  }

  if (pWrdTagSet->bHasName("<include_1>"_Stz)) {
    pPrdTxtVec->vInsertToken(i_InsTkn++,"tag"_Stz,TN_NAME);
    pPrdTxtVec->vInsertToken(i_InsTkn++,"<include_1>"_Stz,TN_NAME);
    pPrdTxtVec->vInsertToken(i_InsTkn++,"="_Stz,TN_NAME);
    pPrdTxtVec->vInsertToken(i_InsTkn++,
                             pWrdTagSet->szValue("<include_1>"_Stz),
                             TN_NAME);
    pPrdTxtVec->vInsertToken(i_InsTkn++,"endTag"_Stz,TN_NAME);
  }

  if (iJotDebug())
    pPrdTxtVec->vListTokens();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // stringise the modified text vector into a pipeline statement
  
  pPrdTxtVec->vVecToString(pToPipelineStmt->szStz(),0,true);
  
  if (AM::TRACE::bVerbose(C_VB_PROCEDURE))
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
/* find word for procedure matching given name                                */
/*----------------------------------------------------------------------------*/
Word* Procedure::pFindProcedure(TokenVec* pTokenVec)
{
  _static_
  
  // check we have procedure category defined
  
  Dic* pDicRoot = AM::DIC::pRoot();

  Word* pProcCatg = pDicRoot->pFindWord(C_PROCEDURE_CATG);
  
  if (! pProcCatg)
    return nullptr;
 
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // try one part name
  
  Token* pTkn0_CalledProcNm_a = pTokenVec->pGet(0);

  s256 sProcNm_single = pTkn0_CalledProcNm_a->szChars();
  
  Word* pwMatchedProc = pDicRoot->pFindWord(sProcNm_single);
  
  if (pwMatchedProc) {
    if (pwMatchedProc->pCategory() != pProcCatg)
      return nullptr;

    // found procedure which matches token0
    return pwMatchedProc;
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // try two-part name

  i8 nTokens = pTokenVec->nTokens();
  if (1 == nTokens)
    return nullptr;

  Token* pTkn1_CalledProcNm_b = pTokenVec->pGet(1);

  s256 sProcNm_compound = sProcNm_single;
  sProcNm_compound.vConcat("__"_Stz);
  sProcNm_compound.vConcat(pTkn1_CalledProcNm_b->szChars());

  pwMatchedProc = pDicRoot->pFindWord(sProcNm_compound);

  if (pwMatchedProc) {

    if (pwMatchedProc->pCategory() != pProcCatg)
      return nullptr;

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // found two-part; correct the command token vector
    
    pTokenVec->vRemoveToken(1);
    
    pTkn0_CalledProcNm_a->vUpdateChars(sProcNm_compound,pTokenVec);
    
    if (iJotDebug())
      pTokenVec->vListTokens();

    // found procedure which matches token0_token1
    return pwMatchedProc;
  }
  
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // try three-part name
  
  nTokens = pTokenVec->nTokens();
  if (2 == nTokens)
    return nullptr;
  
  Token* pTkn2_CalledProcNm_b = pTokenVec->pGet(2);
  
  s256 sProcNm_compound_3 = sProcNm_compound;
  sProcNm_compound_3.vConcat("__"_Stz);
  sProcNm_compound_3.vConcat(pTkn2_CalledProcNm_b->szChars());
    
  pwMatchedProc = pDicRoot->pFindWord(sProcNm_compound_3);
  
  if (! pwMatchedProc)
    return nullptr;
  
  if (pwMatchedProc->pCategory() != pProcCatg)
    return nullptr;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // found two-part; correct the command token vector
  pTokenVec->vRemoveToken(1);
  pTokenVec->vRemoveToken(1);
  pTkn0_CalledProcNm_a->vUpdateChars(sProcNm_compound_3,pTokenVec);
  
  if (iJotDebug())
    pTokenVec->vListTokens();
  
  // found procedure which matches token0_token1
  return pwMatchedProc;
  
}



/*----------------------------------------------------------------------------*/
/* add word for procedure in root dictionary                                  */
/*----------------------------------------------------------------------------*/
void Procedure::vParseAndAdd(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PROCEDURE,_FOOTER_);

  
  s4K sProc;
  
  m_pTokenVec->vToTknString(sProc);
  if (AM::TRACE::bVerbose(C_VB_PROCEDURE))
    vTrace("%s\n"_Stz,sProc.azChar());

  m_pTokenVec->vIntegrityCheck();
  
  Token* pTkn1_ProcNm = m_pTokenVec->pGet(1);
  
  m_sProcNm = pTkn1_ProcNm->szChars();
    
  if (! ( m_pTokenVec->pGet(2)->bSame("tags"_Stz) ||
          m_pTokenVec->pGet(2)->bSame("parameters"_Stz) ||
          m_pTokenVec->pGet(2)->bSame("begin"_Stz) ) ) {
    
    m_sProcNm.vConcat("__"_Stz);
    m_sProcNm.vConcat(m_pTokenVec->pGet(2)->szChars());
    
    m_pTokenVec->vRemoveToken(2);
    m_pTokenVec->vIntegrityCheck();

    pTkn1_ProcNm->vUpdateChars(m_sProcNm,m_pTokenVec);
    m_pTokenVec->vIntegrityCheck();

    //m_pTokenVec->vDumpTokens();
  }

  if (! ( m_pTokenVec->pGet(2)->bSame("tags"_Stz) ||
          m_pTokenVec->pGet(2)->bSame("parameters"_Stz) ||
          m_pTokenVec->pGet(2)->bSame("begin"_Stz) ) ) {
    
    m_sProcNm.vConcat("__"_Stz);
    m_sProcNm.vConcat(m_pTokenVec->pGet(2)->szChars());
        
    m_pTokenVec->vRemoveToken(2);
    m_pTokenVec->vIntegrityCheck();
    
    pTkn1_ProcNm->vUpdateChars(m_sProcNm,m_pTokenVec);
    m_pTokenVec->vIntegrityCheck();
    
    //m_pTokenVec->vDumpTokens();
  }

  
  Dic* pDicRoot = AM::DIC::pRoot();

  Word* pProcCatg = pDicRoot->pFindWord(C_PROCEDURE_CATG);
  
  m_pProcWord = pDicRoot->pFindWord(m_sProcNm);
  if (m_pProcWord)
    return;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  
  m_pProcWord = new (pDicRoot) Word(m_sProcNm);
  
  m_pProcWord->vCategory(pProcCatg);
  
  if (! m_pProcWord->pWrdTagSet())
    m_pProcWord->vWrdTagSet(new TagSet);

  vParseTagSets(m_pProcWord);
  vParseParameters();


  m_pTokenVec->vVecToString(m_sProcText,0,false);
  
  if (AM::TRACE::bVerbose(C_VB_PROCEDURE))
    vTrace("%s\n"_Stz,m_sProcText.azChar());

  m_pProcWord->pWrdTagSet()->vInsert("procText"_Stz,m_sProcText);
}


/*----------------------------------------------------------------------------*/
/* parse parameters of procedure                                              */
/*----------------------------------------------------------------------------*/
void Procedure::vParseParameters(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PROCEDURE,_FOOTER_);

  i8 nTokens = m_pTokenVec->nTokens();

  Token* pTkn = nullptr;
  
  i8 i_Tkn = 0;
  for (; i_Tkn < nTokens; i_Tkn++) {
    pTkn = m_pTokenVec->pGet(i_Tkn);
    if (pTkn->bSame("parameters"_Stz))
      break;
  }
  
  if (! pTkn)
    return;
  
  TagSet* pTagSet = m_pProcWord->pWrdTagSet(true);
  
  for (i_Tkn++; i_Tkn < nTokens; i_Tkn++) {
    pTkn = m_pTokenVec->pGet(i_Tkn);
    
    if (pTkn->bSame("begin"_Stz))
      break;
        
    pTagSet->vInsert(pTkn->szChars());
  }

}

/*----------------------------------------------------------------------------*/
/* parse tagSets of procedure                                                 */
/*----------------------------------------------------------------------------*/
void Procedure::vParseTagSets(Word* pProcWord)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PROCEDURE,_FOOTER_);

  i8 nTokens = m_pTokenVec->nTokens();
  
  Token* pTkn = nullptr;
  
  i8 i_Tkn = 0;
  for (; i_Tkn < nTokens; i_Tkn++) {
    pTkn = m_pTokenVec->pGet(i_Tkn);
    if (pTkn->bSame("tags"_Stz))
      break;
  }
  
  i8 i_Cut = i_Tkn;
  
  if (i_Tkn == nTokens)
    return;
  
  TagSet* pTagSet = pProcWord->pWrdTagSet(true);
  
  for (i_Tkn++; i_Tkn < nTokens; i_Tkn++) {
    pTkn = m_pTokenVec->pGet(i_Tkn);
    
    if (pTkn->bSame("parameters"_Stz) ||
        pTkn->bSame("begin"_Stz))
      break;
    
    pTagSet->vInsert(pTkn->szChars());
  }
  
  // remove the tags clause from the token vector
  for (;;) {
    m_pTokenVec->vRemoveToken(i_Cut);
    Token* pTest = m_pTokenVec->pGet(i_Cut);
    if (pTest->bSame("parameters"_Stz) ||
        pTest->bSame("begin"_Stz))
      break;
  }
  
}


//******************************************************************************
//******************************************************************************
//******************************************************************************


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Procedure::bParseAndAdd_2(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PROCEDURE,_FOOTER_);
  
  s4K sProc;
  
  m_pTokenVec->vToTknString(sProc);
  if (AM::TRACE::bVerbose(C_VB_PROCEDURE))
    vTrace("%s\n"_Stz,sProc.azChar());
  
  if (! bParse_Procedure())  NEVER_REACH; //return false;
  if (! bParse_Name())       NEVER_REACH; //return false;

  vAddProcedureWord();
  
  if (! bParse_Tags())       NEVER_REACH; //return false;
  if (! bParse_TagList())    NEVER_REACH; //return false;
  if (! bParse_Parameters()) NEVER_REACH; //return false;
  if (! bParse_Begin())      NEVER_REACH; //return false;
  if (! bParse_SemiColon())  NEVER_REACH; //return false;
  
  vAddProcedureText();
  
  return true;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Procedure::bParse_Procedure(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PROCEDURE,_FOOTER_);

  if (! (m_pToken->bSame("procedure"_Stz) ||
         m_pToken->bSame("procedureLocal"_Stz)))
    return false;
  
  if (! bAdvanceToken())
    return false;

  return true;
}

/*----------------------------------------------------------------------------*/
/* parse name of procedure (maybe two or three parts)                         */
/*----------------------------------------------------------------------------*/
bool Procedure::bParse_Name(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PROCEDURE,_FOOTER_);

  m_pProcNmToken = m_pToken;
  m_sProcNm = m_pToken->szChars();
  
  if (m_sProcNm.bSame("std_start"_Stz))
    vTrace("%s\n"_Stz,m_sProcNm.szStz());

  if (! bAdvanceToken())
    return false;

  // two part name
  
  if (m_pToken->bSame("tags"_Stz)       ||
      m_pToken->bSame("tag"_Stz)        ||
      m_pToken->bSame("parameters"_Stz) ||
      m_pToken->bSame("begin"_Stz)        )
    return true;
  
  m_sProcNm.vConcat("__"_Stz);
  m_sProcNm.vConcat(m_pToken->szChars());
  
  if (! bRemoveToken())
    return false;
  vUpdateProcNmToken();
  
  // three part name

  if (m_pToken->bSame("tags"_Stz)       ||
      m_pToken->bSame("tag"_Stz)        ||
      m_pToken->bSame("parameters"_Stz) ||
      m_pToken->bSame("begin"_Stz)        )
    return true;

  m_sProcNm.vConcat("__"_Stz);
  m_sProcNm.vConcat(m_pToken->szChars());
  
  if (! bRemoveToken())
    return false;
  vUpdateProcNmToken();

  return true;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Procedure::bParse_Tags(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PROCEDURE,_FOOTER_);

  if (! m_pToken->bSame("tags"_Stz))
    return true;

  if (! bRemoveToken())
    return false;
  
  for (;;) {
    
    if (m_pToken->bSame("tag"_Stz)        ||
        m_pToken->bSame("parameters"_Stz) ||
        m_pToken->bSame("begin"_Stz)        )
      break;

    m_pTagSet->vInsert(m_pToken->szChars());
    
    if (! bRemoveToken())
      return false;
    
  }
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* parse optional parameters                                                  */
/*----------------------------------------------------------------------------*/
bool Procedure::bParse_Parameters(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PROCEDURE,_FOOTER_);
  
  if (! m_pToken->bSame("parameters"_Stz))
    return true;
  
  if (! bAdvanceToken())
    return false;

  for (;;) {
    
    if (m_pToken->bSame("begin"_Stz))
      break;
    
    m_pTagSet->vInsert(m_pToken->szChars());

    if (! bAdvanceToken())
      return false;

  }
  
  return true;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Procedure::bParse_Begin(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PROCEDURE,_FOOTER_);

  if (! m_pToken->bSame("begin"_Stz))
    return false;

  for (;;) {

    if (m_pToken->bSame("end"_Stz))
      break;
    
    if (! bAdvanceToken())
      return false;
  }

  IGNORE_RETURN(bAdvanceToken());

  return true;
}

/*----------------------------------------------------------------------------*/
/* parse 0 or more: tag <tagNm> = val endTag                                  */
/*----------------------------------------------------------------------------*/
bool Procedure::bParse_TagList(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PROCEDURE,_FOOTER_);

  if (! m_pToken->bSame("tag"_Stz))
    return true;
  if (! bRemoveToken())
    return false;

  for (;;) {

    // <tagNm>
    s32 sTagNm = m_pToken->szChars();
    if (! bRemoveToken())
      return false;

    // =
    if (! m_pToken->bSame("="_Stz))
      return false;
    if (! bRemoveToken())
      return false;

    // value
    sSTMT sTagValue = m_pToken->szChars();
    m_pTagSet->vInsert(sTagNm,sTagValue);
    if (! bRemoveToken())
      return false;

    if (! m_pToken->bSame("endtag"_Stz))
      return false;
    if (! bRemoveToken())
      return false;

    if (! m_pToken->bSame("tag"_Stz))
      break;
    if (! bRemoveToken())
      return false;
  }

  return true;
}

/*----------------------------------------------------------------------------*/
/* should see a semicolon                                                     */
/*----------------------------------------------------------------------------*/
bool Procedure::bParse_SemiColon(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PROCEDURE,_FOOTER_);
  
  //if (! m_pToken->bSame(";"_Stz))
  //  return false;
  
  // should be no more tokens
  //if (bAdvanceToken())
  //  return false;

  while (m_pToken)
    bAdvanceToken();

  return true;
}


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* add procedure text                                                         */
/*----------------------------------------------------------------------------*/
void Procedure::vAddProcedureText(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PROCEDURE,_FOOTER_);
  
  m_pTokenVec->vVecToString(m_sProcText,0,false);
  
  if (AM::TRACE::bVerbose(C_VB_PROCEDURE))
    vTrace("%s\n"_Stz,m_sProcText.azChar());

  m_pTagSet->vInsert("procText"_Stz,m_sProcText);
}

/*----------------------------------------------------------------------------*/
/* add procedure word                                                         */
/*----------------------------------------------------------------------------*/
void Procedure::vAddProcedureWord(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PROCEDURE,_FOOTER_);
  
  //vTrace("%s\n"_Stz,m_sProcNm.azChar());

  Dic* pDicRoot = AM::DIC::pRoot();

  Word* pProcCatg = pDicRoot->pFindWord(C_PROCEDURE_CATG);
  
  m_pProcWord = pDicRoot->pFindWord(m_sProcNm);
  
  if (m_pProcWord) {
    m_pTagSet = m_pProcWord->pWrdTagSet(true);
    return;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  m_pProcWord = new (pDicRoot) Word(m_sProcNm);
  
  m_pProcWord->vCategory(pProcCatg);
  
  m_pTagSet = m_pProcWord->pWrdTagSet(true);
}

/*----------------------------------------------------------------------------*/
/* advance to next token                                                      */
/*----------------------------------------------------------------------------*/
bool Procedure::bAdvanceToken(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PROCEDURE,_FOOTER_);

  m_i_Tkn++;
  
  if (m_i_Tkn >= m_nTokens) {
    m_pToken = nullptr;
    return false;
  }
  
  m_pToken = m_pTokenVec->pGet(m_i_Tkn);
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* remove current token from token vector; adjust cursor                      */
/*----------------------------------------------------------------------------*/
bool Procedure::bRemoveToken(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PROCEDURE,_FOOTER_);
  
  m_pTokenVec->vRemoveToken(m_i_Tkn);
  
  m_pTokenVec->vIntegrityCheck();
  
  m_nTokens = m_pTokenVec->nItems();
  
  if (m_i_Tkn >= m_nTokens)
    return false;

  m_pToken = m_pTokenVec->pGet(m_i_Tkn);
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* update token of multi-part name                                            */
/*----------------------------------------------------------------------------*/
void Procedure::vUpdateProcNmToken(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PROCEDURE,_FOOTER_);
  
  m_pProcNmToken->vUpdateChars(m_sProcNm,m_pTokenVec);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
