/*============================================================================*/
/* Steps_Encode.cpp - encode step group                                       */
/*============================================================================*/

#include "StorageEngine.h"
//#include "Tasks.h"
#include "SePrompt.hpp"

extern AttachMap*      gpAttachMap;
extern BPA*            gpBPA;
extern Dic*            gpMainDic;
extern Word*           gpWord;

extern Grid*           gpGrid;
extern StepArray*      gpStepArray;
extern ThreadList*     gpThreadList;
extern TaskList*       gpTaskList;

extern NP_Task         gnpBootTask;
extern Lock            gxDdlLock;
extern Lock            gxDicLock;
extern SEitfList*      gpSEitfList;

// encode: /////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_Encode(void)
{
  vAddPrimitive("again"_Stz,               &vEncodeAgain,         true );
  vAddPrimitive("ahead"_Stz,               &vEncodeAhead,         true );
  vAddPrimitive("encodeCallPrimitive"_Stz, &vEncodeCallPrimitive, false);
  vAddPrimitive("encodeCallRoutine"_Stz,   &vEncodeCallRoutine,   false);
  vAddPrimitive("continue"_Stz,            &vEncodeContinue,      true);
  vAddPrimitive("here"_Stz,                &vEncodeHere,          false);
  vAddPrimitive("if"_Stz,                  &vEncodeIf,            true );
  vAddPrimitive("loop"_Stz,                &vEncodeLoop,          true );
  vAddPrimitive("+loop"_Stz,               &vEncodeLoopPlus,      true );
  vAddPrimitive("next"_Stz,                &vEncodeNext,          true );
  vAddPrimitive("encodePushValue"_Stz,     &vEncodePushValue,     false);
  vAddPrimitive("then"_Stz,                &vEncodeThen,          true );
  vAddPrimitive("until"_Stz,               &vEncodeUntil,         true );
}

/*----------------------------------------------------------------------------*/
/* again -- encode jump to known backward address                   immediate */
/*----------------------------------------------------------------------------*/
void Task::vEncodeAgain(MP_Task mpTask)
{
  ENTRY("vAgain"_Stz); // ( dest -- / -- )
  
  Word* pCurrentWord = mpTask->m_pCurrentWord;
  
  // cstack: pop known backward address
  Cell* pKnownDestination;
  if ( ! mpTask->m_pDataStack->bPop(pKnownDestination) )
    THROW_INVALID_DSTACK("again"_Stz);
  
  // append : jump to known backward address
  pCurrentWord->vEncode("jump"_Stz,pKnownDestination);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* ahead -- encode jump to unknown forward address                  immediate */
/*----------------------------------------------------------------------------*/
void Task::vEncodeAhead(MP_Task mpTask)
{
  ENTRY("vAhead"_Stz);// ( -- orig / -- )
  
  Word* pCurrentWord = mpTask->m_pCurrentWord;
  
  // append : jump to unknown forward address
  pCurrentWord->vEncode("jump"_Stz);
  Cell* pUnknownOrigin = (Cell*)pCurrentWord->pAppend((Cell*)nullptr);
  
  // cstack: push unknown forward address
  mpTask->m_pDataStack->vPushCell(pUnknownOrigin);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* encodeCallPrimitive -- encode call to primitive step                       */
/*----------------------------------------------------------------------------*/
void Task::vEncodeCallPrimitive(MP_Task mpTask)
{
  ENTRY("vEncodeCallPrimitive"_Stz);
  
  Object* pWordObject;
  if ( ! mpTask->m_pDataStack->bPop(pWordObject) )
    THROW_INVALID_DSTACK("vEncodeCallPrimitive"_Stz);
  Word* pCalledWord = _pWord(pWordObject);
  
  Word* pCurrentWord = mpTask->m_pCurrentWord;
  
  TaskStep* ppCalledEntryStep = pCalledWord->ppEntryStep();
  
  pCurrentWord->vAppend(ppCalledEntryStep);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* encodeCallRoutine -- encode call to a routine of calls                     */
/*----------------------------------------------------------------------------*/
void Task::vEncodeCallRoutine(MP_Task mpTask)
{
  ENTRY("vEncodeCallRoutine"_Stz);
  
  Object* pWordObject;
  if ( ! mpTask->m_pDataStack->bPop(pWordObject) )
    THROW_INVALID_DSTACK("encodeCallRoutine"_Stz);
  
  Word* pCalledWord = _pWord(pWordObject);
  
  Word* pCurrentWord = mpTask->m_pCurrentWord;
  
  TaskStep* ppCalledEntryStep;
  if (DT_NOPARTICIPANT == pCalledWord->eDistributed() ||
      DT_LOCALCALLS    == pCurrentWord->eDistributed()) {
    ppCalledEntryStep = pCalledWord->ppEntryStep();
  }
  else {
    s32 sTildeName;
    sTildeName = "~"_Stz;
    sTildeName.vConcat(pCalledWord->sName());
    
    Word* pWordTilde = mpTask->m_xPathStack.pFindWord(sTildeName);
    ppCalledEntryStep = pWordTilde->ppEntryStep();
  }
  
  pCurrentWord->vEncode(ppCalledEntryStep);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* encode -- encode jump on true to known backward address          immediate */
/*----------------------------------------------------------------------------*/
void Task::vEncodeContinue(MP_Task mpTask)
{
  ENTRY("vContinue"_Stz); // ( -- orig / flag -- )
  
  Word* pCurrentWord = mpTask->m_pCurrentWord;
  
  // cstack: pop known backward address
  Cell* pKnownBackward;
  if ( ! mpTask->m_pDataStack->bPop(pKnownBackward) )
    THROW_INVALID_DSTACK("continue"_Stz);
  
  // append : jump on true to known backward address
  pCurrentWord->vEncode("jumpTrue"_Stz,pKnownBackward);
  
  // cstack: push known backward address
  mpTask->m_pDataStack->vPushCell(pKnownBackward);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* here -- push address of last cell of word                        immediate */
/*----------------------------------------------------------------------------*/
void Task::vEncodeHere(MP_Task mpTask)
{
  ENTRY("vFlowHere"_Stz);
  
  Word* pCurrentWord = mpTask->m_pCurrentWord;
  
  Cell* pLastCell = pCurrentWord->pLastCell();
  
  // cstack: push address of last cell
  mpTask->m_pDataStack->vPushCell(pLastCell);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* if -- encode jump on false to unknown forward address            immediate */
/*----------------------------------------------------------------------------*/
void Task::vEncodeIf(MP_Task mpTask)
{
  ENTRY("vIf"_Stz); // ( -- orig / flag -- )
  
  Word* pCurrentWord = mpTask->m_pCurrentWord;
  
  // append : jumpFalse to unknown forward address
  pCurrentWord->vEncode("jumpFalse"_Stz);
  Cell* pUnknownOrigin = (Cell*)pCurrentWord->pAppend((Cell*)nullptr);
  
  // cstack: push unknown forward address
  mpTask->m_pDataStack->vPushCell(pUnknownOrigin);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* loop -- encode runtime for : LOOP                                immediate */
/*----------------------------------------------------------------------------*/
void Task::vEncodeLoop(MP_Task mpTask)
{
  ENTRY("vLoop"_Stz);
  
  Word* pCurrentWord = mpTask->m_pCurrentWord;
  
  // increment index:
  pCurrentWord->vEncode("r>"_Stz);
  pCurrentWord->vEncode("1+"_Stz);
  pCurrentWord->vEncode(">r"_Stz);
  
  i8 iIsDo;
  // cstack: is do ?
  if ( ! mpTask->m_pDataStack->bPop(iIsDo) )
    THROW_INVALID_DSTACK("loop"_Stz);
  
  Cell* pDoLimitTest = nullptr;
  if (! iIsDo) {
    // cstack: pop destination address of backward jump
    if ( ! mpTask->m_pDataStack->bPop(pDoLimitTest) )
      THROW_INVALID_DSTACK("loop"_Stz);
    pDoLimitTest->m_pJumpTo = pCurrentWord->pLastCell();
  }
  
  pCurrentWord->vEncode("2r@"_Stz); // copy index limit to dstack
  pCurrentWord->vEncode("!="_Stz);   // test for limit > index
  
  if (iIsDo) {
    // jump on true to doHere:
    Cell* pDoHere;
    // cstack: pop destination address of backward jump
    if ( ! mpTask->m_pDataStack->bPop(pDoHere) )
      THROW_INVALID_DSTACK("loop"_Stz);
    pCurrentWord->vEncode("jumpTrue"_Stz,pDoHere);
  }
  else {
    pCurrentWord->vEncode("jumpTrue"_Stz,pDoLimitTest+1);
  }
  
  pCurrentWord->vEncode("rdrop"_Stz);
  pCurrentWord->vEncode("rdrop"_Stz);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* loopPlus -- encode runtime for : +LOOP                           immediate */
/*----------------------------------------------------------------------------*/
void Task::vEncodeLoopPlus(MP_Task mpTask)
{
  ENTRY("vLoopPlus"_Stz);
  
  Word* pCurrentWord = mpTask->m_pCurrentWord;
  
  // increment index:
  pCurrentWord->vEncode("r>"_Stz);
  pCurrentWord->vEncode("+"_Stz);
  pCurrentWord->vEncode(">r"_Stz);
  
  i8 iIsDo;
  // cstack: is do ?
  if ( ! mpTask->m_pDataStack->bPop(iIsDo) )
    THROW_INVALID_DSTACK("loop"_Stz);
  
  Cell* pDoLimitTest = nullptr;
  if (! iIsDo) {
    // cstack: pop destination address of backward jump
    if ( ! mpTask->m_pDataStack->bPop(pDoLimitTest) )
      THROW_INVALID_DSTACK("loop"_Stz);
    pDoLimitTest->m_pJumpTo = pCurrentWord->pLastCell();
  }
  
  pCurrentWord->vEncode("2r@"_Stz); // copy index limit to dstack
  pCurrentWord->vEncode("!="_Stz);   // test for limit > index
  
  if (iIsDo) {
    // jump on true to doHere:
    Cell* pDoHere;
    // cstack: pop destination address of backward jump
    if ( ! mpTask->m_pDataStack->bPop(pDoHere) )
      THROW_INVALID_DSTACK("loop"_Stz);
    pCurrentWord->vEncode("jumpTrue"_Stz,pDoHere);
  }
  else {
    pCurrentWord->vEncode("jumpTrue"_Stz,pDoLimitTest+1);
  }
  
  pCurrentWord->vEncode("rdrop"_Stz);
  pCurrentWord->vEncode("rdrop"_Stz);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* next -- encode runtime for : NEXT                                immediate */
/*----------------------------------------------------------------------------*/
void Task::vEncodeNext(MP_Task mpTask)
{
  ENTRY("vLoop"_Stz);
  
  Word* pCurrentWord = mpTask->m_pCurrentWord;
  
  Cell* pDoLimitTest = nullptr;
  // cstack: pop destination address of backward jump
  if ( ! mpTask->m_pDataStack->bPop(pDoLimitTest) )
    THROW_INVALID_DSTACK("loop"_Stz);
  pDoLimitTest->m_pJumpTo = pCurrentWord->pLastCell();
  
  pCurrentWord->vEncode("r@"_Stz); // copy index limit to dstack
  
  pCurrentWord->vEncode("jumpTrue"_Stz,pDoLimitTest+1);
  
  pCurrentWord->vEncode("rdrop"_Stz);
  pCurrentWord->vEncode("rdrop"_Stz);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* encodePushValue -- encode push value                                       */
/*----------------------------------------------------------------------------*/
void Task::vEncodePushValue(MP_Task mpTask)
{
  ENTRY("vEncodePushValue"_Stz);
  
  ValWrk xValA;
  if ( ! mpTask->m_pDataStack->bPop(xValA) )
    THROW_INVALID_DSTACK("encodePushValue"_Stz);
  
  Word* pCurrentWord = mpTask->m_pCurrentWord;
  
  pCurrentWord->vEncode("pushValue"_Stz,&xValA);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* then -- resolve jump to unknown forward address                  immediate */
/*----------------------------------------------------------------------------*/
void Task::vEncodeThen(MP_Task mpTask)
{
  ENTRY("vThen"_Stz);  // ( orig -- / -- )
  
  Word* pCurrentWord = mpTask->m_pCurrentWord;
  
  // cstack: pop unknown forward address
  Cell* pUnknownOrigin;
  if ( ! mpTask->m_pDataStack->bPop(pUnknownOrigin) )
    THROW_INVALID_DSTACK("then"_Stz);
  
  // resolve forward branch to this point in routine
  pUnknownOrigin->m_pJumpTo = pCurrentWord->pLastCell();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* until -- encode jump false to known backward address             immediate */
/*----------------------------------------------------------------------------*/
void Task::vEncodeUntil(MP_Task mpTask)
{
  ENTRY("vUntil"_Stz); // ( dest -- / flag -- )
  
  Word* pCurrentWord = mpTask->m_pCurrentWord;
  
  // cstack: pop known backward address
  Cell* pKnownDestination;
  if ( ! mpTask->m_pDataStack->bPop(pKnownDestination) )
    THROW_INVALID_DSTACK("until"_Stz);
  
  // append : jump on false to known backward address
  pCurrentWord->vEncode("jumpFalse"_Stz,pKnownDestination);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
