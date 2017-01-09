/*============================================================================*/
/* Task_Encode.cpp - task encode group                                        */
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

// encode: /////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_Encode(void)
{
  vAddPrimitive("again",               &vEncodeAgain,         true );
  vAddPrimitive("ahead",               &vEncodeAhead,         true );
  vAddPrimitive("encodeCallPrimitive", &vEncodeCallPrimitive, false);
  vAddPrimitive("encodeCallRoutine",   &vEncodeCallRoutine,   false);
  vAddPrimitive("continue",            &vEncodeContinue,      true);
  vAddPrimitive("here",                &vEncodeHere,          false);
  vAddPrimitive("if",                  &vEncodeIf,            true );
  vAddPrimitive("loop",                &vEncodeLoop,          true );
  vAddPrimitive("+loop",               &vEncodeLoopPlus,      true );
  vAddPrimitive("next",                &vEncodeNext,          true );
  vAddPrimitive("encodePushValue",     &vEncodePushValue,     false);
  vAddPrimitive("then",                &vEncodeThen,          true );
  vAddPrimitive("until",               &vEncodeUntil,         true );
}

/*----------------------------------------------------------------------------*/
/* again -- encode jump to known backward address                   immediate */
/*----------------------------------------------------------------------------*/
void Task::vEncodeAgain(NNP_Task nnpTask)
{
  ENTRY("vAgain"); // ( dest -- / -- )
  
  Word* pCurrentWord = nnpTask->pCurrentWord();
  
  // cstack: pop known backward address
  Cell* pKnownDestination;
  if ( ! nnpTask->m_pDataStack->bPop(pKnownDestination) )
    THROW_INVALID_DSTACK("again");
  
  // append : jump to known backward address
  pCurrentWord->vEncode("jump",pKnownDestination);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* ahead -- encode jump to unknown forward address                  immediate */
/*----------------------------------------------------------------------------*/
void Task::vEncodeAhead(NNP_Task nnpTask)
{
  ENTRY("vAhead");// ( -- orig / -- )
  
  Word* pCurrentWord = nnpTask->pCurrentWord();
  
  // append : jump to unknown forward address
  pCurrentWord->vEncode("jump");
  Cell* pUnknownOrigin = (Cell*)pCurrentWord->pAppend((Cell*)nullptr);
  
  // cstack: push unknown forward address
  nnpTask->m_pDataStack->vPush(pUnknownOrigin);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* encodeCallPrimitive -- encode call to primitive step                       */
/*----------------------------------------------------------------------------*/
void Task::vEncodeCallPrimitive(NNP_Task nnpTask)
{
  ENTRY("vEncodeCallPrimitive");
  
  Object* pWordObject;
  if ( ! nnpTask->m_pDataStack->bPop(pWordObject) )
    THROW_INVALID_DSTACK("vEncodeCallPrimitive");
  Word* pCalledWord = _pWord(pWordObject);
  
  Word* pCurrentWord = nnpTask->pCurrentWord();
  
  TaskStep* ppCalledEntryStep = pCalledWord->ppEntryStep();
  
  pCurrentWord->vAppend(ppCalledEntryStep);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* encodeCallRoutine -- encode call to a routine of calls                     */
/*----------------------------------------------------------------------------*/
void Task::vEncodeCallRoutine(NNP_Task nnpTask)
{
  ENTRY("vEncodeCallRoutine");
  
  Object* pWordObject;
  if ( ! nnpTask->m_pDataStack->bPop(pWordObject) )
    THROW_INVALID_DSTACK("encodeCallRoutine");
  
  Word* pCalledWord = _pWord(pWordObject);
  
  Word* pCurrentWord = nnpTask->pCurrentWord();
  
  TaskStep* ppCalledEntryStep;
  if (DT_NOPARTICIPANT == pCalledWord->eDistributed() ||
      DT_LOCALCALLS    == pCurrentWord->eDistributed()) {
    ppCalledEntryStep = pCalledWord->ppEntryStep();
  }
  else {
    s32 sTildeName;
    sTildeName = "~";
    String::vConcat(sTildeName.a(),pCalledWord->sName().a());
    
    Word* pWordTilde = nnpTask->pFindWord(sTildeName);
    ppCalledEntryStep = pWordTilde->ppEntryStep();
  }
  
  pCurrentWord->vEncode(ppCalledEntryStep);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* encode -- encode jump on true to known backward address          immediate */
/*----------------------------------------------------------------------------*/
void Task::vEncodeContinue(NNP_Task nnpTask)
{
  ENTRY("vContinue"); // ( -- orig / flag -- )
  
  Word* pCurrentWord = nnpTask->pCurrentWord();
  
  // cstack: pop known backward address
  Cell* pKnownBackward;
  if ( ! nnpTask->m_pDataStack->bPop(pKnownBackward) )
    THROW_INVALID_DSTACK("continue");
  
  // append : jump on true to known backward address
  pCurrentWord->vEncode("jumpTrue",pKnownBackward);
  
  // cstack: push known backward address
  nnpTask->m_pDataStack->vPush(pKnownBackward);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* here -- push address of last cell of word                        immediate */
/*----------------------------------------------------------------------------*/
void Task::vEncodeHere(NNP_Task nnpTask)
{
  ENTRY("vFlowHere");
  
  Word* pCurrentWord = nnpTask->pCurrentWord();
  
  Cell* pLastCell = pCurrentWord->pLastCell();
  
  // cstack: push address of last cell
  nnpTask->m_pDataStack->vPush(pLastCell);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* if -- encode jump on false to unknown forward address            immediate */
/*----------------------------------------------------------------------------*/
void Task::vEncodeIf(NNP_Task nnpTask)
{
  ENTRY("vIf"); // ( -- orig / flag -- )
  
  Word* pCurrentWord = nnpTask->pCurrentWord();
  
  // append : jumpFalse to unknown forward address
  pCurrentWord->vEncode("jumpFalse");
  Cell* pUnknownOrigin = (Cell*)pCurrentWord->pAppend((Cell*)nullptr);
  
  // cstack: push unknown forward address
  nnpTask->m_pDataStack->vPush(pUnknownOrigin);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* loop -- encode runtime for : LOOP                                immediate */
/*----------------------------------------------------------------------------*/
void Task::vEncodeLoop(NNP_Task nnpTask)
{
  ENTRY("vLoop");
  
  Word* pCurrentWord = nnpTask->pCurrentWord();
  
  // increment index:
  pCurrentWord->vEncode("r>");
  pCurrentWord->vEncode("1+");
  pCurrentWord->vEncode(">r");
  
  i8 iIsDo;
  // cstack: is do ?
  if ( ! nnpTask->m_pDataStack->bPop(iIsDo) )
    THROW_INVALID_DSTACK("loop");
  
  Cell* pDoLimitTest = nullptr;
  if (! iIsDo) {
    // cstack: pop destination address of backward jump
    if ( ! nnpTask->m_pDataStack->bPop(pDoLimitTest) )
      THROW_INVALID_DSTACK("loop");
    pDoLimitTest->m_pJumpTo = pCurrentWord->pLastCell();
  }
  
  pCurrentWord->vEncode("2r@"); // copy index limit to dstack
  pCurrentWord->vEncode("!=");   // test for limit > index
  
  if (iIsDo) {
    // jump on true to doHere:
    Cell* pDoHere;
    // cstack: pop destination address of backward jump
    if ( ! nnpTask->m_pDataStack->bPop(pDoHere) )
      THROW_INVALID_DSTACK("loop");
    pCurrentWord->vEncode("jumpTrue",pDoHere);
  }
  else {
    pCurrentWord->vEncode("jumpTrue",pDoLimitTest+1);
  }
  
  pCurrentWord->vEncode("rdrop");
  pCurrentWord->vEncode("rdrop");
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* loopPlus -- encode runtime for : +LOOP                           immediate */
/*----------------------------------------------------------------------------*/
void Task::vEncodeLoopPlus(NNP_Task nnpTask)
{
  ENTRY("vLoopPlus");
  
  Word* pCurrentWord = nnpTask->pCurrentWord();
  
  // increment index:
  pCurrentWord->vEncode("r>");
  pCurrentWord->vEncode("+");
  pCurrentWord->vEncode(">r");
  
  i8 iIsDo;
  // cstack: is do ?
  if ( ! nnpTask->m_pDataStack->bPop(iIsDo) )
    THROW_INVALID_DSTACK("loop");
  
  Cell* pDoLimitTest = nullptr;
  if (! iIsDo) {
    // cstack: pop destination address of backward jump
    if ( ! nnpTask->m_pDataStack->bPop(pDoLimitTest) )
      THROW_INVALID_DSTACK("loop");
    pDoLimitTest->m_pJumpTo = pCurrentWord->pLastCell();
  }
  
  pCurrentWord->vEncode("2r@"); // copy index limit to dstack
  pCurrentWord->vEncode("!=");   // test for limit > index
  
  if (iIsDo) {
    // jump on true to doHere:
    Cell* pDoHere;
    // cstack: pop destination address of backward jump
    if ( ! nnpTask->m_pDataStack->bPop(pDoHere) )
      THROW_INVALID_DSTACK("loop");
    pCurrentWord->vEncode("jumpTrue",pDoHere);
  }
  else {
    pCurrentWord->vEncode("jumpTrue",pDoLimitTest+1);
  }
  
  pCurrentWord->vEncode("rdrop");
  pCurrentWord->vEncode("rdrop");
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* next -- encode runtime for : NEXT                                immediate */
/*----------------------------------------------------------------------------*/
void Task::vEncodeNext(NNP_Task nnpTask)
{
  ENTRY("vLoop");
  
  Word* pCurrentWord = nnpTask->pCurrentWord();
  
  Cell* pDoLimitTest = nullptr;
  // cstack: pop destination address of backward jump
  if ( ! nnpTask->m_pDataStack->bPop(pDoLimitTest) )
    THROW_INVALID_DSTACK("loop");
  pDoLimitTest->m_pJumpTo = pCurrentWord->pLastCell();
  
  pCurrentWord->vEncode("r@"); // copy index limit to dstack
  
  pCurrentWord->vEncode("jumpTrue",pDoLimitTest+1);
  
  pCurrentWord->vEncode("rdrop");
  pCurrentWord->vEncode("rdrop");
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* encodePushValue -- encode push value                                       */
/*----------------------------------------------------------------------------*/
void Task::vEncodePushValue(NNP_Task nnpTask)
{
  ENTRY("vEncodePushValue");
  
  ValWsp xValA;
  if ( ! nnpTask->m_pDataStack->bPop(xValA) )
    THROW_INVALID_DSTACK("encodePushValue");
  
  Word* pCurrentWord = nnpTask->pCurrentWord();
  
  pCurrentWord->vEncode("pushValue",&xValA);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* then -- resolve jump to unknown forward address                  immediate */
/*----------------------------------------------------------------------------*/
void Task::vEncodeThen(NNP_Task nnpTask)
{
  ENTRY("vThen");  // ( orig -- / -- )
  
  Word* pCurrentWord = nnpTask->pCurrentWord();
  
  // cstack: pop unknown forward address
  Cell* pUnknownOrigin;
  if ( ! nnpTask->m_pDataStack->bPop(pUnknownOrigin) )
    THROW_INVALID_DSTACK("then");
  
  // resolve forward branch to this point in routine
  pUnknownOrigin->m_pJumpTo = pCurrentWord->pLastCell();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* until -- encode jump false to known backward address             immediate */
/*----------------------------------------------------------------------------*/
void Task::vEncodeUntil(NNP_Task nnpTask)
{
  ENTRY("vUntil"); // ( dest -- / flag -- )
  
  Word* pCurrentWord = nnpTask->pCurrentWord();
  
  // cstack: pop known backward address
  Cell* pKnownDestination;
  if ( ! nnpTask->m_pDataStack->bPop(pKnownDestination) )
    THROW_INVALID_DSTACK("until");
  
  // append : jump on false to known backward address
  pCurrentWord->vEncode("jumpFalse",pKnownDestination);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
