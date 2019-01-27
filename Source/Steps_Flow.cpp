/*============================================================================*/
/* Steps_Flow.cpp - flow step group                                           */
/*============================================================================*/

#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

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
void Step::vFlowBegin_(MP_Task mpTask)
{
  ENTRY("vBegin_"_Stz);
  
  mpTask->m_pCompiler->m_pBeginIP = mpTask->FIP.m_aCell;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* execute -- execute word on data stack                                      */
/*----------------------------------------------------------------------------*/
void Step::vFlowExecute(MP_Task mpTask)
{
  ENTRY("vFlowExecute"_Stz);
  
  Object* pWordObject;
  if ( ! mpTask->m_pDataStack->bPop(pWordObject) )
    THROW_INVALID_DSTACK("execute"_Stz);
  Word* pWord = _pWord(pWordObject);
  
  if (WT_PRIMITIVE == pWord->eWordType()) {
    StepFp fpStepFp = pWord->fpEntryStep();
    (fpStepFp)(mpTask);
  }
  else if (WT_OBJECT == pWord->eWordType()) {
    mpTask->m_pReturnStack->vPushCell(mpTask->FIP.m_aCell);
    mpTask->FIP.m_aCell = pWord->aEntryCell();
    
    Object* pObject = (Object*)pWord->aFirstCell();
    mpTask->m_pDataStack->vPushPointer(pObject);
  }
  else if (WT_ROUTINE == pWord->eWordType()){
    mpTask->m_pReturnStack->vPushCell(mpTask->FIP.m_aCell);
    mpTask->FIP.m_aCell = pWord->aEntryCell();
  }
  else {
    NEVER_REACH;
  }
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* hazard -- should never be called                                           */
/*----------------------------------------------------------------------------*/
void Step::vFlowHazard(MP_Task mpTask)
{
  ENTRY("vFlowHazard"_Stz);
  
  THROW("Entered hazard"_Stz);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* jump -- jump to location                                                   */
/*----------------------------------------------------------------------------*/
void Step::vFlowJump(MP_Task mpTask)
{
  ENTRY("vJump"_Stz);
  
  mpTask->FIP.m_aCell = mpTask->FIP.m_aCell->m_aJumpTo;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* jumpBegin_ -- jump to begin_ (used only in : docol)              immediate */
/*----------------------------------------------------------------------------*/
void Step::vFlowJumpBegin_(MP_Task mpTask)
{
  ENTRY("vFlowJumpBegin_"_Stz);
  
  mpTask->FIP.m_aCell = mpTask->m_pCompiler->m_pBeginIP;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* jumpFalse -- jump on false to location                                     */
/*----------------------------------------------------------------------------*/
#define JUMP_FALSE
#ifdef JUMP_FALSE
void Step::vFlowJumpFalse(MP_Task mpTask)
{
  ENTRY("vJumpFalse"_Stz);
  
  //if ( ! bGuard(1) )
  //  THROW_INVALID_DSTACK("jumpFalse"_Stz);
  
  // get pointer to value on the top of the stack
  Stack* pDataStack = mpTask->m_pDataStack;
  Value* aTopValue = reinterpret_cast<Value*>(pDataStack->m_aTop);

  eCN eClassNo = aTopValue->eClass(); // inline
  
  i8 iVal;
  if (CN_INTVAL == eClassNo) {
    iVal = _aIntVal(aTopValue)->iInt(); // inline
  }
  else
  if (CN_PTRVAL == eClassNo) {
    iVal = _aPtrVal(aTopValue)->pObj() ? 1 : 0; // inline
  }
  else {
    iVal = -1;
    NEVER_REACH;
  }
  
  pDataStack->vPopBy(sizeof(IntVal)); // inline
  pDataStack->m_nValues--;
  
  // n.b. the IP points to the jump arg
  
  if (iVal)
    // do not jump, skip over argument
    mpTask->FIP.m_aCell++;
  else
    // jump to location
    mpTask->FIP.m_aCell = mpTask->FIP.m_aCell->m_aJumpTo;
  
  EXIT();
}

#else //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void Step::vFlowJumpFalse(MP_Task mpTask)
{
  ENTRY("vJumpFalse"_Stz);
  
  i8 iVal;
  if ( ! mpTask->m_pDataStack->bPopAsInt_Quick(iVal) )
    THROW_INVALID_DSTACK("jumpFalse"_Stz);
  
  // n.b. the IP points to the jump arg
  
  if (iVal)
    // do not jump, skip over argument
    mpTask->FIP.m_aCell++;
  else
    // jump to location
    mpTask->FIP.m_aCell = mpTask->FIP.m_aCell->m_aJumpTo;
  
  EXIT();
}
#endif

/*----------------------------------------------------------------------------*/
/* jumpTrue -- jump on true to location                                       */
/*----------------------------------------------------------------------------*/
void Step::vFlowJumpTrue(MP_Task mpTask)
{
  ENTRY("vJumpTrue"_Stz);
  
  i8 iVal;
  if ( ! mpTask->m_pDataStack->bPopAsInt(iVal) )
    THROW_INVALID_DSTACK("jumpTrue"_Stz);
  
  // n.b. the IP points to the jump arg
  
  if (iVal)
    // jump to location
    mpTask->FIP.m_aCell = mpTask->FIP.m_aCell->m_aJumpTo;
  else
    // do not jump, skip over argument
    mpTask->FIP.m_aCell++;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* nop -- no op                                                               */
/*----------------------------------------------------------------------------*/
void Step::vFlowNop(MP_Task mpTask)
{
  ENTRY("vFlowNop"_Stz);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* retryPoint -- mark retry point                                             */
/*----------------------------------------------------------------------------*/
void Step::vFlowRetryPoint(MP_Task mpTask)
{
  ENTRY("vFlowRetryPoint"_Stz);
  
  mpTask->m_pCompiler->m_pRetryIP = mpTask->FIP.m_aCell;
  
  mpTask->m_pDataStack->vRetryPoint();
  
  mpTask->m_pReturnStack->vRetryPoint();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* test -- print test                                                         */
/*----------------------------------------------------------------------------*/
void Step::vFlowTest(MP_Task mpTask)
{
  ENTRY("vFlowTest"_Stz);
  
  vTrace("[%s] Test\n"_Stz,mpTask->aJobName());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* throw -- throw c++ exception (for testing purposes)                        */
/*----------------------------------------------------------------------------*/
void Step::vFlowThrow(MP_Task mpTask)
{
  ENTRY("vFlowThrow"_Stz);
  
  char caException[256];
  
  StrLib::vSprint(caException,"THROW TEST"_Stz);
  
  throw caException;
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
