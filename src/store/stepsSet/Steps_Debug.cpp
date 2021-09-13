/*============================================================================*/
/* Steps_Debug.cpp - debug step group                                         */
/*============================================================================*/

#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "Steps.hpp"

#include "Job.hpp"
#include "Node_I.hpp"
#include "LogicUnit.hpp"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Debug(void)
{
  vAddStep("debug::breakpoint"_Stz,  &Step::vDebugBreakpoint);
  vAddStep("debug::flag@"_Stz,       &Step::vDebugFlagFetch);
  vAddStep("debug::flag!"_Stz,       &Step::vDebugFlagStore);
}

/*----------------------------------------------------------------------------*/
/* debug::breakpoint -- debug breakpoint                                      */
/*----------------------------------------------------------------------------*/
void Step::vDebugBreakpoint(void)
{
  ENTRY();

  vSeeFrameStack();
  
  BASE::vDebugPoint();
  
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* debug::flag@ -- fetch debug flag                                           */
/*----------------------------------------------------------------------------*/
void Step::vDebugFlagFetch(void)
{
  ENTRY();

  m_pDataStack->vPushInt(m_p_TaskLogicUnit->m_iDebugFlag);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* debug::flag! -- store debug flag                                           */
/*----------------------------------------------------------------------------*/
void Step::vDebugFlagStore(void)
{
  ENTRY();

  m_p_TaskLogicUnit->m_iDebugFlag = m_pDataStack->iPopInt(__func__);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
