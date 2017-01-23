/*============================================================================*/
/* Task_Word.cpp - task word group                                            */
/*============================================================================*/

#include "StorageEngine.h"
//#include "Tasks.h"
#include "SePrompt.hpp"

extern Arguments       gxArguments;

extern AttachMap*      gpAttachMap;
extern BufferPool*     gpBufferPool;
extern Heap*           gpMediumHeap;
extern BPA*            gpBPA;
extern Dic*            gpMainDic;
extern Word*           gpWord;

extern Grid*           gpGrid;
extern StepArray*      gpStepArray;
extern ThreadList*     gpThreadList;
extern TaskList*       gpTaskList;
extern TxCollection*   gpTxCollection;

extern NP_Task         gnpRootTask;
extern StepBoard*      gpStepBoard;
extern Lock            gxDdlLock;
extern Lock            gxDicLock;
extern SEitfList*      gpSEitfList;

// words: //////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_Word(void)
{
  vAddPrimitive("allParticipants", &vAllParticipants,  true );
  vAddPrimitive("allot",           &vAllot,            false);
  vAddPrimitive("cell",            &vCell,             false);
  vAddPrimitive(":",               &vColon,            false);
  vAddPrimitive(",",               &vComma,            false);
  vAddPrimitive("compile,",        &vCompileComma,     false);
  vAddPrimitive("create",          &vCreate,           false);
  vAddPrimitive(">create",         &vCreateFrom,       false);
  vAddPrimitive("does>",           &vDoes,             true );
  vAddPrimitive("does_rt",         &vDoes_RT,          false);
  vAddPrimitive("find",            &vFind,             false);
  vAddPrimitive("findInCurrent",   &vFindInCurrent,    false);
  vAddPrimitive("forget",          &vForget,           false);
  vAddPrimitive("immediate",       &vImmediate,        true );
  vAddPrimitive("localCalls",      &vLocalCalls,       true );
  vAddPrimitive("locate",          &vLocate,           false);
  vAddPrimitive("oneParticipant",  &vOneParticipant,   true );
  vAddPrimitive("postpone",        &vPostpone,         true );
  vAddPrimitive("see",             &vSee,              false);
  vAddPrimitive(";",               &vSemicolon,        true );
  vAddPrimitive("'",               &vTick,             true );
  vAddPrimitive("word.dump",       &vWordDump,         false);
  vAddPrimitive("word.forget",     &vWordForget,       false);
  vAddPrimitive("word.immediate@", &vWordImmediateGet, false);
  vAddPrimitive("word.immediate!", &vWordImmediateSet, false);
  vAddPrimitive("word.isImmediate",&vWordIsImmediate,  false);
  vAddPrimitive("word.isPrimitive",&vWordIsPrimitive,  false);
  vAddPrimitive("word.kind@",      &vWordKindGet,      false);
  vAddPrimitive("word.kind!",      &vWordKindSet,      false);
  vAddPrimitive("word.name",       &vWordName,         false);
  vAddPrimitive("word.prev",       &vWordPrev,         false);
  vAddPrimitive("word.this",       &vWordThis,         false);
  vAddPrimitive("word.type@",      &vWordTypeGet,      false);
  vAddPrimitive("word.type!",      &vWordTypeSet,      false);
}

/*----------------------------------------------------------------------------*/
/* allParticipants -- create an allParticipant word                 immediate */
/*----------------------------------------------------------------------------*/
void Task::vAllParticipants(NNP_Task nnpTask)
{
  ENTRY("vAllParticipants");
  
  Word* pCurrentWord = nnpTask->pCurrentWord();
  
  //vTrace("Task::vImmediate: %s\n",pCurrentWord->sName().a());
  
  pCurrentWord->vDistributed(DT_ALLPARTICIPANTS);
  
  // get number of parameters
  nnpTask->vTokenNext();
  
  // convert string to integer
  char* aToken = nnpTask->m_pTokeniser->sToken().a();
  i8 iParams = String::iToI8(aToken);
  
  // set the number of parameters in the current word
  pCurrentWord->vParams(iParams);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* allot -- reserve n bytes in current word                                   */
/*----------------------------------------------------------------------------*/
void Task::vAllot(NNP_Task nnpTask)
{
  ENTRY("vAllot");
  
  i8 iBytes;
  
  if ( !  nnpTask->m_pDataStack->bPop(iBytes) )
    THROW_INVALID_DSTACK("allot");
  
  Word* pCurrentWord = nnpTask->pCurrentWord();
  
  char* pAddress = pCurrentWord->pAllot(iBytes);
  ASSERT(pAddress);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* cell -- push size of cell                                                  */
/*----------------------------------------------------------------------------*/
void Task::vCell(NNP_Task nnpTask)
{
  ENTRY("vCell");
  
  i8 nBytes = sizeof(void*);
  
  nnpTask->m_pDataStack->vPush(nBytes);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* : -- colon                                                                 */
/*----------------------------------------------------------------------------*/
void Task::vColon(NNP_Task nnpTask)
{
  ENTRY("vColon");
  
  nnpTask->m_bCompiling = true;
  
  nnpTask->vTokenNext();
  
  Word* pNewWord = new(nnpTask->m_xPathStack.pTop())
                     Word(nnpTask->m_pTokeniser->sToken().a());
  
  nnpTask->vCurrentWord(pNewWord);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* comma -- encode push value                                                 */
/*----------------------------------------------------------------------------*/
void Task::vComma(NNP_Task nnpTask)
{
  ENTRY("vComma");
  
  Word* pCurrentWord = nnpTask->pCurrentWord();
  
  ValWsp xValWsp;
  if ( ! nnpTask->m_pDataStack->bPop(xValWsp) )
    THROW_INVALID_DSTACK("comma");
  
  pCurrentWord->vEncode("pushValue",&xValWsp);
  pCurrentWord->vEncode("exit");
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* compile, -- encode call                                                    */
/*----------------------------------------------------------------------------*/
void Task::vCompileComma(NNP_Task nnpTask)
{
  ENTRY("vCompileComma");
  
  Object* pWordObject;
  if ( ! nnpTask->m_pDataStack->bPop(pWordObject) )
    THROW_INVALID_DSTACK("compile,");
  Word* pCalledWord = _pWord(pWordObject);
  
  Word* pCurrentWord = nnpTask->pCurrentWord();
  
  TaskStep* ppCalledEntryStep = pCalledWord->ppEntryStep();
  
  pCurrentWord->vEncode(ppCalledEntryStep);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* create -- create word in dictionary                                        */
/*----------------------------------------------------------------------------*/
void Task::vCreate(NNP_Task nnpTask)
{
ENTRY("vCreate");
  
  nnpTask->vTokenNext();
  
  Dic* pDic;
  if (nnpTask->m_pSEitf)
    pDic = nnpTask->m_pSEitf->pDicStack()->pTop();
  else
    pDic = nnpTask->m_xPathStack.pTop();
  
  Word* pNewWord = new(pDic) Word(nnpTask->m_pTokeniser->sToken().a());
  
  nnpTask->vCurrentWord(pNewWord);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* >create -- create word in dictionary using string on stack                 */
/*----------------------------------------------------------------------------*/
void Task::vCreateFrom(NNP_Task nnpTask)
{
ENTRY("vCreateFrom");

  s32 sName;
  if ( ! nnpTask->m_pDataStack->bPop(sName) )
    THROW_INVALID_DSTACK(">create");
  
  Dic* pDic;
  if (nnpTask->m_pSEitf)
    pDic = nnpTask->m_pSEitf->pDicStack()->pTop();
  else
    pDic = nnpTask->m_xPathStack.pTop();
  
  Word* pNewWord = new(pDic) Word(sName.a());
  
  nnpTask->vCurrentWord(pNewWord);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* does> -- routine does                                                      */
/*----------------------------------------------------------------------------*/
void Task::vDoes(NNP_Task nnpTask)
{
ENTRY("vDoes");
  
  Word* pCurrentWord = nnpTask->pCurrentWord();
  
  pCurrentWord->vEncode("does_rt");
  pCurrentWord->vEncode("exit");
  
  //pCurrentWord->vEncode("pushBody");
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* does_rt -- runtime for does>                                               */
/*----------------------------------------------------------------------------*/
void Task::vDoes_RT(NNP_Task nnpTask)
{
  ENTRY("vDoes_RT");
  
  Word* pCurrentWord = nnpTask->pCurrentWord();
  
  pCurrentWord->vEntryStep(&Task::vEnterObject);
  pCurrentWord->vEntryCell(nnpTask->IP.m_pCell+1);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* find -- find word                                                          */
/*----------------------------------------------------------------------------*/
void Task::vFind(NNP_Task nnpTask)
{
ENTRY("vFind");
  
  s256 sName;
  if ( ! nnpTask->m_pDataStack->bPop(sName) )
    THROW_INVALID_DSTACK("find");
 
  if (nnpTask->m_iTracing) {
    i8 i = 0; i++;
  }
  
  Word* pWord = nnpTask->pFindWord(sName);
  
  nnpTask->m_pDataStack->vPush(pWord);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* findInCurrent -- find word in current dictionary                           */
/*----------------------------------------------------------------------------*/
void Task::vFindInCurrent(NNP_Task nnpTask)
{
  ENTRY("vFind");
  
  s256 sName;
  if ( ! nnpTask->m_pDataStack->bPop(sName) )
    THROW_INVALID_DSTACK("findInCurrent");
  
  if (nnpTask->m_iTracing) {
    i8 i = 0; i++;
  }
  
  Word* pWord = nnpTask->pFindWordInCurrent(sName);
  
  nnpTask->m_pDataStack->vPush(pWord);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* forget -- forget word                                                      */
/*----------------------------------------------------------------------------*/
void Task::vForget(NNP_Task nnpTask)
{
ENTRY("vForget");
  
  nnpTask->vTokenNext();
  
  Word* pWord = nnpTask->pFindWord(nnpTask->m_pTokeniser->sToken());
  
  if ( ! pWord )
    THROW("No such word: %s",nnpTask->m_pTokeniser->sToken().a());
  
  bool bResult = nnpTask->bForgetWord(nnpTask->m_pTokeniser->sToken());
  ASSERT(bResult);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* immediate -- make the most recent definition an immediate word             */
/*----------------------------------------------------------------------------*/
void Task::vImmediate(NNP_Task nnpTask)
{
  ENTRY("vImmediate");
  
  Word* pCurrentWord = nnpTask->pCurrentWord();
  
  //vTrace("Task::vImmediate: %s\n",pCurrentWord->sName().a());
  
  pCurrentWord->vImmediate(true);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* localCalls -- set current word as a locals calls only word       immediate */
/*----------------------------------------------------------------------------*/
void Task::vLocalCalls(NNP_Task nnpTask)
{
  ENTRY("vLocalCalls");
  
  Word* pCurrentWord = nnpTask->pCurrentWord();
  
  pCurrentWord->vDistributed(DT_LOCALCALLS);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* locate -- locate word based on ip                                          */
/*----------------------------------------------------------------------------*/
void Task::vLocate(NNP_Task nnpTask)
{
  ENTRY("vLocate");
  
  i8 iAsInt;
  if ( ! nnpTask->m_pDataStack->bPopParamInt(iAsInt) )
    THROW_INVALID_DSTACK("locate");
  
  void* pCellVoid = (void*)iAsInt;
  Cell* pCell = _pCell(pCellVoid);
  
  Word* pLocatedWord = nnpTask->pLocateWord(pCell);
  
  if (pLocatedWord) {
    s32 sName;
    sName = pLocatedWord->sName();
    nnpTask->m_pDataStack->vPush(sName);
  }
  else {
    i8 iZero = 0;
    nnpTask->m_pDataStack->vPush(iZero);
  }
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* oneParticipant -- create a oneParticipant word                   immediate */
/*----------------------------------------------------------------------------*/
void Task::vOneParticipant(NNP_Task nnpTask)
{
  ENTRY("vAllParticipants");
  
  Word* pCurrentWord = nnpTask->pCurrentWord();
  
  //vTrace("Task::vImmediate: %s\n",pCurrentWord->sName().a());
  
  pCurrentWord->vDistributed(DT_ONEPARTICIPANT);
  
  // get number of parameters
  nnpTask->vTokenNext();
  
  // convert token to integer
  i8 iParams = String::iToI8(nnpTask->m_pTokeniser->sToken().a());
  
  // set the number of parameters in the current word
  pCurrentWord->vParams(iParams);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* postpone -- append call to next word from input to current word  immediate */
/*----------------------------------------------------------------------------*/
void Task::vPostpone(NNP_Task nnpTask)
{
  ENTRY("vPostpone");
  
  Word* pCurrentWord = nnpTask->pCurrentWord();
  
  // get name of word to be called
  nnpTask->vTokenNext();
  
  // find the word in dic stack
  Word* pCalledWord = nnpTask->pFindWord(nnpTask->m_pTokeniser->sToken());
  if ( ! pCalledWord )
    THROW("No word: %s ",nnpTask->m_pTokeniser->sToken().a());
  
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
    pCurrentWord->vEncode("pushValue",pCalledWord);
    if (WT_PRIMITIVE == pCalledWord->eWordType())
      pCurrentWord->vEncode("encodeCallPrimitive");
    else
      pCurrentWord->vEncode("encodeCallRoutine");
  }
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* see -- dump routine                                                        */
/*----------------------------------------------------------------------------*/
void Task::vSee(NNP_Task nnpTask)
{
  ENTRY("vSee");
  
  s48K sWorkspace;
  
  nnpTask->vTokenNext();
  
  Word* pWord = nnpTask->pFindWord(nnpTask->m_pTokeniser->sToken());
  if ( ! pWord ) {
    vTrace("See: %s unknown\n",nnpTask->m_pTokeniser->sToken().a());
    return;
  }
  
  vTrace("See: %s\n",pWord->sName().a());
  
  Cell* pDoesCell = pWord->pEntryCell();
  
  Word* pDoesWord = (Word*)( ((u8)pDoesCell & (u8)~0xFFFF)
                            + sizeof(HeapBuffer));
  
  while (pDoesCell < pDoesWord->pLastCell()) {
    
    Word* pCallsWord = nnpTask->m_xPathStack.pFindStep(*pDoesCell->m_ppEntryStep);
    
    // jumps
    if ( *pDoesCell->m_ppEntryStep == Task::vFlowJump ||
        *pDoesCell->m_ppEntryStep == Task::vFlowJumpFalse ||
        *pDoesCell->m_ppEntryStep == Task::vFlowJumpTrue ) {
      
      vTrace("%p: %s",pDoesCell,pCallsWord->sName().a());
      
      pDoesCell++;
      
      vTrace(" %p\n",pDoesCell->m_pJumpTo);
      
      pDoesCell++;
      
      continue;
    }
    
    // push
    if ( *pDoesCell->m_ppEntryStep == Task::vPushValue ) {
      
      vTrace("%p: %s",pDoesCell,pCallsWord->sName().a());
      
      pDoesCell++;
      
      ValWsp* pValue = reinterpret_cast<ValWsp*>(pDoesCell);
      
      if (CN_STRVAL == pValue->eClass()) {
        vTrace(" \"%s\"\n",pValue->sStr());
      }
      else {
        pValue->vToString(sWorkspace.a());
        vTrace(" %s\n",sWorkspace.a());
      }
      
      i8 iBytes = pValue->iSize();
      pDoesCell = reinterpret_cast<Cell*>(pcAdd(pDoesCell,iBytes));
      
      continue;
    }
    
    // enterRoutine
    if ( *pDoesCell->m_ppEntryStep == Task::vEnterRoutine ) {
      
      Word* pCallsWord = (Word*)( ((u8)pDoesCell->m_ppEntryStep & (u8)~0xFFFF)
                                 + sizeof(HeapBuffer));
      
      vTrace("%p: %s\n",pDoesCell,pCallsWord->sName().a());
      pDoesCell++;
      continue;
    }
    
    // other
    vTrace("%p: %s\n",pDoesCell,pCallsWord->sName().a());
    
    pDoesCell++;
  }
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* ; -- end compile routine                                                   */
/*----------------------------------------------------------------------------*/
void Task::vSemicolon(NNP_Task nnpTask)
{
  ENTRY("vSemicolon");
  
  Word* pCurrentWord = nnpTask->pCurrentWord();
  
  if (nnpTask->m_nLocals) {
    
    // reset the number of local variables
    nnpTask->m_nLocals = 0;
    
    // pop top dictionary
    nnpTask->vDicDrop();
    
    // enocde locals frame pop
    pCurrentWord->vEncode("locals.pop");
  }
  
  pCurrentWord->vEncode("exit");
  
  nnpTask->m_bCompiling = false;
  
  if (DT_ALLPARTICIPANTS == pCurrentWord->eDistributed() ||
      DT_ONEPARTICIPANT  == pCurrentWord->eDistributed()) {
    
    s32 sTildeName;
    sTildeName = "~";
    String::vConcat(sTildeName.a(),pCurrentWord->sName().a());
    
    // create new word for tilde of current word
    Dic* pDic = nnpTask->pDicPeek();
    Word* pWordTilde = new(pDic) Word(sTildeName.a());
    // ** do not set as current word **
    
    // encode the word
    ValWsp xValWsp;
    
    xValWsp = sTildeName.a();
    pWordTilde->vEncode("pushValue",&xValWsp);
    pWordTilde->vEncode("tracePoint");
    
    xValWsp = pCurrentWord->iParams();
    pWordTilde->vEncode("pushValue",&xValWsp);
    pWordTilde->vEncode("put$$");
    
    xValWsp = pCurrentWord->sName().a();
    pWordTilde->vEncode("pushValue",&xValWsp);
    
    xValWsp = "theIndirectCall";
    pWordTilde->vEncode("pushValue",&xValWsp);
    pWordTilde->vEncode("tracePoint");
    
    pWordTilde->vEncode("$$>valset");
    
    if (DT_ALLPARTICIPANTS == pCurrentWord->eDistributed())
      pWordTilde->vEncode(">>allParticipants>");
    else
      pWordTilde->vEncode(">>oneParticipant>");
    
    xValWsp = sTildeName.a();
    pWordTilde->vEncode("pushValue",&xValWsp);
    pWordTilde->vEncode("tracePoint");
    
    pWordTilde->vEncode("checkResult");
    
    pWordTilde->vEncode("exit");
  }
  
  nnpTask->vCurrentWord(nullptr);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* tick -- find next word                                                     */
/*----------------------------------------------------------------------------*/
void Task::vTick(NNP_Task nnpTask)
{
  ENTRY("vTick");
  
  nnpTask->vTokenNext();
  
  //  if ( ! bExtracted ) {
  //    String::vSprint(nnpTask->m_sException,"No name: ' ");
  //    nnpTask->m_sFile = __FILE__;
  //    throw (i8)__LINE__;
  //  }
  
  Word* pWord = nnpTask->pFindWord(nnpTask->m_pTokeniser->sToken());
  
  if ( ! pWord )
    THROW("No word: %s ",nnpTask->m_pTokeniser->sToken().a());
  
  if (nnpTask->m_bCompiling) {
    Word* pCurrentWord = nnpTask->pCurrentWord();
    pCurrentWord->vEncode("pushValue",pWord);
  }
  else {
    nnpTask->m_pDataStack->vPush(pWord);
  }
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* word -- root kind                                                          */
/*----------------------------------------------------------------------------*/
void Task::vWord(NNP_Task nnpTask)
{
  ENTRY("vWord");
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* word.forget -- forget word on stack                                            */
/*----------------------------------------------------------------------------*/
void Task::vWordForget(NNP_Task nnpTask)
{
ENTRY("vWordForget");
  
  Object* pWordObject;
  if ( ! nnpTask->m_pDataStack->bPop(pWordObject) )
    THROW_INVALID_DSTACK("vWordForget");
  Word* pWord = _pWord(pWordObject);
  
  bool bResult = nnpTask->bForgetWord(pWord->sName());
  ASSERT(bResult);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* word.dump -- dump word                                                     */
/*----------------------------------------------------------------------------*/
void Task::vWordDump(NNP_Task nnpTask)
{
ENTRY("vWordDump");
  
  Object* pWordObject;
  if ( ! nnpTask->m_pDataStack->bPop(pWordObject) )
    THROW_INVALID_DSTACK("vWordDump");
  Word* pWord = _pWord(pWordObject);
  
  pWord->vContentDump();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* word.immediate@ -- get immediate flag of word                              */
/*----------------------------------------------------------------------------*/
void Task::vWordImmediateGet(NNP_Task nnpTask)
{
  ENTRY("vWordImmediateGet");
  
  Object* pWordObject;
  if ( ! nnpTask->m_pDataStack->bPop(pWordObject) )
    THROW_INVALID_DSTACK("word.immediate@");
  Word* pWord = _pWord(pWordObject);
  
  i8 iImmediate = pWord->bImmediate();
  
  nnpTask->m_pDataStack->vPush(iImmediate);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* word.immediate! -- set immediate flag of word                              */
/*----------------------------------------------------------------------------*/
void Task::vWordImmediateSet(NNP_Task nnpTask)
{
  ENTRY("vWordImmediateSet");
  
  Object* pWordObject;
  if ( ! nnpTask->m_pDataStack->bPop(pWordObject) )
    THROW_INVALID_DSTACK("word.immediate!");
  Word* pWord = _pWord(pWordObject);
  
  i8 iImmediate;
  if ( ! nnpTask->m_pDataStack->bPop(iImmediate) )
    THROW_INVALID_DSTACK("word.immediate!");
  Word* pWordKind = _pWord(pWordObject);
  
  pWord->vImmediate(iImmediate);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* isImmediate -- is word immediate?                                          */
/*----------------------------------------------------------------------------*/
void Task::vWordIsImmediate(NNP_Task nnpTask)
{
  ENTRY("vWordIsImmediate");
  
  // we should just modify the pointers rather than actually pop
  
  Object* pWordObject;
  if ( ! nnpTask->m_pDataStack->bPop(pWordObject) )
    THROW_INVALID_DSTACK("isImmediate");
  Word* pWord = _pWord(pWordObject);
  
  i8 iIsImmediate = pWord->bImmediate() ? 1 : 0;
  
  nnpTask->m_pDataStack->vPush(iIsImmediate);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* word.isPrimitive -- is word a primitive?                                   */
/*----------------------------------------------------------------------------*/
void Task::vWordIsPrimitive(NNP_Task nnpTask)
{
  ENTRY("vWordIsPrimitive");
  
  Object* pWordObject;
  if ( ! nnpTask->m_pDataStack->bPop(pWordObject) )
    THROW_INVALID_DSTACK("word.isPrimitive");
  Word* pWord = _pWord(pWordObject);
  
  i8 iIsPrimitive = WT_PRIMITIVE == pWord->eWordType() ? 1 : 0;
  
  nnpTask->m_pDataStack->vPush(iIsPrimitive);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* word.kind@ -- word kind get                                                */
/*----------------------------------------------------------------------------*/
void Task::vWordKindGet(NNP_Task nnpTask)
{
ENTRY("vWordKindGet");
  
  Object* pWordObject;
  if ( ! nnpTask->m_pDataStack->bPop(pWordObject) )
    THROW_INVALID_DSTACK("vWordKindGet");
  Word* pWord = _pWord(pWordObject);
  
  Word* pWordKind = pWord->pKind();
  
  nnpTask->m_pDataStack->vPush(pWordKind);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* word.kind! -- word kind set                                                */
/*----------------------------------------------------------------------------*/
void Task::vWordKindSet(NNP_Task nnpTask)
{
ENTRY("vWordKindSet");
  
  Object* pWordObject;
  if ( ! nnpTask->m_pDataStack->bPop(pWordObject) )
    THROW_INVALID_DSTACK("vWordKindSet");
  Word* pWord = _pWord(pWordObject);
  
  if ( ! nnpTask->m_pDataStack->bPop(pWordObject) )
    THROW_INVALID_DSTACK("vWordKindSet");
  Word* pWordKind = _pWord(pWordObject);
  
  pWord->vKind(pWordKind);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* word.name -- push name of word                                             */
/*----------------------------------------------------------------------------*/
void Task::vWordName(NNP_Task nnpTask)
{
ENTRY("vWordName");
  
  Object* pWordObject;
  if ( ! nnpTask->m_pDataStack->bPop(pWordObject) )
    THROW_INVALID_DSTACK("word.name");
  Word* pWord = _pWord(pWordObject);
  
  s32 sWordName(pWord->sName().a());
  
  nnpTask->m_pDataStack->vPush(sWordName);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* word.prev -- push prev word                                                */
/*----------------------------------------------------------------------------*/
void Task::vWordPrev(NNP_Task nnpTask)
{
  ENTRY("vWordPrev");
  
  Object* pWordObject;
  if ( ! nnpTask->m_pDataStack->bPop(pWordObject) )
    THROW_INVALID_DSTACK("wordPrev");
  Word* pWord = _pWord(pWordObject);
  
  Word* pPrevWord = pWord->pPrevWord();
  
  nnpTask->m_pDataStack->vPush(pPrevWord);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* word.this -- push current word                                             */
/*----------------------------------------------------------------------------*/
void Task::vWordThis(NNP_Task nnpTask)
{
ENTRY("vWordThis");
  
  Word* pWord = nnpTask->pCurrentWord();
  
  nnpTask->m_pDataStack->vPush(pWord);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* word.type@ -- word type get                                                */
/*----------------------------------------------------------------------------*/
void Task::vWordTypeGet(NNP_Task nnpTask)
{
  ENTRY("vWordTypeGet");
  
  Object* pWordObject;
  if ( ! nnpTask->m_pDataStack->bPop(pWordObject) )
    THROW_INVALID_DSTACK("vWordTypeGet");
  Word* pWord = _pWord(pWordObject);
  
  eWT eWordType = pWord->eWordType();
  
  nnpTask->m_pDataStack->vPush(eWordType);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* word.type! -- word type set                                                */
/*----------------------------------------------------------------------------*/
void Task::vWordTypeSet(NNP_Task nnpTask)
{
  ENTRY("vWordTypeSet");
  
  Object* pWordObject;
  if ( ! nnpTask->m_pDataStack->bPop(pWordObject) )
    THROW_INVALID_DSTACK("vWordTypeSet");
  Word* pWord = _pWord(pWordObject);
  
  i8 iWordType;
  
  if ( ! nnpTask->m_pDataStack->bPop(iWordType) )
    THROW_INVALID_DSTACK("vWordTypeSet");
  Word* pWordKind = _pWord(pWordObject);
  
  pWord->vWordType((eWT)iWordType);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
