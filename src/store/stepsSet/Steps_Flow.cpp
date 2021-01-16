/*============================================================================*/
/* Steps_Flow.cpp - flow step group                                           */
/*============================================================================*/

#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "StepTiming.hpp"


/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Flow(void)
{
  vAddStep("hazard"_Stz,          &Step::vSenHazard);
  vAddStep("jump"_Stz,            &Step::vSenJump);
  vAddStep("jumpFalse"_Stz,       &Step::vSenJumpFalse);
  vAddStep("jumpTrue"_Stz,        &Step::vSenJumpTrue);
  vAddStep("nop"_Stz,             &Step::vSenNop);
  vAddStep("reenter"_Stz,         &Step::vSenReenter);
  vAddStep("test"_Stz,            &Step::vSenTest);
  vAddStep("throw"_Stz,           &Step::vSenThrow);
}

/*----------------------------------------------------------------------------*/
/* hazard -- should never be called                                           */
/*----------------------------------------------------------------------------*/
void Step::vSenHazard(void)
{
  ENTRY();
  
  vThrow(FK_CHECK,VL_FATAL,__func__,__FILE__,__LINE__,"Entered hazard"_Stz);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* jump -- jump to location                                                   */
/*----------------------------------------------------------------------------*/
void Step::vSenJump(void)
{
  ENTRY();
  
  m_aInxPtr = m_aInxPtr->m_aJumpTo;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* jumpFalse -- jump on false to location                                     */
/*----------------------------------------------------------------------------*/
void Step::vSenJumpFalse(void)
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
void Step::vSenJumpTrue(void)
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
void Step::vSenNop(void)
{
  ENTRY();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* reenter -- re-enter the word currenty being executed (i.e. start over)     */
/*----------------------------------------------------------------------------*/
void Step::vSenReenter(void)
{
  ENTRY();
  
  Word* pExecuting = CodeBlock::pToWord(m_aInxPtr);
  
  m_aInxPtr = pExecuting->aEntryCell();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* test -- print test                                                         */
/*----------------------------------------------------------------------------*/
void Step::vSenTest(void)
{
  ENTRY();
  
  vTrace("[%s] Test\n"_Stz,szJobName());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* throw -- throw c++ exception (for testing purposes)                        */
/*----------------------------------------------------------------------------*/
void Step::vSenThrow(void)
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
