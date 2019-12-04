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
  
  vAddStep("encode::call"_Stz,        &Step::vEncodeCall);
  vAddStep("encode::push"_Stz,        &Step::vEncodePush);
}

/*----------------------------------------------------------------------------*/
/* again -- encode jump to known backward address                   immediate */
/*----------------------------------------------------------------------------*/
void Step::vEncodeAgain(void)
{
  ENTRY(); // ENTRY("vEncodeAgain"_Stz); // ( dest -- / -- )
  
  Word* pCurrentWord = m_pCompiler->pCurrentWord();
  
  // cstack: pop known backward address
  Cell* aKnownDestination = m_pDataStack->aPopFip(__func__);
  
  // append : jump to known backward address
  pCurrentWord->vEncodeJump("jump"_Stz,aKnownDestination);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* ahead -- encode jump to unknown forward address                  immediate */
/*----------------------------------------------------------------------------*/
void Step::vEncodeAhead(void)
{
  ENTRY(); // ENTRY("vEncodeAhead"_Stz);// ( -- orig / -- )
  
  Word* pCurrentWord = m_pCompiler->pCurrentWord();
  
  // append : jump to unknown forward address
  pCurrentWord->vEncodeCall("jump"_Stz);
  Cell* aUnknownOrigin = (Cell*)pCurrentWord->aAppend((Cell*)nullptr);
  
  // cstack: push unknown forward address
  m_pDataStack->vPushFip(aUnknownOrigin);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* encode::call -- encode call                                                */
/*----------------------------------------------------------------------------*/
void Step::vEncodeCall(void)
{
  ENTRY(); // ENTRY("vEncodeCall"_Stz);
  
  Word* pCurrentWord = m_pCompiler->pCurrentWord();
  
  Word* pCalledWord = _pWord(m_pDataStack->pPopPtr(__func__));

  StepFp* afpCalledEntryStep = pCalledWord->afpEntryStep();
  
  pCurrentWord->vEncodeCall(afpCalledEntryStep);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* encode -- encode jump on true to known backward address          immediate */
/*----------------------------------------------------------------------------*/
void Step::vEncodeContinue(void)
{
  ENTRY(); // ENTRY("vEncodeContinue"_Stz); // ( -- orig / flag -- )
  
  Word* pCurrentWord = m_pCompiler->pCurrentWord();
  
  // cstack: pop known backward address
  Cell* aKnownBackward = m_pDataStack->aPopFip(__func__);
  
  // append : jump on true to known backward address
  pCurrentWord->vEncodeJump("jumpTrue"_Stz,aKnownBackward);
  
  // cstack: push known backward address
  m_pDataStack->vPushFip(aKnownBackward);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* here -- push address of last cell of word                        immediate */
/*----------------------------------------------------------------------------*/
void Step::vEncodeHere(void)
{
  ENTRY(); // ENTRY("vEncodeHere"_Stz);
  
  Word* pCurrentWord = m_pCompiler->pCurrentWord();
  
  Cell* aLastCell = pCurrentWord->aLastCell();
  
  // cstack: push address of last cell
  m_pDataStack->vPushFip(aLastCell);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* if -- encode jump on false to unknown forward address            immediate */
/*----------------------------------------------------------------------------*/
void Step::vEncodeIf(void)
{
  ENTRY(); // ENTRY("vEncodeIf"_Stz); // ( -- orig / flag -- )
  
  Word* pCurrentWord = m_pCompiler->pCurrentWord();
  
  // append : jumpFalse to unknown forward address
  pCurrentWord->vEncodeCall("jumpFalse"_Stz);
  Cell* aUnknownOrigin = (Cell*)pCurrentWord->aAppend((Cell*)nullptr);
  
  // cstack: push unknown forward address
  m_pDataStack->vPushFip(aUnknownOrigin);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* ifAgain -- encode jump to known backward address (inside if)     immediate */
/*----------------------------------------------------------------------------*/
void Step::vEncodeIfAgain(void)
{
  ENTRY(); // ENTRY("vEncodeIfAgain"_Stz); // ( dest -- / -- )
  
  Word* pCurrentWord = m_pCompiler->pCurrentWord();
  
  // cstack: pop known if address
  Cell* aIfDestination = m_pDataStack->aPopFip(__func__);
  
  // cstack: pop known backward address
  Cell* aKnownDestination = m_pDataStack->aPopFip(__func__);
  
  // append : jump to known backward address
  pCurrentWord->vEncodeJump("jump"_Stz,aKnownDestination);
  
  // cstack: push known forward address
  m_pDataStack->vPushFip(aKnownDestination);
  
  // cstack: push known forward address
  m_pDataStack->vPushFip(aIfDestination);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* loop -- encode runtime for : LOOP                                immediate */
/*----------------------------------------------------------------------------*/
void Step::vEncodeLoop(void)
{
  ENTRY(); // ENTRY("vEncodeLoop"_Stz);
  
  Word* pCurrentWord = m_pCompiler->pCurrentWord();
  
  // increment index:
  pCurrentWord->vEncodeCall("r>"_Stz);
  pCurrentWord->vEncodeCall("1+"_Stz);
  pCurrentWord->vEncodeCall(">r"_Stz);
  
  i8 iIsDo = m_pDataStack->iPopInt(__func__);
  
  Cell* aDoLimitTest = nullptr;
  if (! iIsDo) {
    // cstack: pop destination address of backward jump
    aDoLimitTest = m_pDataStack->aPopFip(__func__);
    aDoLimitTest->m_aJumpTo = pCurrentWord->aLastCell();
  }
  
  pCurrentWord->vEncodeCall("2r@"_Stz); // copy index limit to dstack
  pCurrentWord->vEncodeCall("!="_Stz);   // test for limit > index
  
  if (iIsDo) {
    // jump on true to doHere:
    // cstack: pop destination address of backward jump
    Cell* aDoHere = m_pDataStack->aPopFip(__func__);
    pCurrentWord->vEncodeJump("jumpTrue"_Stz,aDoHere);
  }
  else {
    pCurrentWord->vEncodeJump("jumpTrue"_Stz,aDoLimitTest+1);
  }
  
  pCurrentWord->vEncodeCall("rdrop"_Stz);
  pCurrentWord->vEncodeCall("rdrop"_Stz);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* loopPlus -- encode runtime for : +LOOP                           immediate */
/*----------------------------------------------------------------------------*/
void Step::vEncodeLoopPlus(void)
{
  ENTRY(); // ENTRY("vEncodeLoopPlus"_Stz);
  
  Word* pCurrentWord = m_pCompiler->pCurrentWord();
  
  // increment index:
  pCurrentWord->vEncodeCall("r>"_Stz);
  pCurrentWord->vEncodeCall("+"_Stz);
  pCurrentWord->vEncodeCall(">r"_Stz);
  
  i8 iIsDo = m_pDataStack->iPopInt(__func__);
  
  Cell* aDoLimitTest = nullptr;
  if (! iIsDo) {
    // cstack: pop destination address of backward jump
    aDoLimitTest = m_pDataStack->aPopFip(__func__);
    aDoLimitTest->m_aJumpTo = pCurrentWord->aLastCell();
  }
  
  pCurrentWord->vEncodeCall("2r@"_Stz); // copy index limit to dstack
  pCurrentWord->vEncodeCall("!="_Stz);   // test for limit > index
  
  if (iIsDo) {
    // jump on true to doHere:
    // cstack: pop destination address of backward jump
    Cell* aDoHere = m_pDataStack->aPopFip(__func__);
    pCurrentWord->vEncodeJump("jumpTrue"_Stz,aDoHere);
  }
  else {
    pCurrentWord->vEncodeJump("jumpTrue"_Stz,aDoLimitTest+1);
  }
  
  pCurrentWord->vEncodeCall("rdrop"_Stz);
  pCurrentWord->vEncodeCall("rdrop"_Stz);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* next -- encode runtime for : NEXT                                immediate */
/*----------------------------------------------------------------------------*/
void Step::vEncodeNext(void)
{
  ENTRY(); // ENTRY("vEncodeNext"_Stz);
  
  Word* pCurrentWord = m_pCompiler->pCurrentWord();
  
  // cstack: pop destination address of backward jump
  Cell* aDoLimitTest = m_pDataStack->aPopFip(__func__);
  aDoLimitTest->m_aJumpTo = pCurrentWord->aLastCell();
  
  pCurrentWord->vEncodeCall("r@"_Stz); // copy index limit to dstack
  
  pCurrentWord->vEncodeJump("jumpTrue"_Stz,aDoLimitTest+1);
  
  pCurrentWord->vEncodeCall("rdrop"_Stz);
  pCurrentWord->vEncodeCall("rdrop"_Stz);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* encode::push -- encode push of value                                       */
/*----------------------------------------------------------------------------*/
void Step::vEncodePush(void)
{
  ENTRY(); // ENTRY("vEncodePush"_Stz);
  
  Word* pCurrentWord = m_pCompiler->pCurrentWord();
  
  GenVar xVal;
  m_pDataStack->vPopVal(xVal,__func__);

  pCurrentWord->vEncodePush(xVal);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* then -- resolve jump to unknown forward address                  immediate */
/*----------------------------------------------------------------------------*/
void Step::vEncodeThen(void)
{
  ENTRY(); // ENTRY("vEncodeThen"_Stz);  // ( orig -- / -- )
  
  Word* pCurrentWord = m_pCompiler->pCurrentWord();
  
  // cstack: pop unknown forward address
  Cell* aUnknownOrigin = m_pDataStack->aPopFip(__func__);
  
  // resolve forward branch to this point in routine
  aUnknownOrigin->m_aJumpTo = pCurrentWord->aLastCell();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* until -- encode jump false to known backward address             immediate */
/*----------------------------------------------------------------------------*/
void Step::vEncodeUntil(void)
{
  ENTRY(); // ENTRY("vEncodeUntil"_Stz); // ( dest -- / flag -- )
  
  Word* pCurrentWord = m_pCompiler->pCurrentWord();
  
  // cstack: pop known backward address
  Cell* aKnownDestination = m_pDataStack->aPopFip(__func__);
  
  // append : jump on false to known backward address
  pCurrentWord->vEncodeJump("jumpFalse"_Stz,aKnownDestination);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
