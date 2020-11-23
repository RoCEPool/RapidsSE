/*============================================================================*/
/* Steps_Forth.cpp - forth step group                                         */
/*============================================================================*/

#include "Steps.h"

extern Stz* gszaMungedText[];

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Forth(void)
{
  vAddStep(":"_Stz,                 &Step::vColon);
  vAddStep("("_Stz,                 &Step::vComment,          true );
  vAddStep(">createInRoot"_Stz,     &Step::vCreateFromInRoot);
  vAddStep("execute"_Stz,           &Step::vFlowExecute);
  vAddStep("exit_rt"_Stz,           &Step::vExit_RT);
  vAddStep("find:"_Stz,             &Step::vFind,             true);
  vAddStep("immediate"_Stz,         &Step::vImmediate,        true);
  vAddStep("mungedText"_Stz,        &Step::vMungedText);
  vAddStep("postpone"_Stz,          &Step::vPostpone,         true);
  vAddStep("see"_Stz,               &Step::vSee,              true);
  vAddStep(";"_Stz,                 &Step::vSemicolon,        true);
  vAddStep("series-name"_Stz,       &Step::vSeriesName);
}

/*----------------------------------------------------------------------------*/
/* : (aka colon) -- create word named by next token, enter compiling state    */
/*----------------------------------------------------------------------------*/
void Step::vColon(void)
{
  ENTRY();
  
  s256 sName;
  
  m_pCompiler->vMode(CM_COMPILING);
  
  m_pInput->vReadToken();
  sName = m_pInput->szChars();
  
  //vTrace("Colon: %s\n"_Stz,sName.szStz());
  
  Dic* pDic;
  if (m_oDicStack.iCount())
    pDic = m_oDicStack.pTop();
  else
    pDic = AM::pRootDic();
  
  Word* pNewWord = new(pDic) Word(sName);
  
  Word* pInclude = m_pInput->pUseInclude();
  pNewWord->vInclude(pInclude);
  
  m_pCompiler->vCurrentWord(pNewWord);
  
  m_pCompiler->vNestedWord(0,pNewWord);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* comment -- comment                                                         */
/*----------------------------------------------------------------------------*/
void Step::vComment(void)
{
  ENTRY();
  
  for (;;) {
    m_pInput->vReadToken();
    if (m_pInput->szChars()->bEqual(")"_Stz))
      break;
  }
  
  EXIT();
}
/*----------------------------------------------------------------------------*/
/* >createInRoot -- create word in root dictionary using string on stack      */
/*----------------------------------------------------------------------------*/
void Step::vCreateFromInRoot(void)
{
  ENTRY();
  
  s32 sName;
  m_pDataStack->vPopStr(sName,__func__);
  
  Dic* pDic = AM::pRootDic();
  
  //vTrace(": %s\n"_Stz,sName.azChar());
  Word* pNewWord = new(pDic) Word(sName);
  
  //m_pCompiler->vCurrentWord(pNewWord);
  
  m_pDataStack->vPushPtr(pNewWord);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* execute -- execute word on data stack                                      */
/*----------------------------------------------------------------------------*/
void Step::vFlowExecute(void)
{
  ENTRY();
  
  StepFp fpStep;
  Object* pObject;

  Word* pWord = _pWord(m_pDataStack->pPopPtr(__func__));
  
  eWK eWord = pWord->eWord();
  
  switch(eWord) {
    case WK_CPPOBJECT:
      m_pReturnStack->vPushFip(m_aInxPtr);
      m_aInxPtr = pWord->aEntryCell();
      break;
          
    case WK_INTRINSIC:
      fpStep = pWord->fpEntryStep();
      ((Step*)this->*fpStep)();
      break;
          
    case WK_LOCAL:
      NEVER_REACH;
      break;
          
    case WK_ROUTINE:
      m_pReturnStack->vPushFip(m_aInxPtr);
      m_aInxPtr = pWord->aEntryCell();
      break;
          
    default:
      NEVER_REACH;
  }
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* exit_rt -- runtime for exit from routine                                   */
/*----------------------------------------------------------------------------*/
void Step::vExit_RT(void)
{
  ENTRY();
  
  //vTrace("Exit: r eturn stack %lld\n"_Stz, m_pReturnStack->iValues());
  
  if ( (m_pTask->m_iTraceMask & C_TRACE_ROUTINES) )
    m_pTask->vTracePoint("<"_Stz);
  
  if (m_pLocalsFrame
      && m_pLocalsFrame->aDiscardPoint() == m_pReturnStack->aTopVal())
    m_pTask->vLocalsPop();
  
  // make sure we don't return to nowhere
  ASSERT(m_pReturnStack->nValues());
  
  Cell* aCell = m_pReturnStack->aPopFip(__func__);
  
  m_aInxPtr = aCell;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* find -- find and push word named by next token                             */
/*----------------------------------------------------------------------------*/
void Step::vFind(void)
{
  ENTRY();
  
  m_pInput->vReadToken();

  s32 sWordNm;
  sWordNm = m_pInput->szChars();
  
  Word* pWord = m_oDicStack.pFindWord(sWordNm);
  
  if (! pWord)
    vThrow(FK_FATAL,VL_FATAL,__func__,__FILE__,__LINE__,
           "No word: %s "_Stz,sWordNm.azChar());
  
  Datum xVar;
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

/*----------------------------------------------------------------------------*/
/* immediate -- make the word being compiled an immediate word                */
/*----------------------------------------------------------------------------*/
void Step::vImmediate(void)
{
  ENTRY();
  
  Word* pCurrentWord = m_pCompiler->pCurrentWord();
  
  //vTrace("Step::vImmediate: %s\n"_Stz,pCurrentWord->sName().azChar());
  
  pCurrentWord->vImmediate(true);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* mungedText -- push a particular munged text                                */
/*----------------------------------------------------------------------------*/
void Step::vMungedText(void)
{
  ENTRY();

  i8 i_Text = m_pDataStack->iPopInt(__func__);
  
  Stz* szText = gszaMungedText[i_Text];
  
  //vTrace("MungedText[%lld]:\n%s\n"_Stz,i_Text,szText);
  
  m_pDataStack->vPushStz(szText);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* postpone -- encode call to word named by next token into word being compiled  */
/*----------------------------------------------------------------------------*/
void Step::vPostpone(void)
{
  ENTRY();
  
  Word* pCurrentWord = m_pCompiler->pCurrentWord();
  
  // get name of word to be called
  m_pInput->vReadToken();

  // find the word in dic stack
  Word* pCalledWord = m_oDicStack.pFindWord(m_pInput->szChars());
  if ( ! pCalledWord )
    vThrow(FK_CHECK,VL_FATAL,__func__,__FILE__,__LINE__,
           "No word: %s "_Stz,
           m_pInput->szChars()->azChar());
  
  
  Datum xVar;
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
    pCurrentWord->vEncodeCall("encode::call"_Stz);
  }
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* see -- see inx contents of word                                            */
/*----------------------------------------------------------------------------*/
void Step::vSee(void)
{
  ENTRY();
  
  Word* pCurrentWord = m_pCompiler->pCurrentWord();

  AM::vWordSee(pCurrentWord,&m_oDicStack);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* ; -- end compilation of current word, enter interpreter state              */
/*----------------------------------------------------------------------------*/
void Step::vSemicolon(void)
{
  ENTRY();
  
  static i8 iMaxUsed = 0;
  
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
  
  i8 iUsed = pCurrentWord->nRoutineUsed();
  if (iUsed > iMaxUsed)
    iMaxUsed = iUsed;
//  vTrace(" Routine %lldB (%lld): %s\n"_Stz,
//         pCurrentWord->nRoutineUsed(),
//         iMaxUsed,
//         pCurrentWord->szWordNm());
  
  m_pCompiler->vMode(CM_INTERPRETING);
    
  //m_pCompiler->vCurrentWord(nullptr);
    
//  if (pCurrentWord->szWordNm()->bSame("tpl_liteOp_PulseMarker"_Stz))
//    AM::vWordSee(pCurrentWord,&m_oDicStack);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* series-name -- generate next name within series for word named on stack    */
/*----------------------------------------------------------------------------*/
void Step::vSeriesName(void)
{
  ENTRY();
  
  s32 sName;
  m_pDataStack->vPopStr(sName,__func__);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  i8 iNext = 0;
  
  Dic* pRootDic = AM::pRootDic();
  
  Word* pWord = nullptr;
  while ( (pWord = pRootDic->pEachWord(pWord)) ) {

    s32 sStr;
    s32 sInt;
    
    StrLib::vSplitSeries(sStr.azChar(),sInt.azChar(),pWord->szWordNm()->azChar());
    
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


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
