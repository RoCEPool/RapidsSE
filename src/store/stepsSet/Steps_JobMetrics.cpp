/*============================================================================*/
/* Steps_JobMetrics.cpp - job metrics step group                              */
/*============================================================================*/

#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "Steps.hpp"

#include "JobMetrics.hpp"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_JobMetrics(void)
{
  vAddStep("jobMetrics::collect"_Stz,  &Step::vJobMetricsCollect);
  vAddStep("jobMetrics::jobSet"_Stz,   &Step::vJobMetricsJobSet);
}


/*----------------------------------------------------------------------------*/
/* jobMetrics::collect -- collect job metrics                                 */
/*----------------------------------------------------------------------------*/
void Step::vJobMetricsCollect(void)
{
  ENTRY();

  JobId tJobId;
  tJobId = C_UNKNOWN;
  
  JobMetrics* pJobMetrics = new JobMetrics(tJobId);
  
  delete pJobMetrics;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* jobMetrics::jobSet -- get set of jobs for which we will get job metrics    */
/*----------------------------------------------------------------------------*/
void Step::vJobMetricsJobSet(void)
{
  ENTRY();
  
  IntStack* pIntStack = _pIntStack(m_pDataStack->pPopPtr(__func__));

  JobMetrics::vJobSet(pIntStack);
  
  EXIT();
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
