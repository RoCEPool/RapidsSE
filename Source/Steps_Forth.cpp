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
void Step::vAllot(MP_Task mpTask)
{
  ENTRY("vAllot"_Stz);
  
  i8 iBytes;
  
  if ( !  mpTask->m_pForth->m_pDataStack->bPop(iBytes) )
    THROW_INVALID_DSTACK("allot"_Stz);
  
  RASSERT(iBytes);
  
  Word* pCurrentWord = mpTask->m_pCompiler->m_pCurrentWord;
  
  byte* aAddress = pCurrentWord->ayAllot(iBytes);
  ASSERT(aAddress);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* cell -- push size of a cell                                                */
/*----------------------------------------------------------------------------*/
void Step::vCell(MP_Task mpTask)
{
  ENTRY("vCell"_Stz);
  
  i8 nBytes = sizeof(void*);
  
  mpTask->m_pForth->m_pDataStack->vPushI8(nBytes);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* : (aka colon) -- create word named by next token, enter compiling state    */
/*----------------------------------------------------------------------------*/
void Step::vColon(MP_Task mpTask)
{
  ENTRY("vColon"_Stz);
  
  s32 sName;
  
  mpTask->m_pCompiler->m_bCompiling = true;
  
  mpTask->m_pCompiler->m_pTokeniser->vNextToken();
  sName = mpTask->m_pCompiler->m_pTokeniser->sToken();
  
  Dic* pDic;
  if (mpTask->m_oDicStack.iCount())
    pDic = mpTask->m_oDicStack.pTop();
  else
    pDic = AM::pRootDic();
  
  
  //vTrace(": %s\n"_Stz,sName.aChar());
  
  Word* pNewWord = new(pDic) Word(sName);
  
  mpTask->m_pCompiler->m_pCurrentWord = pNewWord;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* comma -- encode push of value on stack into word being compiled            */
/*----------------------------------------------------------------------------*/
void Step::vComma(MP_Task mpTask)
{
  ENTRY("vComma"_Stz);
  
  Word* pCurrentWord = mpTask->m_pCompiler->m_pCurrentWord;
  
  GenVar xValue;
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(xValue) )
    THROW_INVALID_DSTACK("comma"_Stz);
  
  pCurrentWord->vEncode("pushValue"_Stz,&xValue);
    
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* compile, -- encode call to word on stack into word being compiled          */
/*----------------------------------------------------------------------------*/
void Step::vCompileComma(MP_Task mpTask)
{
  ENTRY("vCompileComma"_Stz);
  
  Object* pWordObject;
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(pWordObject) )
    THROW_INVALID_DSTACK("compile,"_Stz);
  Word* pCalledWord = _pWord(pWordObject);
  
  Word* pCurrentWord = mpTask->m_pCompiler->m_pCurrentWord;
  
  StepFp* afpCalledEntryStep = pCalledWord->afpEntryStep();
  
  pCurrentWord->vEncode(afpCalledEntryStep);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* create -- create word named by next token, becomes word being compiled     */
/*----------------------------------------------------------------------------*/
void Step::vCreate(MP_Task mpTask)
{
  ENTRY("vCreate"_Stz);
  
  mpTask->m_pCompiler->m_pTokeniser->vNextToken();
  
  Dic* pDic;
  if (mpTask->m_oDicStack.iCount())
    pDic = mpTask->m_oDicStack.pTop();
  else
    pDic = AM::pRootDic();
  
  
  //vTrace(": %s\n"_Stz,mpTask->m_pCompiler->m_pTokeniser->sToken().aChar());
  
  Word* pNewWord = new(pDic) Word(mpTask->m_pCompiler->m_pTokeniser->sToken());
  
  mpTask->m_pCompiler->m_pCurrentWord = pNewWord;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* createPersistent -- create word in dictionary (persistent heap)            */
/*----------------------------------------------------------------------------*/
void Step::vCreatePersistent(MP_Task mpTask)
{
  ENTRY("vCreate"_Stz);
  
  mpTask->m_pCompiler->m_pTokeniser->vNextToken();
  
  Dic* pDic;
  if (mpTask->m_oDicStack.iCount())
    pDic = mpTask->m_oDicStack.pTop();
  else
    pDic = AM::pRootDic();
  
  //vTrace(": %s\n"_Stz,mpTask->m_pCompiler->m_pTokeniser->sToken().aChar());
  
  Word* pNewWord = new(pDic,true) Word(mpTask->m_pCompiler->m_pTokeniser->sToken());
  
  mpTask->m_pCompiler->m_pCurrentWord = pNewWord;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* >create -- create word in dictionary using string on stack                 */
/*----------------------------------------------------------------------------*/
void Step::vCreateFrom(MP_Task mpTask)
{
  ENTRY("vCreateFrom"_Stz);
  
  s32 sName;
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(sName) )
    THROW_INVALID_DSTACK(">create"_Stz);
  
  Dic* pDic;
  if (mpTask->m_oDicStack.iCount())
    pDic = mpTask->m_oDicStack.pTop();
  else
    pDic = AM::pRootDic();
  
  //vTrace(": %s\n"_Stz,sName.aChar());
  Word* pNewWord = new(pDic) Word(sName);
  
  if (! mpTask->m_pCompiler)
    mpTask->m_pCompiler = new Compiler;
  
  mpTask->m_pCompiler->m_pCurrentWord = pNewWord;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* >createPersistent -- create word in dic using string on stack (persistent) */
/*----------------------------------------------------------------------------*/
void Step::vCreateFromPersistent(MP_Task mpTask)
{
  ENTRY("vCreateFrom"_Stz);
  
  s32 sName;
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(sName) )
    THROW_INVALID_DSTACK(">create"_Stz);
  
  Dic* pDic;
  if (mpTask->m_oDicStack.iCount())
    pDic = mpTask->m_oDicStack.pTop();
  else
    pDic = AM::pRootDic();
  
  //vTrace(": %s\n"_Stz,sName.aChar());
  Word* pNewWord = new(pDic,true) Word(sName);
  
  mpTask->m_pCompiler->m_pCurrentWord = pNewWord;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* >createInRoot -- create word in root dictionary using string on stack      */
/*----------------------------------------------------------------------------*/
void Step::vCreateFromInRoot(MP_Task mpTask)
{
  ENTRY("vCreateFromInRoot"_Stz);
  
  s32 sName;
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(sName) )
    THROW_INVALID_DSTACK(">createInRoot"_Stz);
  
  Dic* pDic = AM::pRootDic();
  
  //vTrace(": %s\n"_Stz,sName.aChar());
  Word* pNewWord = new(pDic) Word(sName);
  
  mpTask->m_pCompiler->m_pCurrentWord = pNewWord;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* createTenants -- create tenant word in dictionary                          */
/*----------------------------------------------------------------------------*/
void Step::vCreateTenants(MP_Task mpTask)
{
  ENTRY("vCreateTenants"_Stz);
  
  mpTask->m_pCompiler->m_pTokeniser->vNextToken();
    
  Dic* pDic = AM::pRootDic();

  Word* pNewWord = new(pDic) Word(mpTask->m_pCompiler->m_pTokeniser->sToken());
  
  mpTask->m_pCompiler->m_pCurrentWord = pNewWord;
  
  AM::vTenants(pNewWord);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* does> -- routine does                                                      */
/*----------------------------------------------------------------------------*/
void Step::vDoes(MP_Task mpTask)
{
  ENTRY("vDoes"_Stz);
  
  Word* pCurrentWord = mpTask->m_pCompiler->m_pCurrentWord;
  
  pCurrentWord->vEncode("does_rt"_Stz);
  pCurrentWord->vEncode("exit_rt"_Stz);
  
  //pCurrentWord->vEncode("pushBody");
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* does_rt -- runtime for does>                                               */
/*----------------------------------------------------------------------------*/
void Step::vDoes_RT(MP_Task mpTask)
{
  ENTRY("vDoes_RT"_Stz);
  
  Word* pCurrentWord = mpTask->m_pCompiler->m_pCurrentWord;
  
  pCurrentWord->vEntryStep(&Step::vEnterObject);
  pCurrentWord->vEntryCell(mpTask->m_pForth->m_aFipCell + 1);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* exit_rt -- runtime for exit from routine                                   */
/*----------------------------------------------------------------------------*/
void Step::vExit_RT(MP_Task mpTask)
{
  ENTRY("vExit_RT"_Stz);
  
  //vTrace("Exit: r eturn stack %lld\n"_Stz, mpTask->m_pForth->m_pReturnStack->iValues());
  
  if ( (mpTask->m_iTracing & C_TRACE_ROUTINES) )
    mpTask->vTracePoint("<"_Stz);
  
  if (mpTask->m_pForth->m_pLocalsFrame
      && mpTask->m_pForth->m_pLocalsFrame->aDiscardPoint() ==
      mpTask->m_pForth->m_pReturnStack->aPeekValue())
    mpTask->vLocalsPop();
  
  Cell* aCell;
  if ( ! mpTask->m_pForth->m_pReturnStack->bPop(aCell) )
    THROW_INVALID_RSTACK("exit_rt"_Stz);
  
  mpTask->m_pForth->m_aFipCell = aCell;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* find -- find and push word named on stack                                  */
/*----------------------------------------------------------------------------*/
void Step::vFind(MP_Task mpTask)
{
  ENTRY("vFind"_Stz);
  
  s256 sName;
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(sName) )
    THROW_INVALID_DSTACK("find"_Stz);
  
  if (mpTask->m_iTracing) {
    i8 i = 0;
  }
  
  Word* pWord = mpTask->m_oDicStack.pFindWord(sName);
  
  mpTask->m_pForth->m_pDataStack->vPushPointer(pWord);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* findInCurrent -- find and push word named on stack withinin curr dic       */
/*----------------------------------------------------------------------------*/
void Step::vFindInCurrent(MP_Task mpTask)
{
  ENTRY("vFindInCurrent"_Stz);
  
  s256 sName;
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(sName) )
    THROW_INVALID_DSTACK("findInCurrent"_Stz);
  
  if (mpTask->m_iTracing) {
    i8 i = 0; i++;
  }
  
  Word* pWord = mpTask->m_oDicStack.pFindInCurrent(sName);
  
  mpTask->m_pForth->m_pDataStack->vPushPointer(pWord);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* forget -- forget the word named by next token                              */
/*----------------------------------------------------------------------------*/
void Step::vForget(MP_Task mpTask)
{
  ENTRY("vForget"_Stz);
  
  mpTask->m_pCompiler->m_pTokeniser->vNextToken();
  
  Word* pWord = mpTask->m_oDicStack.pFindWord(mpTask->m_pCompiler->m_pTokeniser->sToken());
  
  if ( ! pWord )
    THROW("No such word: %s"_Stz,mpTask->m_pCompiler->m_pTokeniser->sToken().aChar());
  
  s32 sName;
  
  sName = mpTask->m_pCompiler->m_pTokeniser->sToken();
  
  Word* pRemovedWord = mpTask->m_oDicStack.pRemoveWord(sName);
  ASSERT(pRemovedWord);
  
  delete pRemovedWord;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* immediate -- make the word being compiled an immediate word                */
/*----------------------------------------------------------------------------*/
void Step::vImmediate(MP_Task mpTask)
{
  ENTRY("vImmediate"_Stz);
  
  Word* pCurrentWord = mpTask->m_pCompiler->m_pCurrentWord;
  
  //vTrace("Step::vImmediate: %s\n"_Stz,pCurrentWord->sName().aChar());
  
  pCurrentWord->vImmediate(true);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* locate -- locate and push word name based on the ip which is on the stack  */
/*----------------------------------------------------------------------------*/
void Step::vLocate(MP_Task mpTask)
{
  ENTRY("vLocate"_Stz);
  
  i8 iAddrAsInt;
  if ( ! mpTask->m_pForth->m_pDataStack->bPopAsInt(iAddrAsInt) )
    THROW_INVALID_DSTACK("locate"_Stz);
  
  void* aCellVoid = (void*)iAddrAsInt;
  Cell* aHasCell = _aCell(aCellVoid);
  
  Word* pLocatedWord = mpTask->m_oDicStack.pLocateWord(aHasCell);
  
  if (pLocatedWord) {
    s32 sName;
    sName = pLocatedWord->sName();
    mpTask->m_pForth->m_pDataStack->vPushS32(sName);
  }
  else {
    i8 iZero = 0;
    mpTask->m_pForth->m_pDataStack->vPushI8(iZero);
  }
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* postpone -- encode call to word named by next token into word being compiled  */
/*----------------------------------------------------------------------------*/
void Step::vPostpone(MP_Task mpTask)
{
  ENTRY("vPostpone"_Stz);
  
  Word* pCurrentWord = mpTask->m_pCompiler->m_pCurrentWord;
  
  // get name of word to be called
  mpTask->m_pCompiler->m_pTokeniser->vNextToken();
  
  // find the word in dic stack
  Word* pCalledWord = mpTask->m_oDicStack.pFindWord(mpTask->m_pCompiler->m_pTokeniser->sToken());
  if ( ! pCalledWord )
    THROW("No word: %s "_Stz,mpTask->m_pCompiler->m_pTokeniser->sToken().aChar());
  
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
void Step::vPrompt(MP_Task mpTask)
{
  ENTRY("vImmediate"_Stz);
  
  Word* pCurrentWord = mpTask->m_pCompiler->m_pCurrentWord;
  
  pCurrentWord->vPrompt(true);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* see -- dump routine of word named by next token                            */
/*----------------------------------------------------------------------------*/
void Step::vSee(MP_Task mpTask)
{
  ENTRY("vSee"_Stz);
  
  s48K sString;
  
  mpTask->m_pCompiler->m_pTokeniser->vNextToken();
  
  Word* pWord = mpTask->m_oDicStack.pFindWord(mpTask->m_pCompiler->m_pTokeniser->sToken());
  if ( ! pWord ) {
    vTrace("See: %s unknown\n"_Stz,mpTask->m_pCompiler->m_pTokeniser->sToken().aChar());
    return;
  }
  
  vTrace("See: %s\n"_Stz,pWord->sName().aChar());
  
  Cell* aDoesCell = pWord->aEntryCell();
  
  Word* pDoesWord = (Word*)( ((u8)aDoesCell & (u8)~0xFFFF)
                                      + sizeof(HeapBuffer));
  
  while (aDoesCell < pDoesWord->aLastCell()) {
    
    StepFp fpHasStep = *aDoesCell->m_afpStep;
    
    Word* pCallsWord = mpTask->m_oDicStack.pLocateWord(fpHasStep);
    
    // jumps
    if (*aDoesCell->m_afpStep == Step::vFlowJump ||
        *aDoesCell->m_afpStep == Step::vFlowJumpFalse ||
        *aDoesCell->m_afpStep == Step::vFlowJumpTrue ) {
      
      vTrace("%p: %s"_Stz,aDoesCell,pCallsWord->sName().aChar());
      
      aDoesCell++;
      
      vTrace(" %p\n"_Stz,aDoesCell->m_aJumpTo);
      
      aDoesCell++;
      
      continue;
    }
    
    // push
    if ( *aDoesCell->m_afpStep == Step::vPushValue ) {
      
      vTrace("%p: %s"_Stz,aDoesCell,pCallsWord->sName().aChar());
      
      aDoesCell++;
      
      ForthValue* aValue = reinterpret_cast<ForthValue*>(aDoesCell);
      
      if (CN_STRVAL == aValue->eClass()) {
        vTrace(" \"%s\"\n"_Stz,_aStrVal(aValue)->aChar());
      }
      else {
        aValue->vToString(sString);
        vTrace(" %s\n"_Stz,sString.aChar());
      }
      
      i8 iBytes = aValue->nSize();
      aDoesCell = aAdd<Cell*>(aDoesCell,iBytes);
      
      continue;
    }
    
    // enterRoutine
    if ( *aDoesCell->m_afpStep == Step::vEnterRoutine ) {
      
      Word* pCallsWord = (Word*)( ((u8)aDoesCell->m_afpStep & (u8)~0xFFFF)
                                           + sizeof(HeapBuffer));
      
      vTrace("%p: %s\n"_Stz,aDoesCell,pCallsWord->sName().aChar());
      aDoesCell++;
      continue;
    }
    
    // other
    vTrace("%p: %s\n"_Stz,aDoesCell,pCallsWord->sName().aChar());
    
    aDoesCell++;
  }
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* ; -- end compilation of current word, enter interpreter state              */
/*----------------------------------------------------------------------------*/
void Step::vSemicolon(MP_Task mpTask)
{
  ENTRY("vSemicolon"_Stz);
  
  Word* pCurrentWord = mpTask->m_pCompiler->m_pCurrentWord;
  
  if (mpTask->m_pCompiler->m_nLocals) {
    
    // reset the number of local variables
    mpTask->m_pCompiler->m_nLocals = 0;
    
    // pop top dictionary
    mpTask->m_oDicStack.vDiscard();
    
    // enocde locals frame pop
    pCurrentWord->vEncode("locals.pop"_Stz);
  }
  
  if (! pCurrentWord->sName().bSame("exit"_Stz) &&
      mpTask->m_oDicStack.pFindWord("exit"_Stz)) {
    
    if (mpTask->m_oDicStack.pFindWord(pCurrentWord->szExitHook())) {
      pCurrentWord->vEncode(pCurrentWord->szExitHook());
      pCurrentWord->vEncode("exit_rt"_Stz);
    }
    else
      pCurrentWord->vEncode("exit_RT"_Stz);
  }
  else
    pCurrentWord->vEncode("exit_rt"_Stz);
  
  mpTask->m_pCompiler->m_bCompiling = false;
  
  // discard subDic use
  for ( ; mpTask->m_pCompiler->m_nSubDic; mpTask->m_pCompiler->m_nSubDic--)
    mpTask->m_oDicStack.vDiscard();
  
  mpTask->m_pCompiler->m_pCurrentWord = nullptr;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* series-name -- generate next name within series for word named on stack    */
/*----------------------------------------------------------------------------*/
void Step::vSeriesName(Tasks::MP_Task mpTask)
{
  ENTRY("vSeriesName"_Stz);
  
  s32 sName;
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(sName) )
    THROW_INVALID_DSTACK("series-name"_Stz);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  i8 iNext = 0;
  
  Dic* pRootDic = AM::pRootDic();
  
  Word* pCategory = pRootDic->pFindWord("jobSyntax:"_Stz);

  Word* pWord = nullptr;
  while ( (pWord = pRootDic->pEachWord(pCategory,pWord)) ) {

    s32 sStr;
    s32 sInt;
    
    StrLib::vSplit_si(sStr.aChar(),sInt.aChar(),pWord->sName().aChar());
    
    vTrace("  %s - %lld\n"_Stz,sStr.aChar(),sInt.iToI8());
    
    if (sStr.bSame(sName)) {
      iNext = sInt.iToI8() + 1;
      break;
    }
  }

  s32 sNextName(sName);
  sNextName.vCatSprint("%02lld"_Stz,iNext);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  mpTask->m_pForth->m_pDataStack->vPushS32(sNextName);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* subDic -- push sub dictionary                                              */
/*----------------------------------------------------------------------------*/
void Step::vSubDic(MP_Task mpTask)
{
  ENTRY("vSubDic"_Stz);
  
  mpTask->m_pCompiler->m_pTokeniser->vNextToken();
  
  s32 sNameSubDic;
  sNameSubDic = mpTask->m_pCompiler->m_pTokeniser->sToken();
  
  Word* pWordSubDic = mpTask->m_oDicStack.pFindWord(sNameSubDic);
  if ( ! pWordSubDic )
    THROW("No such subDic: %s"_Stz,sNameSubDic.aChar());
  
  Dic* pDic = _pDic(pWordSubDic->aWspArea());
  
  mpTask->m_oDicStack.vPush(pDic);
  
  mpTask->m_pCompiler->m_nSubDic++;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* tick -- find and push word named by next token                             */
/*----------------------------------------------------------------------------*/
void Step::vTick(MP_Task mpTask)
{
  ENTRY("vTick"_Stz);
  
  mpTask->m_pCompiler->m_pTokeniser->vNextToken();
  
  //  if ( ! bExtracted ) {
  //    mpTask->m_sException.vSprint("No name: ' ");
  //    mpTask->m_sFile = __FILE__;
  //    throw (i8)__LINE__;
  //  }
  
  s32 sWordNm;
  sWordNm = mpTask->m_pCompiler->m_pTokeniser->sToken();
  
  Word* pWord = mpTask->m_oDicStack.pFindWord(sWordNm);
  
  if (! pWord)
    vFatal("No word: %s "_Stz,sWordNm.aChar());
  
  if (mpTask->m_pCompiler->m_bCompiling) {
    Word* pCurrentWord = mpTask->m_pCompiler->m_pCurrentWord;
    pCurrentWord->vEncode("pushValue"_Stz,pWord);
  }
  else {
    mpTask->m_pForth->m_pDataStack->vPushPointer(pWord);
  }
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
