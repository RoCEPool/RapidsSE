/*============================================================================*/
/* Steps_Encode.cpp - encode step group                                       */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Encode(void)
{
  vAddStep("again"_Stz,               &Step::vEncodeAgain,         true );
  vAddStep("ahead"_Stz,               &Step::vEncodeAhead,         true );
  vAddStep("continue"_Stz,            &Step::vEncodeContinue,      true );
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
void Step::vEncodeAgain(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vEncodeAgain"_Stz); // ( dest -- / -- )
  
  Word* pCurrentWord = mpTask->m_pCompiler->m_pCurrent;
  
  // cstack: pop known backward address
  Cell* pKnownDestination;
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(pKnownDestination) )
    vThrow(__func__,__FILE__,__LINE__,FK_DSTACK,__STEP__);
  
  // append : jump to known backward address
  pCurrentWord->vEncode("jump"_Stz,pKnownDestination);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* ahead -- encode jump to unknown forward address                  immediate */
/*----------------------------------------------------------------------------*/
void Step::vEncodeAhead(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vEncodeAhead"_Stz);// ( -- orig / -- )
  
  Word* pCurrentWord = mpTask->m_pCompiler->m_pCurrent;
  
  // append : jump to unknown forward address
  pCurrentWord->vEncode("jump"_Stz);
  Cell* pUnknownOrigin = (Cell*)pCurrentWord->aAppend((Cell*)nullptr);
  
  // cstack: push unknown forward address
  mpTask->m_pForth->m_pDataStack->vPushCell(pUnknownOrigin);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* encode -- encode jump on true to known backward address          immediate */
/*----------------------------------------------------------------------------*/
void Step::vEncodeContinue(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vEncodeContinue"_Stz); // ( -- orig / flag -- )
  
  Word* pCurrentWord = mpTask->m_pCompiler->m_pCurrent;
  
  // cstack: pop known backward address
  Cell* pKnownBackward;
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(pKnownBackward) )
    vThrow(__func__,__FILE__,__LINE__,FK_DSTACK,__STEP__);
  
  // append : jump on true to known backward address
  pCurrentWord->vEncode("jumpTrue"_Stz,pKnownBackward);
  
  // cstack: push known backward address
  mpTask->m_pForth->m_pDataStack->vPushCell(pKnownBackward);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* here -- push address of last cell of word                        immediate */
/*----------------------------------------------------------------------------*/
void Step::vEncodeHere(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vEncodeHere"_Stz);
  
  Word* pCurrentWord = mpTask->m_pCompiler->m_pCurrent;
  
  Cell* pLastCell = pCurrentWord->aLastCell();
  
  // cstack: push address of last cell
  mpTask->m_pForth->m_pDataStack->vPushCell(pLastCell);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* if -- encode jump on false to unknown forward address            immediate */
/*----------------------------------------------------------------------------*/
void Step::vEncodeIf(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vEncodeIf"_Stz); // ( -- orig / flag -- )
  
  Word* pCurrentWord = mpTask->m_pCompiler->m_pCurrent;
  
  // append : jumpFalse to unknown forward address
  pCurrentWord->vEncode("jumpFalse"_Stz);
  Cell* pUnknownOrigin = (Cell*)pCurrentWord->aAppend((Cell*)nullptr);
  
  // cstack: push unknown forward address
  mpTask->m_pForth->m_pDataStack->vPushCell(pUnknownOrigin);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* ifAgain -- encode jump to known backward address (inside if)     immediate */
/*----------------------------------------------------------------------------*/
void Step::vEncodeIfAgain(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vEncodeIfAgain"_Stz); // ( dest -- / -- )
  
  Word* pCurrentWord = mpTask->m_pCompiler->m_pCurrent;
  
  // cstack: pop known if address
  Cell* pIfDestination;
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(pIfDestination) )
    vThrow(__func__,__FILE__,__LINE__,FK_DSTACK,__STEP__);
  
  // cstack: pop known backward address
  Cell* pKnownDestination;
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(pKnownDestination) )
    vThrow(__func__,__FILE__,__LINE__,FK_DSTACK,__STEP__);
  
  // append : jump to known backward address
  pCurrentWord->vEncode("jump"_Stz,pKnownDestination);
  
  // cstack: push known forward address
  mpTask->m_pForth->m_pDataStack->vPushCell(pKnownDestination);
  
  // cstack: push known forward address
  mpTask->m_pForth->m_pDataStack->vPushCell(pIfDestination);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* loop -- encode runtime for : LOOP                                immediate */
/*----------------------------------------------------------------------------*/
void Step::vEncodeLoop(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vEncodeLoop"_Stz);
  
  Word* pCurrentWord = mpTask->m_pCompiler->m_pCurrent;
  
  // increment index:
  pCurrentWord->vEncode("r>"_Stz);
  pCurrentWord->vEncode("1+"_Stz);
  pCurrentWord->vEncode(">r"_Stz);
  
  i8 iIsDo;
  // cstack: is do ?
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(iIsDo) )
    vThrow(__func__,__FILE__,__LINE__,FK_DSTACK,__STEP__);
  
  Cell* pDoLimitTest = nullptr;
  if (! iIsDo) {
    // cstack: pop destination address of backward jump
    if ( ! mpTask->m_pForth->m_pDataStack->bPop(pDoLimitTest) )
      vThrow(__func__,__FILE__,__LINE__,FK_DSTACK,__STEP__);
    pDoLimitTest->m_aJumpTo = pCurrentWord->aLastCell();
  }
  
  pCurrentWord->vEncode("2r@"_Stz); // copy index limit to dstack
  pCurrentWord->vEncode("!="_Stz);   // test for limit > index
  
  if (iIsDo) {
    // jump on true to doHere:
    Cell* pDoHere;
    // cstack: pop destination address of backward jump
    if ( ! mpTask->m_pForth->m_pDataStack->bPop(pDoHere) )
      vThrow(__func__,__FILE__,__LINE__,FK_DSTACK,__STEP__);
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
void Step::vEncodeLoopPlus(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vEncodeLoopPlus"_Stz);
  
  Word* pCurrentWord = mpTask->m_pCompiler->m_pCurrent;
  
  // increment index:
  pCurrentWord->vEncode("r>"_Stz);
  pCurrentWord->vEncode("+"_Stz);
  pCurrentWord->vEncode(">r"_Stz);
  
  i8 iIsDo;
  // cstack: is do ?
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(iIsDo) )
    vThrow(__func__,__FILE__,__LINE__,FK_DSTACK,__STEP__);
  
  Cell* pDoLimitTest = nullptr;
  if (! iIsDo) {
    // cstack: pop destination address of backward jump
    if ( ! mpTask->m_pForth->m_pDataStack->bPop(pDoLimitTest) )
      vThrow(__func__,__FILE__,__LINE__,FK_DSTACK,__STEP__);
    pDoLimitTest->m_aJumpTo = pCurrentWord->aLastCell();
  }
  
  pCurrentWord->vEncode("2r@"_Stz); // copy index limit to dstack
  pCurrentWord->vEncode("!="_Stz);   // test for limit > index
  
  if (iIsDo) {
    // jump on true to doHere:
    Cell* pDoHere;
    // cstack: pop destination address of backward jump
    if ( ! mpTask->m_pForth->m_pDataStack->bPop(pDoHere) )
      vThrow(__func__,__FILE__,__LINE__,FK_DSTACK,__STEP__);
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
void Step::vEncodeNext(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vEncodeNext"_Stz);
  
  Word* pCurrentWord = mpTask->m_pCompiler->m_pCurrent;
  
  Cell* pDoLimitTest = nullptr;
  // cstack: pop destination address of backward jump
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(pDoLimitTest) )
    vThrow(__func__,__FILE__,__LINE__,FK_DSTACK,__STEP__);
  pDoLimitTest->m_aJumpTo = pCurrentWord->aLastCell();
  
  pCurrentWord->vEncode("r@"_Stz); // copy index limit to dstack
  
  pCurrentWord->vEncode("jumpTrue"_Stz,pDoLimitTest+1);
  
  pCurrentWord->vEncode("rdrop"_Stz);
  pCurrentWord->vEncode("rdrop"_Stz);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* then -- resolve jump to unknown forward address                  immediate */
/*----------------------------------------------------------------------------*/
void Step::vEncodeThen(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vEncodeThen"_Stz);  // ( orig -- / -- )
  
  Word* pCurrentWord = mpTask->m_pCompiler->m_pCurrent;
  
  // cstack: pop unknown forward address
  Cell* pUnknownOrigin;
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(pUnknownOrigin) )
    vThrow(__func__,__FILE__,__LINE__,FK_DSTACK,__STEP__);
  
  // resolve forward branch to this point in routine
  pUnknownOrigin->m_aJumpTo = pCurrentWord->aLastCell();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* until -- encode jump false to known backward address             immediate */
/*----------------------------------------------------------------------------*/
void Step::vEncodeUntil(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vEncodeUntil"_Stz); // ( dest -- / flag -- )
  
  Word* pCurrentWord = mpTask->m_pCompiler->m_pCurrent;
  
  // cstack: pop known backward address
  Cell* pKnownDestination;
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(pKnownDestination) )
    vThrow(__func__,__FILE__,__LINE__,FK_DSTACK,__STEP__);
  
  // append : jump on false to known backward address
  pCurrentWord->vEncode("jumpFalse"_Stz,pKnownDestination);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
