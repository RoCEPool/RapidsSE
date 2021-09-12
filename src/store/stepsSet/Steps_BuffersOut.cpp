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
void Step::vAddSteps_BuffersOut(void)
{
  vAddStep("BuffersOut[]"_Stz,      &Step::vBuffersOutGet);
  vAddStep("BuffersOut[].get"_Stz,  &Step::vBuffersOutGet);
  vAddStep("BuffersOut[].inc"_Stz,  &Step::vBuffersOutInc);
}

/*----------------------------------------------------------------------------*/
/* BuffersOut[].get -- fetch buffersIn                                        */
/*----------------------------------------------------------------------------*/
void Step::vBuffersOutGet(void)
{
  ENTRY();

  m_pDataStack->vPushInt(m_p_TaskLogicUnit->m_iBuffersOut);

  EXIT();
}


/*----------------------------------------------------------------------------*/
/* BuffersOut[].inc -- inc the buffersOut                                     */
/*----------------------------------------------------------------------------*/
void Step::vBuffersOutInc(void)
{
  ENTRY();

  m_p_TaskLogicUnit->m_iBuffersOut++;

  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
