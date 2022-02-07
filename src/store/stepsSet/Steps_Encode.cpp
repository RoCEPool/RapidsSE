/*============================================================================*/
/* Steps_Encode.cpp - encode step group                                       */
/*============================================================================*/

#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "Steps.hpp"


/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Encode(void)
{
  vAddStep("again"_Stz,         &Step::vEncodeAgain,         true );
  vAddStep("ahead"_Stz,         &Step::vEncodeAhead,         true );
  vAddStep("continue"_Stz,      &Step::vEncodeContinue,      true );
  vAddStep("continue_if"_Stz,   &Step::vEncodeContinue_if,   true );
  vAddStep("here"_Stz,          &Step::vEncodeHere);
  vAddStep("if"_Stz,            &Step::vEncodeIf,            true );
  vAddStep("loop"_Stz,          &Step::vEncodeLoop,          true );
  vAddStep("+loop"_Stz,         &Step::vEncodeLoopPlus,      true );
  vAddStep("next"_Stz,          &Step::vEncodeNext,          true );
  vAddStep("next+"_Stz,         &Step::vEncodeNextPlus,      true );
  vAddStep("then"_Stz,          &Step::vEncodeThen,          true );
  vAddStep("until"_Stz,         &Step::vEncodeUntil,         true );
  
  vAddStep("encode::call"_Stz,  &Step::vEncodeCurrentCall);
  vAddStep("encode::push"_Stz,  &Step::vEncodeCurrentPush);
  
  vAddStep("encode.call"_Stz,   &Step::vEncodeCall);
  vAddStep("encode.push"_Stz,   &Step::vEncodePush);
}

/*----------------------------------------------------------------------------*/
/* encode::call -- encode call into current word                              */
/*----------------------------------------------------------------------------*/
void Step::vEncodeCurrentCall(void)
{
  ENTRY();
  
  Word* pCurrentWord =  m_pSenCompiler->pCurrentWord();
  
  Word* pCalledWord = _pWord(m_pDataStack->pPopPtr(__func__));

  fpSTEPFN* afpCalledEntryStep = pCalledWord->afpEntryStep();
  
  pCurrentWord->vEncodeCall(afpCalledEntryStep);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* encode::push -- encode push of value into current word                     */
/*----------------------------------------------------------------------------*/
void Step::vEncodeCurrentPush(void)
{
  ENTRY();
  
  Word* pCurrentWord =  m_pSenCompiler->pCurrentWord();
  
  CellSingle xVal;
  m_pDataStack->vPopItem(xVal,__func__);

  pCurrentWord->vEncodePush(xVal);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* encode.call -- encode call into given word                                 */
/*----------------------------------------------------------------------------*/
void Step::vEncodeCall(void)
{
  ENTRY();
  
  Word* pTargetWord = _pWord(m_pDataStack->pPopPtr(__func__));
  
  Word* pCalledWord = _pWord(m_pDataStack->pPopPtr(__func__));

  fpSTEPFN* afpCalledEntryStep = pCalledWord->afpEntryStep();
  
  pTargetWord->vEncodeCall(afpCalledEntryStep);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* encode.push -- encode push of value into given word                        */
/*----------------------------------------------------------------------------*/
void Step::vEncodePush(void)
{
  ENTRY();
  
  Word* pTargetWord = _pWord(m_pDataStack->pPopPtr(__func__));

  CellSingle xVal;
  m_pDataStack->vPopItem(xVal,__func__);

  pTargetWord->vEncodePush(xVal);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* again -- encode jump to known backward address                   immediate */
/*----------------------------------------------------------------------------*/
void Step::vEncodeAgain(void)
{
  ENTRY();
  
  Word* pCurrentWord =  m_pSenCompiler->pCurrentWord();
  
  // cstack: pop known backward address
  CodeCell* aKnownDestination = m_pDataStack->aPopFip(__func__);
  
  // append : jump to known backward address
  pCurrentWord->vEncodeJump("jump"_Stz,aKnownDestination);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* ahead -- encode jump to unknown forward address                  immediate */
/*----------------------------------------------------------------------------*/
void Step::vEncodeAhead(void)
{
  ENTRY();
  
  Word* pCurrentWord =  m_pSenCompiler->pCurrentWord();
  
  // append : jump to unknown forward address
  pCurrentWord->vEncodeCall("jump"_Stz);
  CodeCell* aUnknownOrigin = (CodeCell*)pCurrentWord->aEncodeArg((CodeCell*)nullptr);
  
  // cstack: push unknown forward address
  m_pDataStack->vPushFip(aUnknownOrigin);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* encode -- encode jump on true to known backward address          immediate */
/*----------------------------------------------------------------------------*/
void Step::vEncodeContinue(void)
{
  ENTRY();
  
  Word* pCurrentWord =  m_pSenCompiler->pCurrentWord();
  
  // cstack: pop known backward address
  CodeCell* aKnownBackward = m_pDataStack->aPopFip(__func__);
  
  // append : jump on true to known backward address
  pCurrentWord->vEncodeJump("jumpTrue"_Stz,aKnownBackward);
  
  // cstack: push known backward address
  m_pDataStack->vPushFip(aKnownBackward);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* continue_if -- encode jump to known backward address (inside if) immediate */
/*----------------------------------------------------------------------------*/
void Step::vEncodeContinue_if(void)
{
  ENTRY();
  
  Word* pCurrentWord =  m_pSenCompiler->pCurrentWord();
  
  // cstack: pop known if address
  CodeCell* aIfDestination = m_pDataStack->aPopFip(__func__);
  
  // cstack: pop known backward address
  CodeCell* aKnownDestination = m_pDataStack->aPopFip(__func__);
  
  // append : jump to known backward address
  pCurrentWord->vEncodeJump("jump"_Stz,aKnownDestination);
  
  // cstack: push known forward address
  m_pDataStack->vPushFip(aKnownDestination);
  
  // cstack: push known forward address
  m_pDataStack->vPushFip(aIfDestination);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* here -- push address of last cell of word                        immediate */
/*----------------------------------------------------------------------------*/
void Step::vEncodeHere(void)
{
  ENTRY();
  
  Word* pCurrentWord =  m_pSenCompiler->pCurrentWord();
  
  CodeCell* aLastCell = pCurrentWord->aLastCell();
  
  // cstack: push address of last cell
  m_pDataStack->vPushFip(aLastCell);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* if -- encode jump on false to unknown forward address            immediate */
/*----------------------------------------------------------------------------*/
void Step::vEncodeIf(void)
{
  ENTRY();
  
  Word* pCurrentWord =  m_pSenCompiler->pCurrentWord();
  
  // append : jumpFalse to unknown forward address
  pCurrentWord->vEncodeCall("jumpFalse"_Stz);
  CodeCell* aUnknownOrigin = (CodeCell*)pCurrentWord->aEncodeArg((CodeCell*)nullptr);
  
  // cstack: push unknown forward address
  m_pDataStack->vPushFip(aUnknownOrigin);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* loop -- encode runtime for : LOOP                                immediate */
/*----------------------------------------------------------------------------*/
void Step::vEncodeLoop(void)
{
  ENTRY();
  
  Word* pCurrentWord =  m_pSenCompiler->pCurrentWord();
  
  // increment index:
  pCurrentWord->vEncodeCall("r>"_Stz);
  pCurrentWord->vEncodeCall("1+"_Stz);
  pCurrentWord->vEncodeCall(">r"_Stz);
  
  i8 iIsDo = m_pDataStack->iPopBol(__func__);
  
  CodeCell* aDoLimitTest = nullptr;
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
    CodeCell* aDoHere = m_pDataStack->aPopFip(__func__);
    pCurrentWord->vEncodeJump("jumpTrue"_Stz,aDoHere);
  }
  else {
    pCurrentWord->vEncodeJump("jumpTrue"_Stz,aDoLimitTest+1);
  }
  
  pCurrentWord->vEncodeCall("rdiscard"_Stz);
  pCurrentWord->vEncodeCall("rdiscard"_Stz);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* loopPlus -- encode runtime for : +LOOP                           immediate */
/*----------------------------------------------------------------------------*/
void Step::vEncodeLoopPlus(void)
{
  ENTRY();
  
  Word* pCurrentWord =  m_pSenCompiler->pCurrentWord();
  
  // increment index:
  pCurrentWord->vEncodeCall("r>"_Stz);
  pCurrentWord->vEncodeCall("+"_Stz);
  pCurrentWord->vEncodeCall(">r"_Stz);
  
  i8 iIsDo = m_pDataStack->iPopBol(__func__);
  
  CodeCell* aDoLimitTest = nullptr;
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
    CodeCell* aDoHere = m_pDataStack->aPopFip(__func__);
    pCurrentWord->vEncodeJump("jumpTrue"_Stz,aDoHere);
  }
  else {
    pCurrentWord->vEncodeJump("jumpTrue"_Stz,aDoLimitTest+1);
  }
  
  pCurrentWord->vEncodeCall("rdiscard"_Stz);
  pCurrentWord->vEncodeCall("rdiscard"_Stz);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* next -- encode runtime for : NEXT                                immediate */
/*----------------------------------------------------------------------------*/
void Step::vEncodeNext(void)
{
  ENTRY();
  
  Word* pCurrentWord =  m_pSenCompiler->pCurrentWord();
  
  // cstack: pop destination address of backward jump
  CodeCell* aDoLimitTest = m_pDataStack->aPopFip(__func__);
  aDoLimitTest->m_aJumpTo = pCurrentWord->aLastCell();
  
  pCurrentWord->vEncodeCall("r@"_Stz); // copy index limit to dstack
  
  pCurrentWord->vEncodeJump("jumpTrue"_Stz,aDoLimitTest+1);
  
  pCurrentWord->vEncodeCall("rdiscard"_Stz);
  pCurrentWord->vEncodeCall("rdiscard"_Stz);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* next -- encode runtime for : NEXT                                immediate */
/*----------------------------------------------------------------------------*/
void Step::vEncodeNextPlus(void)
{
  ENTRY();
  
  Word* pCurrentWord =  m_pSenCompiler->pCurrentWord();
  
  // cstack: pop destination address of backward jump
  CodeCell* aDoLimitTest = m_pDataStack->aPopFip(__func__);
  aDoLimitTest->m_aJumpTo = pCurrentWord->aLastCell();
  
  i8 iPlus = m_pDataStack->iPopInt(__func__);
  
  pCurrentWord->vEncodeCall("r@"_Stz); // copy index limit to dstack
  
  pCurrentWord->vEncodeJump("jumpTrue"_Stz,aDoLimitTest+iPlus);
  
  pCurrentWord->vEncodeCall("rdiscard"_Stz);
  pCurrentWord->vEncodeCall("rdiscard"_Stz);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* then -- resolve jump to unknown forward address                  immediate */
/*----------------------------------------------------------------------------*/
void Step::vEncodeThen(void)
{
  ENTRY();
  
  Word* pCurrentWord =  m_pSenCompiler->pCurrentWord();
  
  // cstack: pop unknown forward address
  CodeCell* aUnknownOrigin = m_pDataStack->aPopFip(__func__);
  
  // resolve forward branch to this point in routine
  aUnknownOrigin->m_aJumpTo = pCurrentWord->aLastCell();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* until -- encode jump false to known backward address             immediate */
/*----------------------------------------------------------------------------*/
void Step::vEncodeUntil(void)
{
  ENTRY();
  
  Word* pCurrentWord =  m_pSenCompiler->pCurrentWord();
  
  // cstack: pop known backward address
  CodeCell* aKnownDestination = m_pDataStack->aPopFip(__func__);
  
  // append : jump on false to known backward address
  pCurrentWord->vEncodeJump("jumpFalse"_Stz,aKnownDestination);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
