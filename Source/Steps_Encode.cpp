/*============================================================================*/
/* Steps_Encode.cpp - encode step group                                       */
/*============================================================================*/

#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Encode(void)
{
  vAddStep("again"_Stz,               &Step::vEncodeAgain,         true );
  vAddStep("ahead"_Stz,               &Step::vEncodeAhead,         true );
  vAddStep("continue"_Stz,            &Step::vEncodeContinue,      true);
  vAddStep("encodeCallPrimitive"_Stz, &Step::vEncodeCallPrimitive);
  vAddStep("encodeCallRoutine"_Stz,   &Step::vEncodeCallRoutine);
  vAddStep("encodePushValue"_Stz,     &Step::vEncodePushValue);
  vAddStep("here"_Stz,                &Step::vEncodeHere);
  vAddStep("if"_Stz,                  &Step::vEncodeIf,            true );
  vAddStep("ifAgain"_Stz,             &Step::vEncodeIfAgain,       true );
  vAddStep("loop"_Stz,                &Step::vEncodeLoop,          true );
  vAddStep("+loop"_Stz,               &Step::vEncodeLoopPlus,      true );
  vAddStep("next"_Stz,                &Step::vEncodeNext,          true );
  vAddStep("then"_Stz,                &Step::vEncodeThen,          true );
  vAddStep("until"_Stz,               &Step::vEncodeUntil,         true );
}

/*----------------------------------------------------------------------------*/
/* again -- encode jump to known backward address                   immediate */
/*----------------------------------------------------------------------------*/
void Step::vEncodeAgain(MP_Task mpTask)
{
  ENTRY("vEncodeAgain"_Stz); // ( dest -- / -- )
  
  ForthWord* pCurrentWord = mpTask->m_pCompiler->m_pCurrentWord;
  
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
void Step::vEncodeAhead(MP_Task mpTask)
{
  ENTRY("vEncodeAhead"_Stz);// ( -- orig / -- )
  
  ForthWord* pCurrentWord = mpTask->m_pCompiler->m_pCurrentWord;
  
  // append : jump to unknown forward address
  pCurrentWord->vEncode("jump"_Stz);
  Cell* pUnknownOrigin = (Cell*)pCurrentWord->aAppend((Cell*)nullptr);
  
  // cstack: push unknown forward address
  mpTask->m_pDataStack->vPushCell(pUnknownOrigin);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* encodeCallPrimitive -- encode call to primitive step                       */
/*----------------------------------------------------------------------------*/
void Step::vEncodeCallPrimitive(MP_Task mpTask)
{
  ENTRY("vEncodeCallPrimitive"_Stz);
  
  Object* pWordObject;
  if ( ! mpTask->m_pDataStack->bPop(pWordObject) )
    THROW_INVALID_DSTACK("vEncodeCallPrimitive"_Stz);
  ForthWord* pCalledWord = _pWord(pWordObject);
  
  ForthWord* pCurrentWord = mpTask->m_pCompiler->m_pCurrentWord;
  
  StepFp* afpCalledEntryStep = pCalledWord->afpEntryStep();
  
  pCurrentWord->vAppend(afpCalledEntryStep);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* encodeCallRoutine -- encode call to a routine of calls                     */
/*----------------------------------------------------------------------------*/
void Step::vEncodeCallRoutine(MP_Task mpTask)
{
  ENTRY("vEncodeCallRoutine"_Stz);
  
  Object* pWordObject;
  if ( ! mpTask->m_pDataStack->bPop(pWordObject) )
    THROW_INVALID_DSTACK("encodeCallRoutine"_Stz);
  
  ForthWord* pCalledWord = _pWord(pWordObject);
  
  ForthWord* pCurrentWord = mpTask->m_pCompiler->m_pCurrentWord;
  
  StepFp* afpCalledEntryStep = pCalledWord->afpEntryStep();
  
  pCurrentWord->vEncode(afpCalledEntryStep);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* encode -- encode jump on true to known backward address          immediate */
/*----------------------------------------------------------------------------*/
void Step::vEncodeContinue(MP_Task mpTask)
{
  ENTRY("vEncodeContinue"_Stz); // ( -- orig / flag -- )
  
  ForthWord* pCurrentWord = mpTask->m_pCompiler->m_pCurrentWord;
  
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
void Step::vEncodeHere(MP_Task mpTask)
{
  ENTRY("vEncodeHere"_Stz);
  
  ForthWord* pCurrentWord = mpTask->m_pCompiler->m_pCurrentWord;
  
  Cell* pLastCell = pCurrentWord->aLastCell();
  
  // cstack: push address of last cell
  mpTask->m_pDataStack->vPushCell(pLastCell);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* if -- encode jump on false to unknown forward address            immediate */
/*----------------------------------------------------------------------------*/
void Step::vEncodeIf(MP_Task mpTask)
{
  ENTRY("vEncodeIf"_Stz); // ( -- orig / flag -- )
  
  ForthWord* pCurrentWord = mpTask->m_pCompiler->m_pCurrentWord;
  
  // append : jumpFalse to unknown forward address
  pCurrentWord->vEncode("jumpFalse"_Stz);
  Cell* pUnknownOrigin = (Cell*)pCurrentWord->aAppend((Cell*)nullptr);
  
  // cstack: push unknown forward address
  mpTask->m_pDataStack->vPushCell(pUnknownOrigin);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* ifAgain -- encode jump to known backward address (inside if)     immediate */
/*----------------------------------------------------------------------------*/
void Step::vEncodeIfAgain(MP_Task mpTask)
{
  ENTRY("vEncodeIfAgain"_Stz); // ( dest -- / -- )
  
  ForthWord* pCurrentWord = mpTask->m_pCompiler->m_pCurrentWord;
  
  // cstack: pop known if address
  Cell* pIfDestination;
  if ( ! mpTask->m_pDataStack->bPop(pIfDestination) )
    THROW_INVALID_DSTACK("andAgain"_Stz);
  
  // cstack: pop known backward address
  Cell* pKnownDestination;
  if ( ! mpTask->m_pDataStack->bPop(pKnownDestination) )
    THROW_INVALID_DSTACK("andAgain"_Stz);
  
  // append : jump to known backward address
  pCurrentWord->vEncode("jump"_Stz,pKnownDestination);
  
  // cstack: push known forward address
  mpTask->m_pDataStack->vPushCell(pKnownDestination);
  
  // cstack: push known forward address
  mpTask->m_pDataStack->vPushCell(pIfDestination);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* loop -- encode runtime for : LOOP                                immediate */
/*----------------------------------------------------------------------------*/
void Step::vEncodeLoop(MP_Task mpTask)
{
  ENTRY("vEncodeLoop"_Stz);
  
  ForthWord* pCurrentWord = mpTask->m_pCompiler->m_pCurrentWord;
  
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
    pDoLimitTest->m_aJumpTo = pCurrentWord->aLastCell();
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
void Step::vEncodeLoopPlus(MP_Task mpTask)
{
  ENTRY("vEncodeLoopPlus"_Stz);
  
  ForthWord* pCurrentWord = mpTask->m_pCompiler->m_pCurrentWord;
  
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
    pDoLimitTest->m_aJumpTo = pCurrentWord->aLastCell();
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
void Step::vEncodeNext(MP_Task mpTask)
{
  ENTRY("vEncodeNext"_Stz);
  
  ForthWord* pCurrentWord = mpTask->m_pCompiler->m_pCurrentWord;
  
  Cell* pDoLimitTest = nullptr;
  // cstack: pop destination address of backward jump
  if ( ! mpTask->m_pDataStack->bPop(pDoLimitTest) )
    THROW_INVALID_DSTACK("loop"_Stz);
  pDoLimitTest->m_aJumpTo = pCurrentWord->aLastCell();
  
  pCurrentWord->vEncode("r@"_Stz); // copy index limit to dstack
  
  pCurrentWord->vEncode("jumpTrue"_Stz,pDoLimitTest+1);
  
  pCurrentWord->vEncode("rdrop"_Stz);
  pCurrentWord->vEncode("rdrop"_Stz);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* encodePushValue -- encode push value                                       */
/*----------------------------------------------------------------------------*/
void Step::vEncodePushValue(MP_Task mpTask)
{
  ENTRY("vEncodePushValue"_Stz);
  
  GenVar xValA;
  if ( ! mpTask->m_pDataStack->bPop(xValA) )
    THROW_INVALID_DSTACK("encodePushValue"_Stz);
  
  ForthWord* pCurrentWord = mpTask->m_pCompiler->m_pCurrentWord;
  
  pCurrentWord->vEncode("pushValue"_Stz,&xValA);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* then -- resolve jump to unknown forward address                  immediate */
/*----------------------------------------------------------------------------*/
void Step::vEncodeThen(MP_Task mpTask)
{
  ENTRY("vEncodeThen"_Stz);  // ( orig -- / -- )
  
  ForthWord* pCurrentWord = mpTask->m_pCompiler->m_pCurrentWord;
  
  // cstack: pop unknown forward address
  Cell* pUnknownOrigin;
  if ( ! mpTask->m_pDataStack->bPop(pUnknownOrigin) )
    THROW_INVALID_DSTACK("then"_Stz);
  
  // resolve forward branch to this point in routine
  pUnknownOrigin->m_aJumpTo = pCurrentWord->aLastCell();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* until -- encode jump false to known backward address             immediate */
/*----------------------------------------------------------------------------*/
void Step::vEncodeUntil(MP_Task mpTask)
{
  ENTRY("vEncodeUntil"_Stz); // ( dest -- / flag -- )
  
  ForthWord* pCurrentWord = mpTask->m_pCompiler->m_pCurrentWord;
  
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
