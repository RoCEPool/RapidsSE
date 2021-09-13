/*============================================================================*/
/* Steps_OperandCount.cpp -- OperandCount step group                          */
/*============================================================================*/

#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "Steps.hpp"

#include "ePX.h"

#include "Op.hpp"
#include "Pipeline.hpp"
#include "StageSpec.hpp"

#include "Job.hpp"
#include "LogicUnit.hpp"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_BuffersIn(void)
{
  vAddStep("BuffersIn[]"_Stz,      &Step::vBuffersInGet);
  vAddStep("BuffersIn[].get"_Stz,  &Step::vBuffersInGet);
  vAddStep("BuffersIn[].inc"_Stz,  &Step::vBuffersInInc);
}

/*----------------------------------------------------------------------------*/
/* BuffersIn[].get -- fetch buffersIn                                         */
/*----------------------------------------------------------------------------*/
void Step::vBuffersInGet(void)
{
  ENTRY();

  m_pDataStack->vPushInt(m_p_TaskLogicUnit->m_xpStats->m_nBuffIn);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* BuffersIn[].inc -- inc the buffersIn                                       */
/*----------------------------------------------------------------------------*/
void Step::vBuffersInInc(void)
{
  ENTRY();

  //m_p_TaskLogicUnit->m_p_TaskLogicUnit->m_xpStats->m_nBuffIn++;
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
