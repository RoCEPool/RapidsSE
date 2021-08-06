/*============================================================================*/
/* Steps_StringA.cpp - stringA[] step group                                   */
/*============================================================================*/

#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "Steps.hpp"

#include "Node_I.hpp"
#include "Job.hpp"
#include "LogicUnit.hpp"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_StringA(void)
{
  vAddStep("stringA[]"_Stz,      &Step::vStringAGet);
  vAddStep("stringA[].get"_Stz,  &Step::vStringAGet);
  vAddStep("stringA[].set"_Stz,  &Step::vStringASet);
}

/*----------------------------------------------------------------------------*/
/* stringA[].get -- get stringA[]                                             */
/*----------------------------------------------------------------------------*/
void Step::vStringAGet(void)
{
  ENTRY();
    
  m_pDataStack->vPushStr(m_p_TaskLogicUnit->m_sStringA);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* stringA[].set -- set stringA[]                                             */
/*----------------------------------------------------------------------------*/
void Step::vStringASet(void)
{
  ENTRY();
    
  m_pDataStack->vPopStr(m_p_TaskLogicUnit->m_sStringA, __func__);

  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
