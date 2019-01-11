/*============================================================================*/
/* Steps_Word.cpp - word step group                                           */
/*============================================================================*/

#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Word(void)
{
  vAddStep("word::this"_Stz,      &Step::vWordThis);
  
  vAddStep("allot"_Stz,           &Step::vAllot);
  vAddStep("cell"_Stz,            &Step::vCell);
  vAddStep(":"_Stz,               &Step::vColon);
  vAddStep(","_Stz,               &Step::vComma);
  vAddStep("compile,"_Stz,        &Step::vCompileComma);
  vAddStep("create"_Stz,          &Step::vCreate);
  vAddStep("createTenants"_Stz,   &Step::vCreateTenants);
  vAddStep(">create"_Stz,         &Step::vCreateFrom);
  vAddStep(">createInMain"_Stz,   &Step::vCreateFromInMain);
  vAddStep(">createPersistent"_Stz, &Step::vCreateFromPersistent);
  vAddStep("createPersistent"_Stz,&Step::vCreatePersistent);
  vAddStep("does>"_Stz,           &Step::vDoes,             true );
  vAddStep("does_rt"_Stz,         &Step::vDoes_RT);
  vAddStep("find"_Stz,            &Step::vFind);
  vAddStep("findInCurrent"_Stz,   &Step::vFindInCurrent);
  vAddStep("forget"_Stz,          &Step::vForget);
  vAddStep("immediate"_Stz,       &Step::vImmediate,        true );
  vAddStep("locate"_Stz,          &Step::vLocate);

  vAddStep("postpone"_Stz,        &Step::vPostpone,         true );
  vAddStep("see"_Stz,             &Step::vSee);
  vAddStep(";"_Stz,               &Step::vSemicolon,        true );
  vAddStep("subDic"_Stz,          &Step::vSubDic,           true );
  vAddStep("tick"_Stz,            &Step::vTick,             true );
  vAddStep("word.dump"_Stz,       &Step::vWordDump);
  vAddStep("word.forget"_Stz,     &Step::vWordForget);
  vAddStep("word.immediate@"_Stz, &Step::vWordImmediateGet);
  vAddStep("word.immediate!"_Stz, &Step::vWordImmediateSet);
  vAddStep("word.isImmediate"_Stz,&Step::vWordIsImmediate);
  vAddStep("word.isPrimitive"_Stz,&Step::vWordIsPrimitive);
  vAddStep("word.kind@"_Stz,      &Step::vWordKindGet);
  vAddStep("word.kind!"_Stz,      &Step::vWordKindSet);
  vAddStep("word.name"_Stz,       &Step::vWordName);
  vAddStep("word.prev"_Stz,       &Step::vWordPrev);
  vAddStep("word.type@"_Stz,      &Step::vWordTypeGet);
  vAddStep("word.type!"_Stz,      &Step::vWordTypeSet);
}

/*----------------------------------------------------------------------------*/
/* allot -- reserve n bytes in current word                                   */
/*----------------------------------------------------------------------------*/
void Step::vAllot(MP_Task mpTask)
{
  ENTRY("vAllot"_Stz);
  
  i8 iBytes;
  
  if ( !  mpTask->m_pDataStack->bPop(iBytes) )
    THROW_INVALID_DSTACK("allot"_Stz);
  
  RASSERT(iBytes);
  
  Word* pCurrentWord = mpTask->m_pCompiler->m_pCurrentWord;
  
  char* aAddress = pCurrentWord->aAllot(iBytes);
  ASSERT(aAddress);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* cell -- push size of cell                                                  */
/*----------------------------------------------------------------------------*/
void Step::vCell(MP_Task mpTask)
{
  ENTRY("vCell"_Stz);
  
  i8 nBytes = sizeof(void*);
  
  mpTask->m_pDataStack->vPushI8(nBytes);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* : -- colon                                                                 */
/*----------------------------------------------------------------------------*/
void Step::vColon(MP_Task mpTask)
{
  ENTRY("vColon"_Stz);
  
  s32 sName;
  
  mpTask->m_pCompiler->m_bCompiling = true;
  
  mpTask->m_pCompiler->m_pTokeniser->vNextToken();
  sName = mpTask->m_pCompiler->m_pTokeniser->sToken();
  
  Dic* pDic;
  if (mpTask->m_oPathStack.iCount())
    pDic = mpTask->m_oPathStack.pTop();
  else
    pDic = &gpAttachMap->m_xMainDic;
  
  //vTrace(": %s\n"_Stz,sName.aChar());
  
  Word* pNewWord = new(pDic) Word(sName);
  
  mpTask->m_pCompiler->m_pCurrentWord = pNewWord;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* comma -- encode push value                                                 */
/*----------------------------------------------------------------------------*/
void Step::vComma(MP_Task mpTask)
{
  ENTRY("vComma"_Stz);
  
  Word* pCurrentWord = mpTask->m_pCompiler->m_pCurrentWord;
  
  GenVar xVariable;
  if ( ! mpTask->m_pDataStack->bPop(xVariable) )
    THROW_INVALID_DSTACK("comma"_Stz);
  
  pCurrentWord->vEncode("pushValue"_Stz,&xVariable);
  
  // I replaced this with POSTPONE exit where needed -ray
  //pCurrentWord->vEncode("exit"_Stz);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* compile, -- encode call                                                    */
/*----------------------------------------------------------------------------*/
void Step::vCompileComma(MP_Task mpTask)
{
  ENTRY("vCompileComma"_Stz);
  
  Object* pWordObject;
  if ( ! mpTask->m_pDataStack->bPop(pWordObject) )
    THROW_INVALID_DSTACK("compile,"_Stz);
  Word* pCalledWord = _pWord(pWordObject);
  
  Word* pCurrentWord = mpTask->m_pCompiler->m_pCurrentWord;
  
  TaskStep* ppCalledEntryStep = pCalledWord->ppEntryStep();
  
  pCurrentWord->vEncode(ppCalledEntryStep);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* create -- create word in dictionary                                        */
/*----------------------------------------------------------------------------*/
void Step::vCreate(MP_Task mpTask)
{
ENTRY("vCreate"_Stz);
  
  mpTask->m_pCompiler->m_pTokeniser->vNextToken();
  
  Dic* pDic;
  if (mpTask->m_oPathStack.iCount())
    pDic = mpTask->m_oPathStack.pTop();
  else
    pDic = &gpAttachMap->m_xMainDic;
  
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
  if (mpTask->m_oPathStack.iCount())
    pDic = mpTask->m_oPathStack.pTop();
  else
    pDic = &gpAttachMap->m_xMainDic;
  
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
  if ( ! mpTask->m_pDataStack->bPop(sName) )
    THROW_INVALID_DSTACK(">create"_Stz);
  
  Dic* pDic;
  if (mpTask->m_oPathStack.iCount())
    pDic = mpTask->m_oPathStack.pTop();
  else
    pDic = &gpAttachMap->m_xMainDic;
  
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
  if ( ! mpTask->m_pDataStack->bPop(sName) )
    THROW_INVALID_DSTACK(">create"_Stz);
  
  Dic* pDic;
  if (mpTask->m_oPathStack.iCount())
    pDic = mpTask->m_oPathStack.pTop();
  else
    pDic = &gpAttachMap->m_xMainDic;
  
  //vTrace(": %s\n"_Stz,sName.aChar());
  Word* pNewWord = new(pDic,true) Word(sName);
  
  mpTask->m_pCompiler->m_pCurrentWord = pNewWord;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* >createInMain -- create word in main dictionary using string on stack      */
/*----------------------------------------------------------------------------*/
void Step::vCreateFromInMain(MP_Task mpTask)
{
  ENTRY("vCreateFrom"_Stz);
  
  s32 sName;
  if ( ! mpTask->m_pDataStack->bPop(sName) )
    THROW_INVALID_DSTACK(">create"_Stz);
  
  Dic* pDic = &gpAttachMap->m_xMainDic;
  
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
  ENTRY("vCreateTenant"_Stz);
  
  mpTask->m_pCompiler->m_pTokeniser->vNextToken();
  
//  Dic* pDic;
//  if (mpTask->m_oPathStack.iCount())
//    pDic = mpTask->m_oPathStack.pTop();
//  else
//    pDic = &gpAttachMap->m_xMainDic;
  
  //vTrace(": %s\n"_Stz,mpTask->m_pCompiler->m_pTokeniser->sToken().aChar());
  
  Word* pNewWord = new(nullptr) Word(mpTask->m_pCompiler->m_pTokeniser->sToken());
  
  mpTask->m_pCompiler->m_pCurrentWord = pNewWord;
  
  gpAttachMap->m_pTenants = pNewWord;
  
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
  pCurrentWord->vEncode("exit"_Stz);
  
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
  pCurrentWord->vEntryCell(mpTask->IP.m_aCell+1);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* find -- find word                                                          */
/*----------------------------------------------------------------------------*/
void Step::vFind(MP_Task mpTask)
{
ENTRY("vFind"_Stz);
  
  s256 sName;
  if ( ! mpTask->m_pDataStack->bPop(sName) )
    THROW_INVALID_DSTACK("find"_Stz);
 
  if (mpTask->m_iTracing) {
    i8 i = 0;
  }
  
  Word* pWord = mpTask->m_oPathStack.pFindWord(sName);
  
  mpTask->m_pDataStack->vPushPointer(pWord);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* findInCurrent -- find word in current dictionary                           */
/*----------------------------------------------------------------------------*/
void Step::vFindInCurrent(MP_Task mpTask)
{
  ENTRY("vFindInCurrent"_Stz);
  
  s256 sName;
  if ( ! mpTask->m_pDataStack->bPop(sName) )
    THROW_INVALID_DSTACK("findInCurrent"_Stz);
  
  if (mpTask->m_iTracing) {
    i8 i = 0; i++;
  }
  
  Word* pWord = mpTask->m_oPathStack.pFindInCurrent(sName);
  
  mpTask->m_pDataStack->vPushPointer(pWord);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* forget -- forget word                                                      */
/*----------------------------------------------------------------------------*/
void Step::vForget(MP_Task mpTask)
{
ENTRY("vForget"_Stz);
  
  mpTask->m_pCompiler->m_pTokeniser->vNextToken();
  
  Word* pWord = mpTask->m_oPathStack.pFindWord(mpTask->m_pCompiler->m_pTokeniser->sToken());
  
  if ( ! pWord )
    THROW("No such word: %s"_Stz,mpTask->m_pCompiler->m_pTokeniser->sToken().aChar());
  
  s32 sName;
  
  sName = mpTask->m_pCompiler->m_pTokeniser->sToken();
  
  Word* pRemovedWord = mpTask->m_oPathStack.pRemoveWord(sName);
  ASSERT(pRemovedWord);
  
  delete pRemovedWord;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* immediate -- make the most recent definition an immediate word             */
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
/* locate -- locate word based on ip                                          */
/*----------------------------------------------------------------------------*/
void Step::vLocate(MP_Task mpTask)
{
  ENTRY("vLocate"_Stz);
  
  i8 iAddrAsInt;
  if ( ! mpTask->m_pDataStack->bPopAsInt(iAddrAsInt) )
    THROW_INVALID_DSTACK("locate"_Stz);
  
  void* aCellVoid = (void*)iAddrAsInt;
  Cell* aHasCell = _aCell(aCellVoid);
  
  Word* pLocatedWord = mpTask->m_oPathStack.pLocateWord(aHasCell);
  
  if (pLocatedWord) {
    s32 sName;
    sName = pLocatedWord->sName();
    mpTask->m_pDataStack->vPushS32(sName);
  }
  else {
    i8 iZero = 0;
    mpTask->m_pDataStack->vPushI8(iZero);
  }
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* postpone -- append call to next word from input to current word  immediate */
/*----------------------------------------------------------------------------*/
void Step::vPostpone(MP_Task mpTask)
{
  ENTRY("vPostpone"_Stz);
  
  Word* pCurrentWord = mpTask->m_pCompiler->m_pCurrentWord;
  
  // get name of word to be called
  mpTask->m_pCompiler->m_pTokeniser->vNextToken();
  
  // find the word in dic stack
  Word* pCalledWord = mpTask->m_oPathStack.pFindWord(mpTask->m_pCompiler->m_pTokeniser->sToken());
  if ( ! pCalledWord )
    THROW("No word: %s "_Stz,mpTask->m_pCompiler->m_pTokeniser->sToken().aChar());
  
  if (pCalledWord->bImmediate()) {
    
    // put call to immediate word into this current definition
    
    // get indirect pointer to entry step of called word
    TaskStep* ppCalledEntryStep = pCalledWord->ppEntryStep();
    
    // copy indirect pointer into the current word
    pCurrentWord->vEncode(ppCalledEntryStep);
  }
  else {
  
    // put call to normal word into some future definition
    
    // copy indirect pointer into the current word
    pCurrentWord->vEncode("pushValue"_Stz,pCalledWord);
    if (WT_PRIMITIVE == pCalledWord->eWordType())
      pCurrentWord->vEncode("encodeCallPrimitive"_Stz);
    else
      pCurrentWord->vEncode("encodeCallRoutine"_Stz);
  }
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* see -- dump routine                                                        */
/*----------------------------------------------------------------------------*/
void Step::vSee(MP_Task mpTask)
{
  ENTRY("vSee"_Stz);
  
  s48K sString;
  
  mpTask->m_pCompiler->m_pTokeniser->vNextToken();
  
  Word* pWord = mpTask->m_oPathStack.pFindWord(mpTask->m_pCompiler->m_pTokeniser->sToken());
  if ( ! pWord ) {
    vTrace("See: %s unknown\n"_Stz,mpTask->m_pCompiler->m_pTokeniser->sToken().aChar());
    return;
  }
  
  vTrace("See: %s\n"_Stz,pWord->sName().aChar());
  
  Cell* aDoesCell = pWord->aEntryCell();
  
  Word* pDoesWord = (Word*)( ((u8)aDoesCell & (u8)~0xFFFF)
                            + sizeof(HeapBuffer));
  
  while (aDoesCell < pDoesWord->aLastCell()) {
    
    Word* pCallsWord = mpTask->m_oPathStack.pFindStep(*aDoesCell->m_ppEntryStep);
    
    // jumps
    if (*aDoesCell->m_ppEntryStep == Step::vFlowJump ||
        *aDoesCell->m_ppEntryStep == Step::vFlowJumpFalse ||
        *aDoesCell->m_ppEntryStep == Step::vFlowJumpTrue ) {
      
      vTrace("%p: %s"_Stz,aDoesCell,pCallsWord->sName().aChar());
      
      aDoesCell++;
      
      vTrace(" %p\n"_Stz,aDoesCell->m_pJumpTo);
      
      aDoesCell++;
      
      continue;
    }
    
    // push
    if ( *aDoesCell->m_ppEntryStep == Step::vPushValue ) {
      
      vTrace("%p: %s"_Stz,aDoesCell,pCallsWord->sName().aChar());
      
      aDoesCell++;
      
      Value* pValue = reinterpret_cast<Value*>(aDoesCell);
      
      if (CN_STRVAL == pValue->eClass()) {
        vTrace(" \"%s\"\n"_Stz,_pStrVal(pValue)->aChar());
      }
      else {
        pValue->vToString(sString);
        vTrace(" %s\n"_Stz,sString.aChar());
      }
      
      i8 iBytes = pValue->nSize();
      aDoesCell = pAdd<Cell*>(aDoesCell,iBytes);
      
      continue;
    }
    
    // enterRoutine
    if ( *aDoesCell->m_ppEntryStep == Step::vEnterRoutine ) {
      
      Word* pCallsWord = (Word*)( ((u8)aDoesCell->m_ppEntryStep & (u8)~0xFFFF)
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
/* ; -- end compile routine                                                   */
/*----------------------------------------------------------------------------*/
void Step::vSemicolon(MP_Task mpTask)
{
  ENTRY("vSemicolon"_Stz);
  
  Word* pCurrentWord = mpTask->m_pCompiler->m_pCurrentWord;
  
  if (mpTask->m_pCompiler->m_nVariables) {
    
    // reset the number of local variables
    mpTask->m_pCompiler->m_nVariables = 0;
    
    // pop top dictionary
    mpTask->m_oPathStack.vDiscard();
    
    // enocde locals frame pop
    pCurrentWord->vEncode("locals.pop"_Stz);
  }
  
  pCurrentWord->vEncode("exit"_Stz);
  
  mpTask->m_pCompiler->m_bCompiling = false;
      
  // discard subDic use
  for ( ; mpTask->m_pCompiler->m_nSubDic; mpTask->m_pCompiler->m_nSubDic--)
    mpTask->m_oPathStack.vDiscard();

  mpTask->m_pCompiler->m_pCurrentWord = nullptr;
  
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
  
  Word* pWordSubDic = mpTask->m_oPathStack.pFindWord(sNameSubDic);
  if ( ! pWordSubDic )
    THROW("No such subDic: %s"_Stz,sNameSubDic.aChar());
  
  Dic* pDic = _pDic(pWordSubDic->aBodyArea());
  
  mpTask->m_oPathStack.vPush(pDic);

  mpTask->m_pCompiler->m_nSubDic++;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* tick -- find next word                                                     */
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
  
  Word* pWord = mpTask->m_oPathStack.pFindWord(mpTask->m_pCompiler->m_pTokeniser->sToken());
  
  if ( ! pWord )
    THROW("No word: %s "_Stz,mpTask->m_pCompiler->m_pTokeniser->sToken().aChar());
  
  if (mpTask->m_pCompiler->m_bCompiling) {
    Word* pCurrentWord = mpTask->m_pCompiler->m_pCurrentWord;
    pCurrentWord->vEncode("pushValue"_Stz,pWord);
  }
  else {
    mpTask->m_pDataStack->vPushPointer(pWord);
  }
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* word -- root kind                                                          */
/*----------------------------------------------------------------------------*/
void Step::vWord(MP_Task mpTask)
{
  ENTRY("vWord"_Stz);
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* word.forget -- forget word on stack                                        */
/*----------------------------------------------------------------------------*/
void Step::vWordForget(MP_Task mpTask)
{
ENTRY("vWordForget"_Stz);
  
  Object* pWordObject;
  if ( ! mpTask->m_pDataStack->bPop(pWordObject) )
    THROW_INVALID_DSTACK("vWordForget"_Stz);
  Word* pWord = _pWord(pWordObject);
  
  Word* pRemovedWord = mpTask->m_oPathStack.pRemoveWord(pWord->sName());
  ASSERT(pRemovedWord);
  
  delete pRemovedWord;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* word.dump -- dump word                                                     */
/*----------------------------------------------------------------------------*/
void Step::vWordDump(MP_Task mpTask)
{
ENTRY("vWordDump"_Stz);
  
  Object* pWordObject;
  if ( ! mpTask->m_pDataStack->bPop(pWordObject) )
    THROW_INVALID_DSTACK("vWordDump"_Stz);
  Word* pWord = _pWord(pWordObject);
  
  pWord->vContentDump();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* word.immediate@ -- get immediate flag of word                              */
/*----------------------------------------------------------------------------*/
void Step::vWordImmediateGet(MP_Task mpTask)
{
  ENTRY("vWordImmediateGet"_Stz);
  
  Object* pWordObject;
  if ( ! mpTask->m_pDataStack->bPop(pWordObject) )
    THROW_INVALID_DSTACK("word.immediate@"_Stz);
  Word* pWord = _pWord(pWordObject);
  
  i8 iImmediate = pWord->bImmediate();
  
  mpTask->m_pDataStack->vPushI8(iImmediate);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* word.immediate! -- set immediate flag of word                              */
/*----------------------------------------------------------------------------*/
void Step::vWordImmediateSet(MP_Task mpTask)
{
  ENTRY("vWordImmediateSet"_Stz);
  
  Object* pWordObject;
  if ( ! mpTask->m_pDataStack->bPop(pWordObject) )
    THROW_INVALID_DSTACK("word.immediate!"_Stz);
  Word* pWord = _pWord(pWordObject);
  
  i8 iImmediate;
  if ( ! mpTask->m_pDataStack->bPop(iImmediate) )
    THROW_INVALID_DSTACK("word.immediate!"_Stz);
  Word* pWordKind = _pWord(pWordObject);
  
  pWord->vImmediate(iImmediate);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* isImmediate -- is word immediate?                                          */
/*----------------------------------------------------------------------------*/
void Step::vWordIsImmediate(MP_Task mpTask)
{
  ENTRY("vWordIsImmediate"_Stz);
  
  // we should just modify the pointers rather than actually pop
  
  Object* pWordObject;
  if ( ! mpTask->m_pDataStack->bPop(pWordObject) )
    THROW_INVALID_DSTACK("isImmediate"_Stz);
  Word* pWord = _pWord(pWordObject);
  
  i8 iIsImmediate = pWord->bImmediate() ? 1 : 0;
  
  mpTask->m_pDataStack->vPushI8(iIsImmediate);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* word.isPrimitive -- is word a primitive?                                   */
/*----------------------------------------------------------------------------*/
void Step::vWordIsPrimitive(MP_Task mpTask)
{
  ENTRY("vWordIsPrimitive"_Stz);
  
  Object* pWordObject;
  if ( ! mpTask->m_pDataStack->bPop(pWordObject) )
    THROW_INVALID_DSTACK("word.isPrimitive"_Stz);
  Word* pWord = _pWord(pWordObject);
  
  i8 iIsPrimitive = WT_PRIMITIVE == pWord->eWordType() ? 1 : 0;
  
  mpTask->m_pDataStack->vPushI8(iIsPrimitive);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* word.kind@ -- word kind get                                                */
/*----------------------------------------------------------------------------*/
void Step::vWordKindGet(MP_Task mpTask)
{
ENTRY("vWordKindGet"_Stz);
  
  Object* pWordObject;
  if ( ! mpTask->m_pDataStack->bPop(pWordObject) )
    THROW_INVALID_DSTACK("vWordKindGet"_Stz);
  Word* pWord = _pWord(pWordObject);
  
  Word* pWordKind = pWord->pKind();
  
  mpTask->m_pDataStack->vPushPointer(pWordKind);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* word.kind! -- word kind set                                                */
/*----------------------------------------------------------------------------*/
void Step::vWordKindSet(MP_Task mpTask)
{
ENTRY("vWordKindSet"_Stz);
  
  Object* pWordObject;
  if ( ! mpTask->m_pDataStack->bPop(pWordObject) )
    THROW_INVALID_DSTACK("vWordKindSet"_Stz);
  Word* pWord = _pWord(pWordObject);
  
  if ( ! mpTask->m_pDataStack->bPop(pWordObject) )
    THROW_INVALID_DSTACK("vWordKindSet"_Stz);
  Word* pWordKind = _pWord(pWordObject);
  
  pWord->vKind(pWordKind);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* word.name -- push name of word                                             */
/*----------------------------------------------------------------------------*/
void Step::vWordName(MP_Task mpTask)
{
ENTRY("vWordName"_Stz);
  
  Object* pWordObject;
  if ( ! mpTask->m_pDataStack->bPop(pWordObject) )
    THROW_INVALID_DSTACK("word.name"_Stz);
  Word* pWord = _pWord(pWordObject);
  
  s32 sWordName(pWord->sName());
  
  mpTask->m_pDataStack->vPushS32(sWordName);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* word.prev -- push prev word                                                */
/*----------------------------------------------------------------------------*/
void Step::vWordPrev(MP_Task mpTask)
{
  ENTRY("vWordPrev"_Stz);
  
  Object* pWordObject;
  if ( ! mpTask->m_pDataStack->bPop(pWordObject) )
    THROW_INVALID_DSTACK("wordPrev"_Stz);
  Word* pWord = _pWord(pWordObject);
  
  Word* pPrevWord = pWord->pPrevWord();
  
  mpTask->m_pDataStack->vPushPointer(pPrevWord);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* word::this -- push current word                                            */
/*----------------------------------------------------------------------------*/
void Step::vWordThis(MP_Task mpTask)
{
ENTRY("vWordThis"_Stz);
  
  Word* pWord = mpTask->m_pCompiler->m_pCurrentWord;
  
  mpTask->m_pDataStack->vPushPointer(pWord);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* word.type@ -- word type get                                                */
/*----------------------------------------------------------------------------*/
void Step::vWordTypeGet(MP_Task mpTask)
{
  ENTRY("vWordTypeGet"_Stz);
  
  Object* pWordObject;
  if ( ! mpTask->m_pDataStack->bPop(pWordObject) )
    THROW_INVALID_DSTACK("vWordTypeGet"_Stz);
  Word* pWord = _pWord(pWordObject);
  
  eWT eWordType = pWord->eWordType();
  
  mpTask->m_pDataStack->vPushI8(eWordType);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* word.type! -- word type set                                                */
/*----------------------------------------------------------------------------*/
void Step::vWordTypeSet(MP_Task mpTask)
{
  ENTRY("vWordTypeSet"_Stz);
  
  Object* pWordObject;
  if ( ! mpTask->m_pDataStack->bPop(pWordObject) )
    THROW_INVALID_DSTACK("vWordTypeSet"_Stz);
  Word* pWord = _pWord(pWordObject);
  
  i8 iWordType;
  
  if ( ! mpTask->m_pDataStack->bPop(iWordType) )
    THROW_INVALID_DSTACK("vWordTypeSet"_Stz);
  Word* pWordKind = _pWord(pWordObject);
  
  pWord->vWordType((eWT)iWordType);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
