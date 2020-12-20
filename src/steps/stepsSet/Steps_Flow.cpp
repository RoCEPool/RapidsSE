/*============================================================================*/
/* Steps_Flow.cpp - flow step group                                           */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Flow(void)
{
  vAddStep("hazard"_Stz,          &Step::vFlowHazard);
  vAddStep("jump"_Stz,            &Step::vFlowJump);
  vAddStep("jumpFalse"_Stz,       &Step::vFlowJumpFalse);
  vAddStep("jumpTrue"_Stz,        &Step::vFlowJumpTrue);
  vAddStep("nop"_Stz,             &Step::vFlowNop);
  vAddStep("reenter"_Stz,         &Step::vFlowReenter);
  vAddStep("test"_Stz,            &Step::vFlowTest);
  vAddStep("throw"_Stz,           &Step::vFlowThrow);
}

/*----------------------------------------------------------------------------*/
/* hazard -- should never be called                                           */
/*----------------------------------------------------------------------------*/
void Step::vFlowHazard(void)
{
  ENTRY();
  
  vThrow(FK_CHECK,VL_FATAL,__func__,__FILE__,__LINE__,"Entered hazard"_Stz);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* jump -- jump to location                                                   */
/*----------------------------------------------------------------------------*/
void Step::vFlowJump(void)
{
  ENTRY();
  
  m_aInxPtr = m_aInxPtr->m_aJumpTo;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* jumpFalse -- jump on false to location                                     */
/*----------------------------------------------------------------------------*/
void Step::vFlowJumpFalse(void)
{
  ENTRY();
  
  i8 iVal = m_pDataStack->iPopAsInt(__func__);
  
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
/* jumpTrue -- jump on true to location                                       */
/*----------------------------------------------------------------------------*/
void Step::vFlowJumpTrue(void)
{
  ENTRY();
  
  i8 iVal = m_pDataStack->iPopAsInt(__func__);
  
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
  ENTRY();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* reenter -- re-enter the word currenty being executed (i.e. start over)     */
/*----------------------------------------------------------------------------*/
void Step::vFlowReenter(void)
{
  ENTRY();
  
  Word* pExecuting = Routine::pToWord(m_aInxPtr);
  
  m_aInxPtr = pExecuting->aEntryCell();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* test -- print test                                                         */
/*----------------------------------------------------------------------------*/
void Step::vFlowTest(void)
{
  ENTRY();
  
  vTrace("[%s] Test\n"_Stz,m_pTask->szJobName());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* throw -- throw c++ exception (for testing purposes)                        */
/*----------------------------------------------------------------------------*/
void Step::vFlowThrow(void)
{
  ENTRY();
  
  s256 sException;
  
  sException.vSprint("THROW TEST"_Stz);
  
  vThrow(FK_CHECK,VL_FATAL,__func__,__FILE__,__LINE__,sException);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
