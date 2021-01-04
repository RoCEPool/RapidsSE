/*============================================================================*/
/* Steps_StringA.cpp - stringA[] step group                                   */
/*============================================================================*/

#include "BASE.h"
#include "STORE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "StepTiming.hpp"

#include "Result.hpp"
#include "Job.hpp"

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
    
  m_pDataStack->vPushStr(( m_pJob->pLogicUnit_job() )->m_sStringA);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* stringA[].set -- set stringA[]                                             */
/*----------------------------------------------------------------------------*/
void Step::vStringASet(void)
{
  ENTRY();
    
  m_pDataStack->vPopStr(( m_pJob->pLogicUnit_job() )->m_sStringA, __func__);

  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
