/*============================================================================*/
/* Steps_Sen.cpp - sen step group                                             */
/*============================================================================*/

#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "StepTiming.hpp"


extern Stz* gszaMungedText[];

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Sen(void)
{
  vAddStep(":"_Stz,              &Step::vSenColon);
  vAddStep("("_Stz,              &Step::vSenComment,          true );
  vAddStep(">createInRoot"_Stz,  &Step::vSenCreateFromInRoot);
  vAddStep("doColon"_Stz,        &Step::vSenDoColon);
  vAddStep("execute"_Stz,        &Step::vSenExecute);
  vAddStep("exit_rt"_Stz,        &Step::vSenExit_RT);
  vAddStep("find:"_Stz,          &Step::vSenFind,             true);
  vAddStep("hazard"_Stz,         &Step::vSenHazard);
  vAddStep("immediate"_Stz,      &Step::vSenImmediate,        true);
  vAddStep("jump"_Stz,           &Step::vSenJump);
  vAddStep("jumpFalse"_Stz,      &Step::vSenJumpFalse);
  vAddStep("jumpTrue"_Stz,       &Step::vSenJumpTrue);
  vAddStep("mungedText"_Stz,     &Step::vSenMungedText);
  vAddStep("nop"_Stz,            &Step::vSenNop);
  vAddStep("postpone"_Stz,       &Step::vSenPostpone,         true);
  vAddStep("reenter"_Stz,        &Step::vSenReenter);
  vAddStep("see"_Stz,            &Step::vSenSee,              true);
  vAddStep(";"_Stz,              &Step::vSenSemicolon,        true);
  vAddStep("series-name"_Stz,    &Step::vSenSeriesName);
  vAddStep("test"_Stz,           &Step::vSenTest);
  vAddStep("throw"_Stz,          &Step::vSenThrow);
}

/*----------------------------------------------------------------------------*/
/* : (aka colon) -- create word named by next token, enter compiling state    */
/*----------------------------------------------------------------------------*/
void Step::vSenColon(void)
{
  ENTRY();
  
  s256 sWordNm;
  
  m_pSenCompiler->vMode(CM_COMPILING);
  
  m_pInput->vReadToken();
  sWordNm = m_pInput->szChars();
  
  //vTrace("Colon: %s\n"_Stz,sWordNm.szStz());
  
  Dic* pDic;
  if (m_oDicStack.iCount())
    pDic = m_oDicStack.pTop();
  else
    pDic = AM::pRootDic();
  
  Word* pFoundWord = pDic->pFindWord(sWordNm);
  if (pFoundWord) {
    vTrace("\nvColon : word already defined [++] : %s\n\n"_Stz,sWordNm.szStz());
  }
//  else {
//    vTrace("vColon : new word [--] : %s\n"_Stz,sWordNm.szStz());
//  }
  
  Word* pNewWord = new(pDic) Word(sWordNm);
  
  Word* pInclude = m_pInput->pUseInclude();
  pNewWord->vInclude(pInclude);
  
  m_pSenCompiler->vCurrentWord(pNewWord);
  
  m_pSenCompiler->vNestedWord(0,pNewWord);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* comment -- comment                                                         */
/*----------------------------------------------------------------------------*/
void Step::vSenComment(void)
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
void Step::vSenCreateFromInRoot(void)
{
  ENTRY();
  
  s32 sName;
  m_pDataStack->vPopStr(sName,__func__);
  
  Dic* pDic = AM::pRootDic();
  
  //vTrace(": %s\n"_Stz,sName.azChar());
  Word* pNewWord = new(pDic) Word(sName);
  
  //m_pSenCompiler->vCurrentWord(pNewWord);
  
  m_pDataStack->vPushPtr(pNewWord);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* DoColon -- push inxPtr and set inxPtr to execute a different codeblock */
/*----------------------------------------------------------------------------*/
void Step::vSenDoColon(void)
{
  ENTRY();
  
  CodeCell* aPrevCellOfCurrentCodeBlock = m_aInxPtr - 1;
  

  fpSTEPFN* aAddressOf__m_fpIntrinsic =
    (fpSTEPFN*) aPrevCellOfCurrentCodeBlock->m_afpStepFn;
                 

  CodeCell** aAddressOf__m_aInxPtr =
    (CodeCell**) (aAddressOf__m_fpIntrinsic + 1);
  
  
  CodeCell* aStartCellOfCalledCodeBlock = * aAddressOf__m_aInxPtr;
  
  
  m_pReturnStack->vPushFip(m_aInxPtr);
  
  
  m_aInxPtr = aStartCellOfCalledCodeBlock;
  

   if ( (m_iTraceMask & C_TRACE_ROUTINES) )
    Task::vTracePoint(">"_Stz);
  
  EXIT();
}
/*----------------------------------------------------------------------------*/
//void Step::vSenDoColon(void)
//{
//  ENTRY();
//
//  fpSTEPFN* afpEntryCell =
//    (fpSTEPFN*) ((m_aInxPtr - 1)->m_afpStepFn + 1);
//
//  m_pReturnStack->vPushFip(m_aInxPtr);
//  m_aInxPtr = *(CodeCell**)afpEntryCell;
//
//   if ( (m_iTraceMask & C_TRACE_ROUTINES) )
//    Task::vTracePoint(">"_Stz);
//
//  EXIT();
//}


/*----------------------------------------------------------------------------*/
/* execute -- execute word on data stack                                      */
/*----------------------------------------------------------------------------*/
void Step::vSenExecute(void)
{
  ENTRY();
  
  fpSTEPFN fpStepFn;
  Object* pObject;

  Word* pWord = _pWord(m_pDataStack->pPopPtr(__func__));
  
  eWK eWord = pWord->eWord();
  
  switch(eWord) {
    case WK_CPPOBJECT:
      m_pReturnStack->vPushFip(m_aInxPtr);
      m_aInxPtr = pWord->aEntryCell();
      break;
          
    case WK_INTRINSIC:
      fpStepFn = pWord->fpEntryStep();
      ((Step*)this->*fpStepFn)();
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
void Step::vSenExit_RT(void)
{
  ENTRY();
  
  //vTrace("Exit: r eturn stack %lld\n"_Stz, m_pReturnStack->iValues());
  
  if ( (m_iTraceMask & C_TRACE_ROUTINES) )
    Task::vTracePoint("<"_Stz);
  
  if (m_pLocalsFrame
      && m_pLocalsFrame->aDiscardPoint() == (TypedCell*)m_pReturnStack->aTopVal())
    vLocalsPop();
  
  // make sure we don't return to nowhere
  ASSERT(m_pReturnStack->nValues());
  
  CodeCell* aCell = m_pReturnStack->aPopFip(__func__);
  
  m_aInxPtr = aCell;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* find -- find and push word named by next token                             */
/*----------------------------------------------------------------------------*/
void Step::vSenFind(void)
{
  ENTRY();
  
  m_pInput->vReadToken();

  s32 sWordNm;
  sWordNm = m_pInput->szChars();
  
  Word* pWord = m_oDicStack.pFindWord(sWordNm);
  
  if (! pWord)
    vThrow(FK_FATAL,VL_FATAL,__func__,__FILE__,__LINE__,
           "No word: %s "_Stz,sWordNm.azChar());
  
  CellSingle xVar;
  xVar = pWord;

  if (m_pSenCompiler->bCompiling()) {
    Word* pCurrentWord = m_pSenCompiler->pCurrentWord();
    pCurrentWord->vEncodePush(xVar);
  }
  else {
    m_pDataStack->vPushPtr(pWord);
  }
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* hazard -- should never be called                                           */
/*----------------------------------------------------------------------------*/
void Step::vSenHazard(void)
{
  ENTRY();
  
  vThrow(FK_CHECK,VL_FATAL,__func__,__FILE__,__LINE__,"Entered hazard"_Stz);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* immediate -- make the word being compiled an immediate word                */
/*----------------------------------------------------------------------------*/
void Step::vSenImmediate(void)
{
  ENTRY();
  
  Word* pCurrentWord = m_pSenCompiler->pCurrentWord();
  
  //vTrace("Step::vImmediate: %s\n"_Stz,pCurrentWord->sName().azChar());
  
  pCurrentWord->vImmediate(true);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* jump -- jump to location                                                   */
/*----------------------------------------------------------------------------*/
void Step::vSenJump(void)
{
  ENTRY();
  
  m_aInxPtr = m_aInxPtr->m_aJumpTo;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* jumpFalse -- jump on false to location                                     */
/*----------------------------------------------------------------------------*/
void Step::vSenJumpFalse(void)
{
  ENTRY();
  
  i8 iVal = m_pDataStack->iPopAsInt(__func__);
  
  // n.b. the IP points to the jump arg
  
  if (! iVal)
    // jump to location
    m_aInxPtr = m_aInxPtr->m_aJumpTo;
  else
    // do not jump, skip over argument
    m_aInxPtr++;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* jumpTrue -- jump on true to location                                       */
/*----------------------------------------------------------------------------*/
void Step::vSenJumpTrue(void)
{
  ENTRY();
  
  i8 iVal = m_pDataStack->iPopAsInt(__func__);
  
  // n.b. the IP points to the jump arg
  
  if (iVal)
    // jump to location
    m_aInxPtr = m_aInxPtr->m_aJumpTo;
  else
    // do not jump, skip over argument
    m_aInxPtr++;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* mungedText -- push a particular munged text                                */
/*----------------------------------------------------------------------------*/
void Step::vSenMungedText(void)
{
  ENTRY();

  i8 i_Text = m_pDataStack->iPopInt(__func__);
  
  Stz* szText = gszaMungedText[i_Text];
  
  //vTrace("MungedText[%lld]:\n%s\n"_Stz,i_Text,szText);
  
  m_pDataStack->vPushStz(szText);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* nop -- no op                                                               */
/*----------------------------------------------------------------------------*/
void Step::vSenNop(void)
{
  ENTRY();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* postpone -- encode call to word named by next token into word being compiled  */
/*----------------------------------------------------------------------------*/
void Step::vSenPostpone(void)
{
  ENTRY();
  
  Word* pCurrentWord = m_pSenCompiler->pCurrentWord();
  
  // get name of word to be called
  m_pInput->vReadToken();

  // find the word in dic stack
  Word* pCalledWord = m_oDicStack.pFindWord(m_pInput->szChars());
  if ( ! pCalledWord )
    vThrow(FK_CHECK,VL_FATAL,__func__,__FILE__,__LINE__,
           "No word: %s "_Stz,
           m_pInput->szChars()->azChar());
  
  
  CellSingle xVar;
  xVar = pCalledWord;

  if (pCalledWord->bImmediate()) {
    
    // put call to immediate word into this current definition
    
    // get indirect pointer to entry step of called word
    fpSTEPFN* afpCalledEntryStep = pCalledWord->afpEntryStep();
    
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
/* reenter -- re-enter the word currenty being executed (i.e. start over)     */
/*----------------------------------------------------------------------------*/
void Step::vSenReenter(void)
{
  ENTRY();
  
  Word* pExecuting = CodeBlock::pToWord(m_aInxPtr);
  
  m_aInxPtr = pExecuting->aEntryCell();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* see -- see inx contents of word                                            */
/*----------------------------------------------------------------------------*/
void Step::vSenSee(void)
{
  ENTRY();
  
  Word* pCurrentWord = m_pSenCompiler->pCurrentWord();

  AM::vWordSee(pCurrentWord,&m_oDicStack);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* ; -- end compilation of current word, enter interpreter state              */
/*----------------------------------------------------------------------------*/
void Step::vSenSemicolon(void)
{
  ENTRY();
  
  static i8 iMaxUsed = 0;
  
  Word* pCurrentWord = m_pSenCompiler->pCurrentWord();
  
  if (m_pSenCompiler->nLocals()) {
    
    // reset the number of local variables
    m_pSenCompiler->vLocals(0);
    
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
  
  i8 iUsed = pCurrentWord->nCodeBlockUsed();
  if (iUsed > iMaxUsed)
    iMaxUsed = iUsed;
//  vTrace(" CodeBlock %lldB (%lld): %s\n"_Stz,
//         pCurrentWord->nCodeBlockUsed(),
//         iMaxUsed,
//         pCurrentWord->szWordNm());
  
  m_pSenCompiler->vMode(CM_INTERPRETING);
    
  //m_pSenCompiler->vCurrentWord(nullptr);
    
//  if (pCurrentWord->szWordNm()->bSame("tpl_liteOp_PulseMarker"_Stz))
//    AM::vWordSee(pCurrentWord,&m_oDicStack);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* series-name -- generate next name within series for word named on stack    */
/*----------------------------------------------------------------------------*/
void Step::vSenSeriesName(void)
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
      //break; // comment this out
    }
  }

  s32 sNextName(sName);
  sNextName.vCatSprint("%02lld"_Stz,iNext);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  m_pDataStack->vPushStr(sNextName);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* test -- print test                                                         */
/*----------------------------------------------------------------------------*/
void Step::vSenTest(void)
{
  ENTRY();
  
  vTrace("[%s] Test\n"_Stz,szJobName());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* throw -- throw c++ exception (for testing purposes)                        */
/*----------------------------------------------------------------------------*/
void Step::vSenThrow(void)
{
  ENTRY();
  
  s256 sException;
  
  sException.vSprint("THROW TEST"_Stz);
  
  vThrow(FK_CHECK,VL_FATAL,__func__,__FILE__,__LINE__,sException);
  
  EXIT();
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
