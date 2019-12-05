/*============================================================================*/
/* Steps_Forth.cpp - forth step group                                         */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Forth(void)
{
  vAddStep(":"_Stz,               &Step::vColon);
  vAddStep(","_Stz,               &Step::vComma);
  vAddStep("compile,"_Stz,        &Step::vCompileComma);
  vAddStep("createTenants"_Stz,   &Step::vCreateTenants);
  vAddStep(">createInRoot"_Stz,   &Step::vCreateFromInRoot);
  vAddStep(">createPersistent"_Stz, &Step::vCreateFromPersistent);
  vAddStep("createPersistent"_Stz,&Step::vCreatePersistent);
  vAddStep("does>"_Stz,           &Step::vDoes,             true );
  vAddStep("does_rt"_Stz,         &Step::vDoes_RT);
  vAddStep("exit_rt"_Stz,         &Step::vExit_RT);
  vAddStep("findInCurrent"_Stz,   &Step::vFindInCurrent);
  vAddStep("immediate"_Stz,       &Step::vImmediate,        true );
  vAddStep("postpone"_Stz,        &Step::vPostpone,         true );
  vAddStep("prompt"_Stz,          &Step::vPrompt,           true );
  vAddStep(";"_Stz,               &Step::vSemicolon,        true );
  vAddStep("series-name"_Stz,     &Step::vSeriesName);
  vAddStep("tick"_Stz,            &Step::vTick,             true );
}

/*----------------------------------------------------------------------------*/
/* : (aka colon) -- create word named by next token, enter compiling state    */
/*----------------------------------------------------------------------------*/
void Step::vColon(void)
{
  ENTRY(); // ENTRY("vColon"_Stz);
  
  s32 sName;
  
  m_pCompiler->vMode(CM_COMPILING);
  
  m_pCompiler->vNextToken();
  sName = m_pCompiler->szToken();
  
  Dic* pDic;
  if (m_oDicStack.iCount())
    pDic = m_oDicStack.pTop();
  else
    pDic = AM::pRootDic();
  
  
  //vTrace(": %s\n"_Stz,sName.azChar());
  
  Word* pNewWord = new(pDic) Word(sName);
  
  m_pCompiler->vCurrentWord(pNewWord);
  
  m_pCompiler->vNestedWord(0,pNewWord);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* comma -- encode push of value on stack into word being compiled            */
/*----------------------------------------------------------------------------*/
void Step::vComma(void)
{
  ENTRY(); // ENTRY("vComma"_Stz);
  
  Word* pCurrentWord = m_pCompiler->pCurrentWord();
  
  GenVar xValue;
  m_pDataStack->vPopVal(xValue,__func__);
  
  pCurrentWord->vEncodePush(xValue);
    
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* compile, -- encode call to word on stack into word being compiled          */
/*----------------------------------------------------------------------------*/
void Step::vCompileComma(void)
{
  ENTRY(); // ENTRY("vCompileComma"_Stz);
  
  Word* pCalledWord = _pWord(m_pDataStack->pPopPtr(__func__));
  
  Word* pCurrentWord = m_pCompiler->pCurrentWord();
  
  StepFp* afpCalledEntryStep = pCalledWord->afpEntryStep();
  
  pCurrentWord->vEncodeCall(afpCalledEntryStep);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* createPersistent -- create word in dictionary (persistent heap)            */
/*----------------------------------------------------------------------------*/
void Step::vCreatePersistent(void)
{
  ENTRY(); // ENTRY("vCreate"_Stz);
  
  m_pCompiler->vNextToken();
  
  Dic* pDic;
  if (m_oDicStack.iCount())
    pDic = m_oDicStack.pTop();
  else
    pDic = AM::pRootDic();
  
  //vTrace(": %s\n"_Stz,m_pCompiler->szToken()->azChar());
  
  Word* pNewWord = new(pDic,true) Word(m_pCompiler->szToken());
  
  m_pCompiler->vCurrentWord(pNewWord);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* >createPersistent -- create word in dic using string on stack (persistent) */
/*----------------------------------------------------------------------------*/
void Step::vCreateFromPersistent(void)
{
  ENTRY(); // ENTRY("vCreateFrom"_Stz);
  
  s32 sName;
  m_pDataStack->vPopStr(sName,__func__);
  
  Dic* pDic;
  if (m_oDicStack.iCount())
    pDic = m_oDicStack.pTop();
  else
    pDic = AM::pRootDic();
  
  //vTrace(": %s\n"_Stz,sName.azChar());
  Word* pNewWord = new(pDic,true) Word(sName);
  
  m_pCompiler->vCurrentWord(pNewWord);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* >createInRoot -- create word in root dictionary using string on stack      */
/*----------------------------------------------------------------------------*/
void Step::vCreateFromInRoot(void)
{
  ENTRY(); // ENTRY("vCreateFromInRoot"_Stz);
  
  s32 sName;
  m_pDataStack->vPopStr(sName,__func__);
  
  Dic* pDic = AM::pRootDic();
  
  //vTrace(": %s\n"_Stz,sName.azChar());
  Word* pNewWord = new(pDic) Word(sName);
  
  m_pCompiler->vCurrentWord(pNewWord);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* createTenants -- create tenant word in dictionary                          */
/*----------------------------------------------------------------------------*/
void Step::vCreateTenants(void)
{
  ENTRY(); // ENTRY("vCreateTenants"_Stz);
  
  m_pCompiler->vNextToken();
    
  Dic* pDic = AM::pRootDic();

  Word* pNewWord = new(pDic) Word(m_pCompiler->szToken());
  
  m_pCompiler->vCurrentWord(pNewWord);
  
  AM::vTenants(pNewWord);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* does> -- routine does                                                      */
/*----------------------------------------------------------------------------*/
void Step::vDoes(void)
{
  ENTRY(); // ENTRY("vDoes"_Stz);
  
  Word* pCurrentWord = m_pCompiler->pCurrentWord();
  
  pCurrentWord->vEncodeCall("does_rt"_Stz);
  pCurrentWord->vEncodeCall("exit_rt"_Stz);
  
  //pCurrentWord->vEncode("pushBody");
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* does_rt -- runtime for does>                                               */
/*----------------------------------------------------------------------------*/
void Step::vDoes_RT(void)
{
  ENTRY(); // ENTRY("vDoes_RT"_Stz);
  
  Word* pCurrentWord = m_pCompiler->pCurrentWord();
  
  pCurrentWord->vEntryStep(&Step::vEnterObject);
  pCurrentWord->vEntryCell(m_aInxPtr + 1);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* exit_rt -- runtime for exit from routine                                   */
/*----------------------------------------------------------------------------*/
void Step::vExit_RT(void)
{
  ENTRY(); // ENTRY("vExit_RT"_Stz);
  
  //vTrace("Exit: r eturn stack %lld\n"_Stz, m_pReturnStack->iValues());
  
  if ( (m_pTask->m_iTraceMask & C_TRACE_ROUTINES) )
    m_pTask->vTracePoint("<"_Stz);
  
  if (m_pLocalsFrame
      && m_pLocalsFrame->aDiscardPoint() == m_pReturnStack->aTopVal())
    m_pTask->vLocalsPop();
  
  Cell* aCell = m_pReturnStack->aPopFip(__func__);
  
  m_aInxPtr = aCell;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* findInCurrent -- find and push word named on stack withinin curr dic       */
/*----------------------------------------------------------------------------*/
void Step::vFindInCurrent(void)
{
  ENTRY(); // ENTRY("vFindInCurrent"_Stz);
  
  s256 sName;
  m_pDataStack->vPopStr(sName,__func__);
  
  if (m_pTask->m_iTraceMask) {
    i8 i = 0; i++;
  }
  
  Word* pWord = m_oDicStack.pFindInCurrent(sName);
  
  m_pDataStack->vPushPtr(pWord);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* immediate -- make the word being compiled an immediate word                */
/*----------------------------------------------------------------------------*/
void Step::vImmediate(void)
{
  ENTRY(); // ENTRY("vImmediate"_Stz);
  
  Word* pCurrentWord = m_pCompiler->pCurrentWord();
  
  //vTrace("Step::vImmediate: %s\n"_Stz,pCurrentWord->sName().azChar());
  
  pCurrentWord->vImmediate(true);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* postpone -- encode call to word named by next token into word being compiled  */
/*----------------------------------------------------------------------------*/
void Step::vPostpone(void)
{
  ENTRY(); // ENTRY("vPostpone"_Stz);
  
  Word* pCurrentWord = m_pCompiler->pCurrentWord();
  
  // get name of word to be called
  m_pCompiler->vNextToken();
  
  // find the word in dic stack
  Word* pCalledWord = m_oDicStack.pFindWord(m_pCompiler->szToken());
  if ( ! pCalledWord )
    vThrow(FK_CHECK,VL_FATAL,__func__,__FILE__,__LINE__,
           "No word: %s "_Stz,
           m_pCompiler->szToken()->azChar());
  
  
  GenVar xVar;
  xVar = pCalledWord;

  if (pCalledWord->bImmediate()) {
    
    // put call to immediate word into this current definition
    
    // get indirect pointer to entry step of called word
    StepFp* afpCalledEntryStep = pCalledWord->afpEntryStep();
    
    // copy indirect pointer into the current word
    pCurrentWord->vEncodeCall(afpCalledEntryStep);
  }
  else {
    
    // put call to normal word into some future definition
    
    // copy indirect pointer into the current word
    pCurrentWord->vEncodePush(xVar);
    pCurrentWord->vEncodeCall("compile,"_Stz);
  }
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* prompt -- make the word being compiled directly runnable by prompt         */
/*----------------------------------------------------------------------------*/
void Step::vPrompt(void)
{
  ENTRY(); // ENTRY("vImmediate"_Stz);
  
  Word* pCurrentWord = m_pCompiler->pCurrentWord();
  
  pCurrentWord->vPrompt(true);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* ; -- end compilation of current word, enter interpreter state              */
/*----------------------------------------------------------------------------*/
void Step::vSemicolon(void)
{
  ENTRY(); // ENTRY("vSemicolon"_Stz);
  
  Word* pCurrentWord = m_pCompiler->pCurrentWord();
  
  if (m_pCompiler->nLocals()) {
    
    // reset the number of local variables
    m_pCompiler->vLocals(0);
    
    // pop top dictionary
    m_oDicStack.vDiscard();
    
    // enocde locals frame pop
    pCurrentWord->vEncodeCall("locals.pop"_Stz);
  }
  
  if (! pCurrentWord->szWordNm()->bSame("exit"_Stz) &&
      m_oDicStack.pFindWord("exit"_Stz)) {
    
    if (m_oDicStack.pFindWord(pCurrentWord->szExitHook())) {
      pCurrentWord->vEncodeCall(pCurrentWord->szExitHook());
      pCurrentWord->vEncodeCall("exit_rt"_Stz);
    }
    else
      pCurrentWord->vEncodeCall("exit_RT"_Stz);
  }
  else
    pCurrentWord->vEncodeCall("exit_rt"_Stz);
  
  m_pCompiler->vMode(CM_INTERPRETING);
    
  m_pCompiler->vCurrentWord(nullptr);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* series-name -- generate next name within series for word named on stack    */
/*----------------------------------------------------------------------------*/
void Step::vSeriesName(void)
{
  ENTRY(); // ENTRY("vSeriesName"_Stz);
  
  s32 sName;
  m_pDataStack->vPopStr(sName,__func__);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  i8 iNext = 0;
  
  Dic* pRootDic = AM::pRootDic();
  
  Word* pCategory = pRootDic->pFindWord("jobSyntax:"_Stz);

  Word* pWord = nullptr;
  while ( (pWord = pRootDic->pEachWord(pCategory,pWord)) ) {

    s32 sStr;
    s32 sInt;
    
    StrLib::vSplit_si(sStr.azChar(),sInt.azChar(),pWord->szWordNm()->azChar());
    
    // vTrace("  %s - %lld\n"_Stz,sStr.azChar(),sInt.iToI8());
    
    if (sStr.bSame(sName)) {
      iNext = sInt.iToI8() + 1;
      break;
    }
  }

  s32 sNextName(sName);
  sNextName.vCatSprint("%02lld"_Stz,iNext);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  m_pDataStack->vPushStr(sNextName);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* tick -- find and push word named by next token                             */
/*----------------------------------------------------------------------------*/
void Step::vTick(void)
{
  ENTRY(); // ENTRY("vTick"_Stz);
  
  m_pCompiler->vNextToken();
    
  s32 sWordNm;
  sWordNm = m_pCompiler->szToken();
  
  Word* pWord = m_oDicStack.pFindWord(sWordNm);
  
  if (! pWord)
    vThrow(FK_FATAL,VL_FATAL,__func__,__FILE__,__LINE__,
           "No word: %s "_Stz,sWordNm.azChar());
  
  GenVar xVar;
  xVar = pWord;

  if (m_pCompiler->bCompiling()) {
    Word* pCurrentWord = m_pCompiler->pCurrentWord();
    pCurrentWord->vEncodePush(xVar);
  }
  else {
    m_pDataStack->vPushPtr(pWord);
  }
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
