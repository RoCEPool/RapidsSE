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
void Step::vEncodeAgain(Forth* pForth)
{
  ENTRY("vEncodeAgain"_Stz); // ( dest -- / -- )
  
  Word* pCurrentWord = pForth->m_pTask->m_pCompiler->m_pCurrent;
  
  // cstack: pop known backward address
  Cell* pKnownDestination;
  if ( ! pForth->m_pDataStack->bPop(pKnownDestination) )
    THROW_DSTACK();
  
  // append : jump to known backward address
  pCurrentWord->vEncode("jump"_Stz,pKnownDestination);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* ahead -- encode jump to unknown forward address                  immediate */
/*----------------------------------------------------------------------------*/
void Step::vEncodeAhead(Forth* pForth)
{
  ENTRY("vEncodeAhead"_Stz);// ( -- orig / -- )
  
  Word* pCurrentWord = pForth->m_pTask->m_pCompiler->m_pCurrent;
  
  // append : jump to unknown forward address
  pCurrentWord->vEncode("jump"_Stz);
  Cell* pUnknownOrigin = (Cell*)pCurrentWord->aAppend((Cell*)nullptr);
  
  // cstack: push unknown forward address
  pForth->m_pDataStack->vPushCell(pUnknownOrigin);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* encode -- encode jump on true to known backward address          immediate */
/*----------------------------------------------------------------------------*/
void Step::vEncodeContinue(Forth* pForth)
{
  ENTRY("vEncodeContinue"_Stz); // ( -- orig / flag -- )
  
  Word* pCurrentWord = pForth->m_pTask->m_pCompiler->m_pCurrent;
  
  // cstack: pop known backward address
  Cell* pKnownBackward;
  if ( ! pForth->m_pDataStack->bPop(pKnownBackward) )
    THROW_DSTACK();
  
  // append : jump on true to known backward address
  pCurrentWord->vEncode("jumpTrue"_Stz,pKnownBackward);
  
  // cstack: push known backward address
  pForth->m_pDataStack->vPushCell(pKnownBackward);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* here -- push address of last cell of word                        immediate */
/*----------------------------------------------------------------------------*/
void Step::vEncodeHere(Forth* pForth)
{
  ENTRY("vEncodeHere"_Stz);
  
  Word* pCurrentWord = pForth->m_pTask->m_pCompiler->m_pCurrent;
  
  Cell* pLastCell = pCurrentWord->aLastCell();
  
  // cstack: push address of last cell
  pForth->m_pDataStack->vPushCell(pLastCell);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* if -- encode jump on false to unknown forward address            immediate */
/*----------------------------------------------------------------------------*/
void Step::vEncodeIf(Forth* pForth)
{
  ENTRY("vEncodeIf"_Stz); // ( -- orig / flag -- )
  
  Word* pCurrentWord = pForth->m_pTask->m_pCompiler->m_pCurrent;
  
  // append : jumpFalse to unknown forward address
  pCurrentWord->vEncode("jumpFalse"_Stz);
  Cell* pUnknownOrigin = (Cell*)pCurrentWord->aAppend((Cell*)nullptr);
  
  // cstack: push unknown forward address
  pForth->m_pDataStack->vPushCell(pUnknownOrigin);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* ifAgain -- encode jump to known backward address (inside if)     immediate */
/*----------------------------------------------------------------------------*/
void Step::vEncodeIfAgain(Forth* pForth)
{
  ENTRY("vEncodeIfAgain"_Stz); // ( dest -- / -- )
  
  Word* pCurrentWord = pForth->m_pTask->m_pCompiler->m_pCurrent;
  
  // cstack: pop known if address
  Cell* pIfDestination;
  if ( ! pForth->m_pDataStack->bPop(pIfDestination) )
    THROW_DSTACK();
  
  // cstack: pop known backward address
  Cell* pKnownDestination;
  if ( ! pForth->m_pDataStack->bPop(pKnownDestination) )
    THROW_DSTACK();
  
  // append : jump to known backward address
  pCurrentWord->vEncode("jump"_Stz,pKnownDestination);
  
  // cstack: push known forward address
  pForth->m_pDataStack->vPushCell(pKnownDestination);
  
  // cstack: push known forward address
  pForth->m_pDataStack->vPushCell(pIfDestination);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* loop -- encode runtime for : LOOP                                immediate */
/*----------------------------------------------------------------------------*/
void Step::vEncodeLoop(Forth* pForth)
{
  ENTRY("vEncodeLoop"_Stz);
  
  Word* pCurrentWord = pForth->m_pTask->m_pCompiler->m_pCurrent;
  
  // increment index:
  pCurrentWord->vEncode("r>"_Stz);
  pCurrentWord->vEncode("1+"_Stz);
  pCurrentWord->vEncode(">r"_Stz);
  
  i8 iIsDo;
  // cstack: is do ?
  if ( ! pForth->m_pDataStack->bPop(iIsDo) )
    THROW_DSTACK();
  
  Cell* pDoLimitTest = nullptr;
  if (! iIsDo) {
    // cstack: pop destination address of backward jump
    if ( ! pForth->m_pDataStack->bPop(pDoLimitTest) )
      THROW_DSTACK();
    pDoLimitTest->m_aJumpTo = pCurrentWord->aLastCell();
  }
  
  pCurrentWord->vEncode("2r@"_Stz); // copy index limit to dstack
  pCurrentWord->vEncode("!="_Stz);   // test for limit > index
  
  if (iIsDo) {
    // jump on true to doHere:
    Cell* pDoHere;
    // cstack: pop destination address of backward jump
    if ( ! pForth->m_pDataStack->bPop(pDoHere) )
      THROW_DSTACK();
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
void Step::vEncodeLoopPlus(Forth* pForth)
{
  ENTRY("vEncodeLoopPlus"_Stz);
  
  Word* pCurrentWord = pForth->m_pTask->m_pCompiler->m_pCurrent;
  
  // increment index:
  pCurrentWord->vEncode("r>"_Stz);
  pCurrentWord->vEncode("+"_Stz);
  pCurrentWord->vEncode(">r"_Stz);
  
  i8 iIsDo;
  // cstack: is do ?
  if ( ! pForth->m_pDataStack->bPop(iIsDo) )
    THROW_DSTACK();
  
  Cell* pDoLimitTest = nullptr;
  if (! iIsDo) {
    // cstack: pop destination address of backward jump
    if ( ! pForth->m_pDataStack->bPop(pDoLimitTest) )
      THROW_DSTACK();
    pDoLimitTest->m_aJumpTo = pCurrentWord->aLastCell();
  }
  
  pCurrentWord->vEncode("2r@"_Stz); // copy index limit to dstack
  pCurrentWord->vEncode("!="_Stz);   // test for limit > index
  
  if (iIsDo) {
    // jump on true to doHere:
    Cell* pDoHere;
    // cstack: pop destination address of backward jump
    if ( ! pForth->m_pDataStack->bPop(pDoHere) )
      THROW_DSTACK();
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
void Step::vEncodeNext(Forth* pForth)
{
  ENTRY("vEncodeNext"_Stz);
  
  Word* pCurrentWord = pForth->m_pTask->m_pCompiler->m_pCurrent;
  
  Cell* pDoLimitTest = nullptr;
  // cstack: pop destination address of backward jump
  if ( ! pForth->m_pDataStack->bPop(pDoLimitTest) )
    THROW_DSTACK();
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
void Step::vEncodeThen(Forth* pForth)
{
  ENTRY("vEncodeThen"_Stz);  // ( orig -- / -- )
  
  Word* pCurrentWord = pForth->m_pTask->m_pCompiler->m_pCurrent;
  
  // cstack: pop unknown forward address
  Cell* pUnknownOrigin;
  if ( ! pForth->m_pDataStack->bPop(pUnknownOrigin) )
    THROW_DSTACK();
  
  // resolve forward branch to this point in routine
  pUnknownOrigin->m_aJumpTo = pCurrentWord->aLastCell();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* until -- encode jump false to known backward address             immediate */
/*----------------------------------------------------------------------------*/
void Step::vEncodeUntil(Forth* pForth)
{
  ENTRY("vEncodeUntil"_Stz); // ( dest -- / flag -- )
  
  Word* pCurrentWord = pForth->m_pTask->m_pCompiler->m_pCurrent;
  
  // cstack: pop known backward address
  Cell* pKnownDestination;
  if ( ! pForth->m_pDataStack->bPop(pKnownDestination) )
    THROW_DSTACK();
  
  // append : jump on false to known backward address
  pCurrentWord->vEncode("jumpFalse"_Stz,pKnownDestination);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
