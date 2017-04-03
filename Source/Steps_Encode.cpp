/*============================================================================*/
/* Steps_Encode.cpp - encode step group                                       */
/*============================================================================*/

#include "StorageEngine.h"
//#include "Tasks.h"
#include "SePrompt.hpp"

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
  vAddPrimitive("again"_Str,               &vEncodeAgain,         true );
  vAddPrimitive("ahead"_Str,               &vEncodeAhead,         true );
  vAddPrimitive("encodeCallPrimitive"_Str, &vEncodeCallPrimitive, false);
  vAddPrimitive("encodeCallRoutine"_Str,   &vEncodeCallRoutine,   false);
  vAddPrimitive("continue"_Str,            &vEncodeContinue,      true);
  vAddPrimitive("here"_Str,                &vEncodeHere,          false);
  vAddPrimitive("if"_Str,                  &vEncodeIf,            true );
  vAddPrimitive("loop"_Str,                &vEncodeLoop,          true );
  vAddPrimitive("+loop"_Str,               &vEncodeLoopPlus,      true );
  vAddPrimitive("next"_Str,                &vEncodeNext,          true );
  vAddPrimitive("encodePushValue"_Str,     &vEncodePushValue,     false);
  vAddPrimitive("then"_Str,                &vEncodeThen,          true );
  vAddPrimitive("until"_Str,               &vEncodeUntil,         true );
}

/*----------------------------------------------------------------------------*/
/* again -- encode jump to known backward address                   immediate */
/*----------------------------------------------------------------------------*/
void Task::vEncodeAgain(NNP_Task nnpTask)
{
  ENTRY("vAgain"_Str); // ( dest -- / -- )
  
  Word* pCurrentWord = nnpTask->m_pCurrentWord;
  
  // cstack: pop known backward address
  Cell* pKnownDestination;
  if ( ! nnpTask->m_pDataStack->bPop(pKnownDestination) )
    THROW_INVALID_DSTACK("again"_Str);
  
  // append : jump to known backward address
  pCurrentWord->vEncode("jump"_Str,pKnownDestination);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* ahead -- encode jump to unknown forward address                  immediate */
/*----------------------------------------------------------------------------*/
void Task::vEncodeAhead(NNP_Task nnpTask)
{
  ENTRY("vAhead"_Str);// ( -- orig / -- )
  
  Word* pCurrentWord = nnpTask->m_pCurrentWord;
  
  // append : jump to unknown forward address
  pCurrentWord->vEncode("jump"_Str);
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
  ENTRY("vEncodeCallPrimitive"_Str);
  
  Object* pWordObject;
  if ( ! nnpTask->m_pDataStack->bPop(pWordObject) )
    THROW_INVALID_DSTACK("vEncodeCallPrimitive"_Str);
  Word* pCalledWord = _pWord(pWordObject);
  
  Word* pCurrentWord = nnpTask->m_pCurrentWord;
  
  TaskStep* ppCalledEntryStep = pCalledWord->ppEntryStep();
  
  pCurrentWord->vAppend(ppCalledEntryStep);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* encodeCallRoutine -- encode call to a routine of calls                     */
/*----------------------------------------------------------------------------*/
void Task::vEncodeCallRoutine(NNP_Task nnpTask)
{
  ENTRY("vEncodeCallRoutine"_Str);
  
  Object* pWordObject;
  if ( ! nnpTask->m_pDataStack->bPop(pWordObject) )
    THROW_INVALID_DSTACK("encodeCallRoutine"_Str);
  
  Word* pCalledWord = _pWord(pWordObject);
  
  Word* pCurrentWord = nnpTask->m_pCurrentWord;
  
  TaskStep* ppCalledEntryStep;
  if (DT_NOPARTICIPANT == pCalledWord->eDistributed() ||
      DT_LOCALCALLS    == pCurrentWord->eDistributed()) {
    ppCalledEntryStep = pCalledWord->ppEntryStep();
  }
  else {
    s32 sTildeName;
    sTildeName = "~"_Str;
    sTildeName.vConcat(pCalledWord->sName());
    
    Word* pWordTilde = nnpTask->m_xPathStack.pFindWord(sTildeName);
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
  ENTRY("vContinue"_Str); // ( -- orig / flag -- )
  
  Word* pCurrentWord = nnpTask->m_pCurrentWord;
  
  // cstack: pop known backward address
  Cell* pKnownBackward;
  if ( ! nnpTask->m_pDataStack->bPop(pKnownBackward) )
    THROW_INVALID_DSTACK("continue"_Str);
  
  // append : jump on true to known backward address
  pCurrentWord->vEncode("jumpTrue"_Str,pKnownBackward);
  
  // cstack: push known backward address
  nnpTask->m_pDataStack->vPush(pKnownBackward);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* here -- push address of last cell of word                        immediate */
/*----------------------------------------------------------------------------*/
void Task::vEncodeHere(NNP_Task nnpTask)
{
  ENTRY("vFlowHere"_Str);
  
  Word* pCurrentWord = nnpTask->m_pCurrentWord;
  
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
  ENTRY("vIf"_Str); // ( -- orig / flag -- )
  
  Word* pCurrentWord = nnpTask->m_pCurrentWord;
  
  // append : jumpFalse to unknown forward address
  pCurrentWord->vEncode("jumpFalse"_Str);
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
  ENTRY("vLoop"_Str);
  
  Word* pCurrentWord = nnpTask->m_pCurrentWord;
  
  // increment index:
  pCurrentWord->vEncode("r>"_Str);
  pCurrentWord->vEncode("1+"_Str);
  pCurrentWord->vEncode(">r"_Str);
  
  i8 iIsDo;
  // cstack: is do ?
  if ( ! nnpTask->m_pDataStack->bPop(iIsDo) )
    THROW_INVALID_DSTACK("loop"_Str);
  
  Cell* pDoLimitTest = nullptr;
  if (! iIsDo) {
    // cstack: pop destination address of backward jump
    if ( ! nnpTask->m_pDataStack->bPop(pDoLimitTest) )
      THROW_INVALID_DSTACK("loop"_Str);
    pDoLimitTest->m_pJumpTo = pCurrentWord->pLastCell();
  }
  
  pCurrentWord->vEncode("2r@"_Str); // copy index limit to dstack
  pCurrentWord->vEncode("!="_Str);   // test for limit > index
  
  if (iIsDo) {
    // jump on true to doHere:
    Cell* pDoHere;
    // cstack: pop destination address of backward jump
    if ( ! nnpTask->m_pDataStack->bPop(pDoHere) )
      THROW_INVALID_DSTACK("loop"_Str);
    pCurrentWord->vEncode("jumpTrue"_Str,pDoHere);
  }
  else {
    pCurrentWord->vEncode("jumpTrue"_Str,pDoLimitTest+1);
  }
  
  pCurrentWord->vEncode("rdrop"_Str);
  pCurrentWord->vEncode("rdrop"_Str);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* loopPlus -- encode runtime for : +LOOP                           immediate */
/*----------------------------------------------------------------------------*/
void Task::vEncodeLoopPlus(NNP_Task nnpTask)
{
  ENTRY("vLoopPlus"_Str);
  
  Word* pCurrentWord = nnpTask->m_pCurrentWord;
  
  // increment index:
  pCurrentWord->vEncode("r>"_Str);
  pCurrentWord->vEncode("+"_Str);
  pCurrentWord->vEncode(">r"_Str);
  
  i8 iIsDo;
  // cstack: is do ?
  if ( ! nnpTask->m_pDataStack->bPop(iIsDo) )
    THROW_INVALID_DSTACK("loop"_Str);
  
  Cell* pDoLimitTest = nullptr;
  if (! iIsDo) {
    // cstack: pop destination address of backward jump
    if ( ! nnpTask->m_pDataStack->bPop(pDoLimitTest) )
      THROW_INVALID_DSTACK("loop"_Str);
    pDoLimitTest->m_pJumpTo = pCurrentWord->pLastCell();
  }
  
  pCurrentWord->vEncode("2r@"_Str); // copy index limit to dstack
  pCurrentWord->vEncode("!="_Str);   // test for limit > index
  
  if (iIsDo) {
    // jump on true to doHere:
    Cell* pDoHere;
    // cstack: pop destination address of backward jump
    if ( ! nnpTask->m_pDataStack->bPop(pDoHere) )
      THROW_INVALID_DSTACK("loop"_Str);
    pCurrentWord->vEncode("jumpTrue"_Str,pDoHere);
  }
  else {
    pCurrentWord->vEncode("jumpTrue"_Str,pDoLimitTest+1);
  }
  
  pCurrentWord->vEncode("rdrop"_Str);
  pCurrentWord->vEncode("rdrop"_Str);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* next -- encode runtime for : NEXT                                immediate */
/*----------------------------------------------------------------------------*/
void Task::vEncodeNext(NNP_Task nnpTask)
{
  ENTRY("vLoop"_Str);
  
  Word* pCurrentWord = nnpTask->m_pCurrentWord;
  
  Cell* pDoLimitTest = nullptr;
  // cstack: pop destination address of backward jump
  if ( ! nnpTask->m_pDataStack->bPop(pDoLimitTest) )
    THROW_INVALID_DSTACK("loop"_Str);
  pDoLimitTest->m_pJumpTo = pCurrentWord->pLastCell();
  
  pCurrentWord->vEncode("r@"_Str); // copy index limit to dstack
  
  pCurrentWord->vEncode("jumpTrue"_Str,pDoLimitTest+1);
  
  pCurrentWord->vEncode("rdrop"_Str);
  pCurrentWord->vEncode("rdrop"_Str);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* encodePushValue -- encode push value                                       */
/*----------------------------------------------------------------------------*/
void Task::vEncodePushValue(NNP_Task nnpTask)
{
  ENTRY("vEncodePushValue"_Str);
  
  ValWsp xValA;
  if ( ! nnpTask->m_pDataStack->bPop(xValA) )
    THROW_INVALID_DSTACK("encodePushValue"_Str);
  
  Word* pCurrentWord = nnpTask->m_pCurrentWord;
  
  pCurrentWord->vEncode("pushValue"_Str,&xValA);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* then -- resolve jump to unknown forward address                  immediate */
/*----------------------------------------------------------------------------*/
void Task::vEncodeThen(NNP_Task nnpTask)
{
  ENTRY("vThen"_Str);  // ( orig -- / -- )
  
  Word* pCurrentWord = nnpTask->m_pCurrentWord;
  
  // cstack: pop unknown forward address
  Cell* pUnknownOrigin;
  if ( ! nnpTask->m_pDataStack->bPop(pUnknownOrigin) )
    THROW_INVALID_DSTACK("then"_Str);
  
  // resolve forward branch to this point in routine
  pUnknownOrigin->m_pJumpTo = pCurrentWord->pLastCell();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* until -- encode jump false to known backward address             immediate */
/*----------------------------------------------------------------------------*/
void Task::vEncodeUntil(NNP_Task nnpTask)
{
  ENTRY("vUntil"_Str); // ( dest -- / flag -- )
  
  Word* pCurrentWord = nnpTask->m_pCurrentWord;
  
  // cstack: pop known backward address
  Cell* pKnownDestination;
  if ( ! nnpTask->m_pDataStack->bPop(pKnownDestination) )
    THROW_INVALID_DSTACK("until"_Str);
  
  // append : jump on false to known backward address
  pCurrentWord->vEncode("jumpFalse"_Str,pKnownDestination);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
