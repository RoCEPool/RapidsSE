/*============================================================================*/
/* Steps_Flow.cpp - flow step group                                           */
/*============================================================================*/

#include "StorageEngine.h"
//#include "Tasks.h"
#include "SePrompt.hpp"

extern AttachMap*      gpAttachMap;

// flow: ///////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Flow(void)
{
  vAddStep("begin_"_Stz,          &Task::vFlowBegin_);
  vAddStep("execute"_Stz,         &Task::vFlowExecute);
  vAddStep("hazard"_Stz,          &Task::vFlowHazard);
  vAddStep("jump"_Stz,            &Task::vFlowJump);
  vAddStep("jumpBegin_"_Stz,      &Task::vFlowJumpBegin_);
  vAddStep("jumpFalse"_Stz,       &Task::vFlowJumpFalse);
  vAddStep("jumpTrue"_Stz,        &Task::vFlowJumpTrue);
  vAddStep("nop"_Stz,             &Task::vFlowNop);
  vAddStep("retryPoint"_Stz,      &Task::vFlowRetryPoint);
  vAddStep("test"_Stz,            &Task::vFlowTest);
  vAddStep("throw"_Stz,           &Task::vFlowThrow);
}

/*----------------------------------------------------------------------------*/
/* begin_ -- begin of the docol loop : docol (used only in docol)   immediate */
/*----------------------------------------------------------------------------*/
void Task::vFlowBegin_(MP_Task mpTask)
{
  ENTRY("vBegin_"_Stz);
  
  mpTask->m_pBeginIP = mpTask->IP.m_pCell;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* execute -- execute word on data stack                                      */
/*----------------------------------------------------------------------------*/
void Task::vFlowExecute(MP_Task mpTask)
{
  ENTRY("vFlowExecute"_Stz);
  
  Object* pWordObject;
  if ( ! mpTask->m_pDataStack->bPop(pWordObject) )
    THROW_INVALID_DSTACK("execute"_Stz);
  mpTask->m_pWord = _pWord(pWordObject);
  
  if (WT_PRIMITIVE == mpTask->m_pWord->eWordType()) {
    TaskStep pTaskStep = mpTask->m_pWord->pEntryStep();
    (pTaskStep)(mpTask);
  }
  else if (WT_OBJECT == mpTask->m_pWord->eWordType()) {
    mpTask->m_pReturnStack->vPushCell(mpTask->IP.m_pCell);
    mpTask->IP.m_pCell = mpTask->m_pWord->pEntryCell();
    
    Object* pObject = (Object*)mpTask->m_pWord->pFirstCell();
    mpTask->m_pDataStack->vPushObject(pObject);
  }
  else if (WT_ROUTINE == mpTask->m_pWord->eWordType()){
    mpTask->m_pReturnStack->vPushCell(mpTask->IP.m_pCell);
    mpTask->IP.m_pCell = mpTask->m_pWord->pEntryCell();
  }
  else {
    NEVER_REACH;
  }
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* hazard -- should never be called                                           */
/*----------------------------------------------------------------------------*/
void Task::vFlowHazard(MP_Task mpTask)
{
  ENTRY("vFlowHazard"_Stz);
  
  THROW("Entered hazard"_Stz);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* jump -- jump to location                                                   */
/*----------------------------------------------------------------------------*/
void Task::vFlowJump(MP_Task mpTask)
{
  ENTRY("vJump"_Stz);
  
  mpTask->IP.m_pCell = mpTask->IP.m_pCell->m_pJumpTo;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* jumpBegin_ -- jump to begin_ (used only in : docol)              immediate */
/*----------------------------------------------------------------------------*/
void Task::vFlowJumpBegin_(MP_Task mpTask)
{
  ENTRY("vFlowJumpBegin_"_Stz);
  
  mpTask->IP.m_pCell = mpTask->m_pBeginIP;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* jumpFalse -- jump on false to location                                     */
/*----------------------------------------------------------------------------*/
#define JUMP_FALSE
#ifdef JUMP_FALSE
void Task::vFlowJumpFalse(MP_Task mpTask)
{
  ENTRY("vJumpFalse"_Stz);
  
  //if ( ! bGuard(1) )
  //  THROW_INVALID_DSTACK("jumpFalse"_Stz);
  
  // get pointer to value on the top of the stack
  Stack* pDataStack = mpTask->m_pDataStack;
  Value* pTop = reinterpret_cast<Value*>(pDataStack->m_pTop);

  eCN eClassNo = pTop->eClass(); // inline
  
  i8 iVal;
  if (CN_INTVAL == eClassNo) {
    iVal = _pIntVal(pTop)->iInt(); // inline
  }
  else
  if (CN_PTRVAL == eClassNo) {
    iVal = _pPtrVal(pTop)->pPtr() ? 1 : 0; // inline
  }
  else {
    iVal = -1;
    NEVER_REACH;
  }
  
  pDataStack->vPopBy(sizeof(IntVal)); // inline
  pDataStack->m_iValues--;
  
  // n.b. the IP points to the jump arg
  
  if (iVal)
    // do not jump, skip over argument
    mpTask->IP.m_pCell++;
  else
    // jump to location
    mpTask->IP.m_pCell = mpTask->IP.m_pCell->m_pJumpTo;
  
  EXIT();
}

#else //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void Task::vFlowJumpFalse(MP_Task mpTask)
{
  ENTRY("vJumpFalse"_Stz);
  
  i8 iVal;
  if ( ! mpTask->m_pDataStack->bPopAsInt_Quick(iVal) )
    THROW_INVALID_DSTACK("jumpFalse"_Stz);
  
  // n.b. the IP points to the jump arg
  
  if (iVal)
    // do not jump, skip over argument
    mpTask->IP.m_pCell++;
  else
    // jump to location
    mpTask->IP.m_pCell = mpTask->IP.m_pCell->m_pJumpTo;
  
  EXIT();
}
#endif

/*----------------------------------------------------------------------------*/
/* jumpTrue -- jump on true to location                                       */
/*----------------------------------------------------------------------------*/
void Task::vFlowJumpTrue(MP_Task mpTask)
{
  ENTRY("vJumpTrue"_Stz);
  
  i8 iVal;
  if ( ! mpTask->m_pDataStack->bPopAsInt(iVal) )
    THROW_INVALID_DSTACK("jumpTrue"_Stz);
  
  // n.b. the IP points to the jump arg
  
  if (iVal)
    // jump to location
    mpTask->IP.m_pCell = mpTask->IP.m_pCell->m_pJumpTo;
  else
    // do not jump, skip over argument
    mpTask->IP.m_pCell++;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* nop -- no op                                                               */
/*----------------------------------------------------------------------------*/
void Task::vFlowNop(MP_Task mpTask)
{
  ENTRY("vFlowNop"_Stz);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* retryPoint -- mark retry point                                             */
/*----------------------------------------------------------------------------*/
void Task::vFlowRetryPoint(MP_Task mpTask)
{
  ENTRY("vFlowRetryPoint"_Stz);
  
  mpTask->m_pRetryIP = mpTask->IP.m_pCell;
  
  mpTask->m_pDataStack->vRetryPoint();
  
  mpTask->m_pReturnStack->vRetryPoint();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* test -- print test                                                         */
/*----------------------------------------------------------------------------*/
void Task::vFlowTest(MP_Task mpTask)
{
  ENTRY("vFlowTest"_Stz);
  
  vTrace("[%s] Test\n"_Stz,mpTask->aJobName());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* throw -- throw c++ exception (for testing purposes)                        */
/*----------------------------------------------------------------------------*/
void Task::vFlowThrow(MP_Task mpTask)
{
  ENTRY("vFlowThrow"_Stz);
  
  char wBuffer[256];
  
  StrLib::vSprint(wBuffer,"THROW TEST"_Stz);
  
  throw wBuffer;
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
