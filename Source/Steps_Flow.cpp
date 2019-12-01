/*============================================================================*/
/* Steps_Flow.cpp - flow step group                                           */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Flow(void)
{
  vAddStep("begin_"_Stz,          &Step::vFlowBegin_);
  vAddStep("execute"_Stz,         &Step::vFlowExecute);
  vAddStep("hazard"_Stz,          &Step::vFlowHazard);
  vAddStep("jump"_Stz,            &Step::vFlowJump);
  vAddStep("jumpBegin_"_Stz,      &Step::vFlowJumpBegin_);
  vAddStep("jumpFalse"_Stz,       &Step::vFlowJumpFalse);
  vAddStep("jumpTrue"_Stz,        &Step::vFlowJumpTrue);
  vAddStep("nop"_Stz,             &Step::vFlowNop);
  vAddStep("retryPoint"_Stz,      &Step::vFlowRetryPoint);
  vAddStep("test"_Stz,            &Step::vFlowTest);
  vAddStep("throw"_Stz,           &Step::vFlowThrow);
}

/*----------------------------------------------------------------------------*/
/* begin_ -- begin of the docol loop : docol (used only in docol)   immediate */
/*----------------------------------------------------------------------------*/
void Step::vFlowBegin_(void)
{
  ENTRY(); // ENTRY("vBegin_"_Stz);
  
  m_pCompiler->m_pBeginIP = m_aInxPtr;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* execute -- execute word on data stack                                      */
/*----------------------------------------------------------------------------*/
void Step::vFlowExecute(void)
{
  ENTRY(); // ENTRY("vFlowExecute"_Stz);
  
  Object* pWordObject;
  m_pDataStack->vPopPtr(pWordObject,__func__);
  Word* pWord = _pWord(pWordObject);
  
  if (WK_INTRINSIC == pWord->eKind()) {
    StepFp fpStep = pWord->fpEntryStep();
    ((Step*)this->*fpStep)();
  }
  else if (WK_OBJECT == pWord->eKind()) {
    m_pReturnStack->vPushFip(m_aInxPtr);
    m_aInxPtr = pWord->aEntryCell();
    
    Object* pObject = (Object*)pWord->aFirstCell();
    m_pDataStack->vPushPtr(pObject);
  }
  else if (WK_ROUTINE == pWord->eKind()){
    m_pReturnStack->vPushFip(m_aInxPtr);
    m_aInxPtr = pWord->aEntryCell();
  }
  else {
    NEVER_REACH;
  }
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* hazard -- should never be called                                           */
/*----------------------------------------------------------------------------*/
void Step::vFlowHazard(void)
{
  ENTRY(); // ENTRY("vFlowHazard"_Stz);
  
  vThrow(FK_CHECK,VL_FATAL,__func__,__FILE__,__LINE__,"Entered hazard"_Stz);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* jump -- jump to location                                                   */
/*----------------------------------------------------------------------------*/
void Step::vFlowJump(void)
{
  ENTRY(); // ENTRY("vJump"_Stz);
  
  m_aInxPtr = m_aInxPtr->m_aJumpTo;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* jumpBegin_ -- jump to begin_ (used only in : docol)              immediate */
/*----------------------------------------------------------------------------*/
void Step::vFlowJumpBegin_(void)
{
  ENTRY(); // ENTRY("vFlowJumpBegin_"_Stz);
  
  m_aInxPtr = m_pCompiler->m_pBeginIP;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* jumpFalse -- jump on false to location                                     */
/*----------------------------------------------------------------------------*/
void Step::vFlowJumpFalse(void)
{
  ENTRY(); // ENTRY("vJumpFalse"_Stz);
  
  i8 iVal;
  m_pDataStack->vPopAsInt(iVal,__func__);
  
  // n.b. the IP points to the jump arg
  
  if (! iVal)
    // jump to location
    m_aInxPtr = m_aInxPtr->m_aJumpTo;
  else
    // do not jump, skip over argument
    m_aInxPtr++;
  
  EXIT();
}
/*----------------------------------------------------------------------------*/
//void Step::vFlowJumpFalse(void)
//{
//  ENTRY(); // ENTRY("vJumpFalse"_Stz);
//
//  //if ( ! bGuard(1) ) THROW_DSTACK();
//
//  // get pointer to value on the top of the stack
//  Stack* pDataStack = m_pDataStack;
//  AnyVal* aAnyVal = reinterpret_cast<AnyVal*>(pDataStack->m_xTop.pAnyVal);
//
//  eCN eClassNo = aAnyVal->eClass(); // inline
//
//  i8 iVal;
//  if (CN_INTVAL == eClassNo) {
//    iVal = _aIntVal(aAnyVal)->iInt(); // inline
//  }
//  else
//  if (CN_PTRVAL == eClassNo) {
//    iVal = _aPtrVal(aAnyVal)->pObj() ? 1 : 0; // inline
//  }
//  else {
//    iVal = -1;
//    NEVER_REACH;
//  }
//
//  pDataStack->vPopBy(sizeof(IntVal)); // inline
//  pDataStack->m_nValues--;
//
//  // n.b. the IP points to the jump arg
//
//  if (iVal)
//    // do not jump, skip over argument
//    m_aInxPtr++;
//  else
//    // jump to location
//    m_aInxPtr = m_aInxPtr->m_aJumpTo;
//
//  EXIT();
//}

/*----------------------------------------------------------------------------*/
/* jumpTrue -- jump on true to location                                       */
/*----------------------------------------------------------------------------*/
void Step::vFlowJumpTrue(void)
{
  ENTRY(); // ENTRY("vJumpTrue"_Stz);
  
  i8 iVal;
  m_pDataStack->vPopAsInt(iVal,__func__);
  
  // n.b. the IP points to the jump arg
  
  if (iVal)
    // jump to location
    m_aInxPtr = m_aInxPtr->m_aJumpTo;
  else
    // do not jump, skip over argument
    m_aInxPtr++;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* nop -- no op                                                               */
/*----------------------------------------------------------------------------*/
void Step::vFlowNop(void)
{
  ENTRY(); // ENTRY("vFlowNop"_Stz);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* retryPoint -- mark retry point                                             */
/*----------------------------------------------------------------------------*/
void Step::vFlowRetryPoint(void)
{
  ENTRY(); // ENTRY("vFlowRetryPoint"_Stz);
  
  m_pCompiler->m_pRetryIP = m_aInxPtr;
  
  m_pDataStack->vRetryPoint();
  
  m_pReturnStack->vRetryPoint();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* test -- print test                                                         */
/*----------------------------------------------------------------------------*/
void Step::vFlowTest(void)
{
  ENTRY(); // ENTRY("vFlowTest"_Stz);
  
  vTrace("[%s] Test\n"_Stz,m_pTask->szJobName());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* throw -- throw c++ exception (for testing purposes)                        */
/*----------------------------------------------------------------------------*/
void Step::vFlowThrow(void)
{
  ENTRY(); // ENTRY("vFlowThrow"_Stz);
  
  s256 sException;
  
  sException.vSprint("THROW TEST"_Stz);
  
  vThrow(FK_CHECK,VL_FATAL,__func__,__FILE__,__LINE__,sException);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
