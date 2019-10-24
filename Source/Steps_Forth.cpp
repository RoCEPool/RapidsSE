/*============================================================================*/
/* Steps_Forth.cpp - forth step group                                         */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Forth(void)
{
  vAddStep("allot"_Stz,           &Step::vAllot);
  vAddStep("cell"_Stz,            &Step::vCell);
  vAddStep(":"_Stz,               &Step::vColon);
  vAddStep(","_Stz,               &Step::vComma);
  vAddStep("compile,"_Stz,        &Step::vCompileComma);
  vAddStep("create"_Stz,          &Step::vCreate);
  vAddStep("createTenants"_Stz,   &Step::vCreateTenants);
  vAddStep(">create"_Stz,         &Step::vCreateFrom);
  vAddStep(">createInRoot"_Stz,   &Step::vCreateFromInRoot);
  vAddStep(">createPersistent"_Stz, &Step::vCreateFromPersistent);
  vAddStep("createPersistent"_Stz,&Step::vCreatePersistent);
  vAddStep("does>"_Stz,           &Step::vDoes,             true );
  vAddStep("does_rt"_Stz,         &Step::vDoes_RT);
  vAddStep("exit_rt"_Stz,         &Step::vExit_RT);
  vAddStep("find"_Stz,            &Step::vFind);
  vAddStep("findInCurrent"_Stz,   &Step::vFindInCurrent);
  vAddStep("forget"_Stz,          &Step::vForget);
  vAddStep("immediate"_Stz,       &Step::vImmediate,        true );
  vAddStep("locate"_Stz,          &Step::vLocate);
  vAddStep("postpone"_Stz,        &Step::vPostpone,         true );
  vAddStep("prompt"_Stz,          &Step::vPrompt,           true );
  vAddStep("see"_Stz,             &Step::vSee);
  vAddStep(";"_Stz,               &Step::vSemicolon,        true );
  vAddStep("series-name"_Stz,     &Step::vSeriesName);
  vAddStep("subDic"_Stz,          &Step::vSubDic,           true );
  vAddStep("tick"_Stz,            &Step::vTick,             true );
}

/*----------------------------------------------------------------------------*/
/* allot -- reserve n bytes in word being compiled                            */
/*----------------------------------------------------------------------------*/
void Step::vAllot(void)
{
  ENTRY("vAllot"_Stz);
  
  i8 iBytes;
  
  if ( !  m_pDataStack->bPopI8(iBytes) )
    THROW_DSTACK();
  
  RASSERT(iBytes);
  
  Word* pCurrentWord = m_pTask->m_pForth->m_pCompiler->m_pCurrent;
  
  byte* aAddress = pCurrentWord->ayAllot(iBytes);
  ASSERT(aAddress);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* cell -- push size of a cell                                                */
/*----------------------------------------------------------------------------*/
void Step::vCell(void)
{
  ENTRY("vCell"_Stz);
  
  i8 nBytes = sizeof(void*);
  
  m_pDataStack->vPushI8(nBytes);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* : (aka colon) -- create word named by next token, enter compiling state    */
/*----------------------------------------------------------------------------*/
void Step::vColon(void)
{
  ENTRY("vColon"_Stz);
  
  s32 sName;
  
  m_pTask->m_pForth->m_pCompiler->m_bCompiling = true;
  
  m_pTask->m_pForth->m_pCompiler->m_pTokeniser->vNextToken();
  sName = m_pTask->m_pForth->m_pCompiler->m_pTokeniser->sToken();
  
  Dic* pDic;
  if (m_pTask->m_pForth->m_oDicStack.iCount())
    pDic = m_pTask->m_pForth->m_oDicStack.pTop();
  else
    pDic = AM::pRootDic();
  
  
  //vTrace(": %s\n"_Stz,sName.azChar());
  
  Word* pNewWord = new(pDic) Word(sName);
  
  m_pTask->m_pForth->m_pCompiler->m_pCurrent = pNewWord;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* comma -- encode push of value on stack into word being compiled            */
/*----------------------------------------------------------------------------*/
void Step::vComma(void)
{
  ENTRY("vComma"_Stz);
  
  Word* pCurrentWord = m_pTask->m_pForth->m_pCompiler->m_pCurrent;
  
  GenVar xValue;
  if ( ! m_pDataStack->bPopGenVar(xValue) )
    THROW_DSTACK();
  
  pCurrentWord->vEncode("pushValue"_Stz,&xValue);
    
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* compile, -- encode call to word on stack into word being compiled          */
/*----------------------------------------------------------------------------*/
void Step::vCompileComma(void)
{
  ENTRY("vCompileComma"_Stz);
  
  Object* pWordObject;
  if ( ! m_pDataStack->bPopObject(pWordObject) )
    THROW_DSTACK();
  Word* pCalledWord = _pWord(pWordObject);
  
  Word* pCurrentWord = m_pTask->m_pForth->m_pCompiler->m_pCurrent;
  
  StepFp* afpCalledEntryStep = pCalledWord->afpEntryStep();
  
  pCurrentWord->vEncode(afpCalledEntryStep);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* create -- create word named by next token, becomes word being compiled     */
/*----------------------------------------------------------------------------*/
void Step::vCreate(void)
{
  ENTRY("vCreate"_Stz);
  
  m_pTask->m_pForth->m_pCompiler->m_pTokeniser->vNextToken();
  
  Dic* pDic;
  if (m_pTask->m_pForth->m_oDicStack.iCount())
    pDic = m_pTask->m_pForth->m_oDicStack.pTop();
  else
    pDic = AM::pRootDic();
  
  
  //vTrace(": %s\n"_Stz,m_pTask->m_pForth->m_pCompiler->m_pTokeniser->sToken().azChar());
  
  Word* pNewWord = new(pDic) Word(m_pTask->m_pForth->m_pCompiler->m_pTokeniser->sToken());
  
  m_pTask->m_pForth->m_pCompiler->m_pCurrent = pNewWord;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* createPersistent -- create word in dictionary (persistent heap)            */
/*----------------------------------------------------------------------------*/
void Step::vCreatePersistent(void)
{
  ENTRY("vCreate"_Stz);
  
  m_pTask->m_pForth->m_pCompiler->m_pTokeniser->vNextToken();
  
  Dic* pDic;
  if (m_pTask->m_pForth->m_oDicStack.iCount())
    pDic = m_pTask->m_pForth->m_oDicStack.pTop();
  else
    pDic = AM::pRootDic();
  
  //vTrace(": %s\n"_Stz,m_pTask->m_pForth->m_pCompiler->m_pTokeniser->sToken().azChar());
  
  Word* pNewWord = new(pDic,true) Word(m_pTask->m_pForth->m_pCompiler->m_pTokeniser->sToken());
  
  m_pTask->m_pForth->m_pCompiler->m_pCurrent = pNewWord;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* >create -- create word in dictionary using string on stack                 */
/*----------------------------------------------------------------------------*/
void Step::vCreateFrom(void)
{
  ENTRY("vCreateFrom"_Stz);
  
  s32 sName;
  if ( ! m_pDataStack->bPopS32(sName) )
    THROW_DSTACK();
  
  Dic* pDic;
  if (m_pTask->m_pForth->m_oDicStack.iCount())
    pDic = m_pTask->m_pForth->m_oDicStack.pTop();
  else
    pDic = AM::pRootDic();
  
  //vTrace(": %s\n"_Stz,sName.azChar());
  Word* pNewWord = new(pDic) Word(sName);
  
  if (! m_pTask->m_pForth->m_pCompiler)
    m_pTask->m_pForth->m_pCompiler = new Compiler;
  
  m_pTask->m_pForth->m_pCompiler->m_pCurrent = pNewWord;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* >createPersistent -- create word in dic using string on stack (persistent) */
/*----------------------------------------------------------------------------*/
void Step::vCreateFromPersistent(void)
{
  ENTRY("vCreateFrom"_Stz);
  
  s32 sName;
  if ( ! m_pDataStack->bPopS32(sName) )
    THROW_DSTACK();
  
  Dic* pDic;
  if (m_pTask->m_pForth->m_oDicStack.iCount())
    pDic = m_pTask->m_pForth->m_oDicStack.pTop();
  else
    pDic = AM::pRootDic();
  
  //vTrace(": %s\n"_Stz,sName.azChar());
  Word* pNewWord = new(pDic,true) Word(sName);
  
  m_pTask->m_pForth->m_pCompiler->m_pCurrent = pNewWord;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* >createInRoot -- create word in root dictionary using string on stack      */
/*----------------------------------------------------------------------------*/
void Step::vCreateFromInRoot(void)
{
  ENTRY("vCreateFromInRoot"_Stz);
  
  s32 sName;
  if ( ! m_pDataStack->bPopS32(sName) )
    THROW_DSTACK();
  
  Dic* pDic = AM::pRootDic();
  
  //vTrace(": %s\n"_Stz,sName.azChar());
  Word* pNewWord = new(pDic) Word(sName);
  
  m_pTask->m_pForth->m_pCompiler->m_pCurrent = pNewWord;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* createTenants -- create tenant word in dictionary                          */
/*----------------------------------------------------------------------------*/
void Step::vCreateTenants(void)
{
  ENTRY("vCreateTenants"_Stz);
  
  m_pTask->m_pForth->m_pCompiler->m_pTokeniser->vNextToken();
    
  Dic* pDic = AM::pRootDic();

  Word* pNewWord = new(pDic) Word(m_pTask->m_pForth->m_pCompiler->m_pTokeniser->sToken());
  
  m_pTask->m_pForth->m_pCompiler->m_pCurrent = pNewWord;
  
  AM::vTenants(pNewWord);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* does> -- routine does                                                      */
/*----------------------------------------------------------------------------*/
void Step::vDoes(void)
{
  ENTRY("vDoes"_Stz);
  
  Word* pCurrentWord = m_pTask->m_pForth->m_pCompiler->m_pCurrent;
  
  pCurrentWord->vEncode("does_rt"_Stz);
  pCurrentWord->vEncode("exit_rt"_Stz);
  
  //pCurrentWord->vEncode("pushBody");
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* does_rt -- runtime for does>                                               */
/*----------------------------------------------------------------------------*/
void Step::vDoes_RT(void)
{
  ENTRY("vDoes_RT"_Stz);
  
  Word* pCurrentWord = m_pTask->m_pForth->m_pCompiler->m_pCurrent;
  
  pCurrentWord->vEntryStep(&Step::vEnterObject);
  pCurrentWord->vEntryCell(m_aInxPtr + 1);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* exit_rt -- runtime for exit from routine                                   */
/*----------------------------------------------------------------------------*/
void Step::vExit_RT(void)
{
  ENTRY("vExit_RT"_Stz);
  
  //vTrace("Exit: r eturn stack %lld\n"_Stz, m_pReturnStack->iValues());
  
  if ( (m_pTask->m_iTraceMask & C_TRACE_ROUTINES) )
    m_pTask->vTracePoint("<"_Stz);
  
  if (m_pLocalsFrame
      && m_pLocalsFrame->aDiscardPoint() ==
      m_pReturnStack->aPeekValue())
    m_pTask->vLocalsPop();
  
  Cell* aCell;
  if ( ! m_pReturnStack->bPopCell(aCell) )
    THROW_RSTACK();
  
  m_aInxPtr = aCell;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* find -- find and push word named on stack                                  */
/*----------------------------------------------------------------------------*/
void Step::vFind(void)
{
  ENTRY("vFind"_Stz);
  
  s256 sName;
  if ( ! m_pDataStack->bPopS256(sName) )
    THROW_DSTACK();
  
  if (m_pTask->m_iTraceMask) {
    i8 i = 0;
  }
  
  Word* pWord = m_pTask->m_pForth->m_oDicStack.pFindWord(sName);
  
  m_pDataStack->vPushPointer(pWord);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* findInCurrent -- find and push word named on stack withinin curr dic       */
/*----------------------------------------------------------------------------*/
void Step::vFindInCurrent(void)
{
  ENTRY("vFindInCurrent"_Stz);
  
  s256 sName;
  if ( ! m_pDataStack->bPopS256(sName) )
    THROW_DSTACK();
  
  if (m_pTask->m_iTraceMask) {
    i8 i = 0; i++;
  }
  
  Word* pWord = m_pTask->m_pForth->m_oDicStack.pFindInCurrent(sName);
  
  m_pDataStack->vPushPointer(pWord);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* forget -- forget the word named by next token                              */
/*----------------------------------------------------------------------------*/
void Step::vForget(void)
{
  ENTRY("vForget"_Stz);
  
  m_pTask->m_pForth->m_pCompiler->m_pTokeniser->vNextToken();
  
  Word* pWord = m_pTask->m_pForth->m_oDicStack.pFindWord(m_pTask->m_pForth->m_pCompiler->m_pTokeniser->sToken());
  
  if ( ! pWord )
    vThrow(FK_CHECK,__func__,__FILE__,__LINE__,
           "No such word: %s"_Stz,
           m_pTask->m_pForth->m_pCompiler->m_pTokeniser->sToken().azChar());
  
  s32 sName;
  
  sName = m_pTask->m_pForth->m_pCompiler->m_pTokeniser->sToken();
  
  Word* pRemovedWord = m_pTask->m_pForth->m_oDicStack.pRemoveWord(sName);
  ASSERT(pRemovedWord);
  
  delete pRemovedWord;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* immediate -- make the word being compiled an immediate word                */
/*----------------------------------------------------------------------------*/
void Step::vImmediate(void)
{
  ENTRY("vImmediate"_Stz);
  
  Word* pCurrentWord = m_pTask->m_pForth->m_pCompiler->m_pCurrent;
  
  //vTrace("Step::vImmediate: %s\n"_Stz,pCurrentWord->sName().azChar());
  
  pCurrentWord->vImmediate(true);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* locate -- locate and push word name based on the ip which is on the stack  */
/*----------------------------------------------------------------------------*/
void Step::vLocate(void)
{
  ENTRY("vLocate"_Stz);
  
  i8 iAddrAsInt;
  if ( ! m_pDataStack->bPopAsInt(iAddrAsInt) )
    THROW_DSTACK();
  
  void* aCellVoid = (void*)iAddrAsInt;
  Cell* aHasCell = _aCell(aCellVoid);
  
  Word* pLocatedWord = m_pTask->m_pForth->m_oDicStack.pLocateWord(aHasCell);
  
  if (pLocatedWord) {
    s32 sName;
    sName = pLocatedWord->sName();
    m_pDataStack->vPushS32(sName);
  }
  else {
    i8 iZero = 0;
    m_pDataStack->vPushI8(iZero);
  }
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* postpone -- encode call to word named by next token into word being compiled  */
/*----------------------------------------------------------------------------*/
void Step::vPostpone(void)
{
  ENTRY("vPostpone"_Stz);
  
  Word* pCurrentWord = m_pTask->m_pForth->m_pCompiler->m_pCurrent;
  
  // get name of word to be called
  m_pTask->m_pForth->m_pCompiler->m_pTokeniser->vNextToken();
  
  // find the word in dic stack
  Word* pCalledWord = m_pTask->m_pForth->m_oDicStack.pFindWord(m_pTask->m_pForth->m_pCompiler->m_pTokeniser->sToken());
  if ( ! pCalledWord )
    vThrow(FK_CHECK,__func__,__FILE__,__LINE__,
           "No word: %s "_Stz,
           m_pTask->m_pForth->m_pCompiler->m_pTokeniser->sToken().azChar());
  
  if (pCalledWord->bImmediate()) {
    
    // put call to immediate word into this current definition
    
    // get indirect pointer to entry step of called word
    StepFp* afpCalledEntryStep = pCalledWord->afpEntryStep();
    
    // copy indirect pointer into the current word
    pCurrentWord->vEncode(afpCalledEntryStep);
  }
  else {
    
    // put call to normal word into some future definition
    
    // copy indirect pointer into the current word
    pCurrentWord->vEncode("pushValue"_Stz,pCalledWord);
    pCurrentWord->vEncode("compile,"_Stz);
  }
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* prompt -- make the word being compiled directly runnable by prompt         */
/*----------------------------------------------------------------------------*/
void Step::vPrompt(void)
{
  ENTRY("vImmediate"_Stz);
  
  Word* pCurrentWord = m_pTask->m_pForth->m_pCompiler->m_pCurrent;
  
  pCurrentWord->vPrompt(true);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* see -- dump routine of word named by next token                            */
/*----------------------------------------------------------------------------*/
void Step::vSee(void)
{
  ENTRY("vSee"_Stz);
  
  s48K sString;
  
  m_pTask->m_pForth->m_pCompiler->m_pTokeniser->vNextToken();
  
  Word* pWord = m_pTask->m_pForth->m_oDicStack.pFindWord(m_pTask->m_pForth->m_pCompiler->m_pTokeniser->sToken());
  if ( ! pWord ) {
    vTrace("See: %s unknown\n"_Stz,m_pTask->m_pForth->m_pCompiler->m_pTokeniser->sToken().azChar());
    return;
  }
  
  vTrace("See: %s\n"_Stz,pWord->sName().azChar());
  
  Cell* aDoesCell = pWord->aEntryCell();
  
  Word* pDoesWord = (Word*)( ((u8)aDoesCell & (u8)~0xFFFF)
                                      + sizeof(HeapBuffer));
  
  while (aDoesCell < pDoesWord->aLastCell()) {
    
    StepFp fpHasStep = *aDoesCell->m_afpIntrinsic;
    
    Word* pCallsWord = m_pTask->m_pForth->m_oDicStack.pLocateWord(fpHasStep);
    
    // jumps
    if (*aDoesCell->m_afpIntrinsic == &Step::vFlowJump ||
        *aDoesCell->m_afpIntrinsic == &Step::vFlowJumpFalse ||
        *aDoesCell->m_afpIntrinsic == &Step::vFlowJumpTrue ) {
      
      vTrace("%p: %s"_Stz,aDoesCell,pCallsWord->sName().azChar());
      
      aDoesCell++;
      
      vTrace(" %p\n"_Stz,aDoesCell->m_aJumpTo);
      
      aDoesCell++;
      
      continue;
    }
    
    // push
    if ( *aDoesCell->m_afpIntrinsic == &Step::vPushValue ) {
      
      vTrace("%p: %s"_Stz,aDoesCell,pCallsWord->sName().azChar());
      
      aDoesCell++;
      
      ForthValue* aValue = reinterpret_cast<ForthValue*>(aDoesCell);
      
      if (CN_STRVAL == aValue->eClass()) {
        vTrace(" \"%s\"\n"_Stz,_aStrVal(aValue)->azChar());
      }
      else {
        aValue->vToString(sString);
        vTrace(" %s\n"_Stz,sString.azChar());
      }
      
      i8 iBytes = aValue->nSize();
      aDoesCell = aAdd<Cell*>(aDoesCell,iBytes);
      
      continue;
    }
    
    // enterRoutine
    if ( *aDoesCell->m_afpIntrinsic == &Step::vEnterRoutine ) {
      
      Word* pCallsWord = (Word*)( ((u8)aDoesCell->m_afpIntrinsic & (u8)~0xFFFF)
                                           + sizeof(HeapBuffer));
      
      vTrace("%p: %s\n"_Stz,aDoesCell,pCallsWord->sName().azChar());
      aDoesCell++;
      continue;
    }
    
    // other
    vTrace("%p: %s\n"_Stz,aDoesCell,pCallsWord->sName().azChar());
    
    aDoesCell++;
  }
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* ; -- end compilation of current word, enter interpreter state              */
/*----------------------------------------------------------------------------*/
void Step::vSemicolon(void)
{
  ENTRY("vSemicolon"_Stz);
  
  Word* pCurrentWord = m_pTask->m_pForth->m_pCompiler->m_pCurrent;
  
  if (m_pTask->m_pForth->m_pCompiler->m_nLocals) {
    
    // reset the number of local variables
    m_pTask->m_pForth->m_pCompiler->m_nLocals = 0;
    
    // pop top dictionary
    m_pTask->m_pForth->m_oDicStack.vDiscard();
    
    // enocde locals frame pop
    pCurrentWord->vEncode("locals.pop"_Stz);
  }
  
  if (! pCurrentWord->sName().bSame("exit"_Stz) &&
      m_pTask->m_pForth->m_oDicStack.pFindWord("exit"_Stz)) {
    
    if (m_pTask->m_pForth->m_oDicStack.pFindWord(pCurrentWord->szExitHook())) {
      pCurrentWord->vEncode(pCurrentWord->szExitHook());
      pCurrentWord->vEncode("exit_rt"_Stz);
    }
    else
      pCurrentWord->vEncode("exit_RT"_Stz);
  }
  else
    pCurrentWord->vEncode("exit_rt"_Stz);
  
  m_pTask->m_pForth->m_pCompiler->m_bCompiling = false;
  
  // discard subDic use
  for ( ; m_pTask->m_pForth->m_pCompiler->m_nSubDic; m_pTask->m_pForth->m_pCompiler->m_nSubDic--)
    m_pTask->m_pForth->m_oDicStack.vDiscard();
  
  m_pTask->m_pForth->m_pCompiler->m_pCurrent = nullptr;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* series-name -- generate next name within series for word named on stack    */
/*----------------------------------------------------------------------------*/
void Step::vSeriesName(void)
{
  ENTRY("vSeriesName"_Stz);
  
  s32 sName;
  if ( ! m_pDataStack->bPopS32(sName) )
    THROW_DSTACK();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  i8 iNext = 0;
  
  Dic* pRootDic = AM::pRootDic();
  
  Word* pCategory = pRootDic->pFindWord("jobSyntax:"_Stz);

  Word* pWord = nullptr;
  while ( (pWord = pRootDic->pEachWord(pCategory,pWord)) ) {

    s32 sStr;
    s32 sInt;
    
    StrLib::vSplit_si(sStr.azChar(),sInt.azChar(),pWord->sName().azChar());
    
    vTrace("  %s - %lld\n"_Stz,sStr.azChar(),sInt.iToI8());
    
    if (sStr.bSame(sName)) {
      iNext = sInt.iToI8() + 1;
      break;
    }
  }

  s32 sNextName(sName);
  sNextName.vCatSprint("%02lld"_Stz,iNext);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  m_pDataStack->vPushS32(sNextName);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* subDic -- push sub dictionary                                              */
/*----------------------------------------------------------------------------*/
void Step::vSubDic(void)
{
  ENTRY("vSubDic"_Stz);
  
  m_pTask->m_pForth->m_pCompiler->m_pTokeniser->vNextToken();
  
  s32 sNameSubDic;
  sNameSubDic = m_pTask->m_pForth->m_pCompiler->m_pTokeniser->sToken();
  
  Word* pWordSubDic = m_pTask->m_pForth->m_oDicStack.pFindWord(sNameSubDic);
  if ( ! pWordSubDic )
    vThrow(FK_CHECK,__func__,__FILE__,__LINE__,
           "No such subDic: %s"_Stz,
           sNameSubDic.azChar());
  
  Dic* pDic = _pDic(pWordSubDic->aWspArea());
  
  m_pTask->m_pForth->m_oDicStack.vPush(pDic);
  
  m_pTask->m_pForth->m_pCompiler->m_nSubDic++;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* tick -- find and push word named by next token                             */
/*----------------------------------------------------------------------------*/
void Step::vTick(void)
{
  ENTRY("vTick"_Stz);
  
  m_pTask->m_pForth->m_pCompiler->m_pTokeniser->vNextToken();
    
  s32 sWordNm;
  sWordNm = m_pTask->m_pForth->m_pCompiler->m_pTokeniser->sToken();
  
  Word* pWord = m_pTask->m_pForth->m_oDicStack.pFindWord(sWordNm);
  
  if (! pWord)
    vThrow(FK_FATAL,__func__,__FILE__,__LINE__,
           "No word: %s "_Stz,sWordNm.azChar());
  
  if (m_pTask->m_pForth->m_pCompiler->m_bCompiling) {
    Word* pCurrentWord = m_pTask->m_pForth->m_pCompiler->m_pCurrent;
    pCurrentWord->vEncode("pushValue"_Stz,pWord);
  }
  else {
    m_pDataStack->vPushPointer(pWord);
  }
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
