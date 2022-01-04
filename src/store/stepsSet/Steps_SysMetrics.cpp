/*============================================================================*/
/* Steps_SysMetrics.cpp - system metrics step group                           */
/*============================================================================*/

#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "Steps.hpp"

#include "SysMetrics.hpp"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_SysMetrics(void)
{
  vAddStep("sysMetrics::new"_Stz,      &Step::vSysMetricsNew);
}


/*----------------------------------------------------------------------------*/
/* sysMetrics::new -- new sys metrics                                         */
/*----------------------------------------------------------------------------*/
void Step::vSysMetricsNew(void)
{
  ENTRY();
  
  SysMetrics* pSysMetrics = new SysMetrics;
  
  m_pDataStack->vPushPtr(pSysMetrics);

  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
