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
void Step::vAddSteps_OperandCount(void)
{
  vAddStep("OperandCount[]"_Stz,      &Step::vOperandCountGet);
  vAddStep("OperandCount[].get"_Stz,  &Step::vOperandCountGet);
  vAddStep("OperandCount[].inc"_Stz,  &Step::vOperandCountInc);
}

/*----------------------------------------------------------------------------*/
/* OperandCount[].get -- fetch operandCount                                   */
/*----------------------------------------------------------------------------*/
void Step::vOperandCountGet(void)
{
  ENTRY();

  m_pDataStack->vPushInt(m_p_TaskLogicUnit->m_iOperandCount);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* OperandCount[].inc -- inc the operandCount                                 */
/*----------------------------------------------------------------------------*/
void Step::vOperandCountInc(void)
{
  ENTRY();

  m_p_TaskLogicUnit->m_iOperandCount++;
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
